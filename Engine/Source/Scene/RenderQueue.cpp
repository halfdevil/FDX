
#include "Scene/RenderQueue.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CObjectPoolPtr<CRenderNodeState> CRenderQueue::RenderNodeStatePool = nullptr;
	CObjectPoolPtr<CRenderNode> CRenderQueue::RenderNodePool = nullptr;
		
	CRenderQueue::CRenderQueue()
		: muiRenderCount(1),
		mDiffuseTree(nullptr),
		mLightUniformBlock(nullptr),
		mPerFrameUniformBlock(nullptr),
		mCamera(nullptr)
	{		
	}

	CRenderQueue::~CRenderQueue()
	{
		RenderNodeStatePool->ReleaseObject(mState);
	}

	EResult CRenderQueue::Create(IGraphicsDevice * pGraphicsDevice)
	{
		EResult lResult = EResult::Success;

		mPerFrameUniformBlock = std::make_unique<CPerFrameUnifromBlock>();
		lResult = mPerFrameUniformBlock->Create(pGraphicsDevice, 0);
		
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CPerFrameUnifromBlock::Create() failed");
		}
		else
		{
			mLightUniformBlock = std::make_unique<CLightUniformBlock>();
			lResult = mLightUniformBlock->Create(pGraphicsDevice, 10, 1);

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CLightUniformBlock::Create() failed");
			}
			else
			{
				if (RenderNodePool == nullptr)
					RenderNodePool = std::make_unique<CObjectPool<CRenderNode>>(MaxRenderNodes);

				if (RenderNodeStatePool == nullptr)
					RenderNodeStatePool = std::make_unique<CObjectPool<CRenderNodeState>>(MaxRenderNodes);

				mDiffuseTree = std::make_unique<CRenderTree>();
				mState = RenderNodeStatePool->GetObject();
			}
		}

		return lResult;
	}

	void CRenderQueue::AddRenderable(CRenderable * pRenderable)
	{
		if (muiRenderCount == pRenderable->GetRenderCount())
			return;

		switch (pRenderable->GetRenderableType())
		{
		case ERenderableType::Normal:
			{
				pRenderable->SetRenderCount(muiRenderCount);
				mRenderables.push_back(pRenderable);
			}
			break;

		case ERenderableType::Mesh:
			{
				CMeshEntity *lMesh = dynamic_cast<CMeshEntity*>(pRenderable);
				for (uint32_t lIdx = 0; lIdx < lMesh->GetNumSubMeshes(); lIdx++)
				{
					CSubMeshEntity *lSubMesh = lMesh->GetSubMesh(lIdx);
					AddRenderable(lSubMesh);
				}
			}
			break;

		default:
			break;
		}
	}

	void CRenderQueue::AddLight(ILight * pLight)
	{
		if (pLight != nullptr)
		{
			mLights.push_back(pLight);
			mLightTree.push_back(std::make_unique<CRenderTree>());
			mShadowTree.push_back(std::make_unique<CRenderTree>());
			mLightUniformBlock->AddLight(pLight);
		}
	}

	void CRenderQueue::SetCamera(CCamera * pCamera)
	{
		mCamera = pCamera;
	}

	void CRenderQueue::RenderDiffuse(IGraphicsDevice *pGraphicsDevice)
	{
		mDiffuseTree->Draw();
	}

	void CRenderQueue::RenderLight(IGraphicsDevice *pGraphicsDevice)
	{
		for (const auto &lLightNode : mLightTree)
			lLightNode->Draw();
	}

	void CRenderQueue::RenderShadow(IGraphicsDevice * pGraphicsDevice)
	{
		for (uint32_t lIdx = 0; lIdx < mLights.size(); lIdx++)
		{
			if (mLights[lIdx]->IsShadowCaster())
			{
				mLights[lIdx]->GetShadowMap()->Activate();
				mShadowTree[lIdx]->Draw();
				mLights[lIdx]->GetShadowMap()->Deactivate();
			}
		}
	}

	void CRenderQueue::Reset()
	{
		for (const auto &lLightNode : mLightTree)
			lLightNode->Clear();

		for (const auto &lShadowNode : mShadowTree)
			lShadowNode->Clear();

		mDiffuseTree->Clear();
		mRenderables.clear();

		muiRenderCount++;
	}

	void CRenderQueue::Prepare()
	{
		if (mCamera == nullptr)
			return;

		for (const auto &lRenderable : mRenderables)
		{
			IMaterial *lMaterial = lRenderable->GetMaterial();

			if (!lMaterial->IsTransparent())
			{
				if (lMaterial->UsesPass(EPassType::Light))
				{
					for (int32_t lIdx = 0; lIdx < (int32_t)mLights.size(); lIdx++)
						AddToTree(lRenderable, lMaterial, lIdx, EPassType::Light);
				}
				else if (lMaterial->UsesPass(EPassType::Diffuse))
				{
					AddToTree(lRenderable, lMaterial, -1, EPassType::Diffuse);
				}
				
				if (lMaterial->UsesPass(EPassType::Shadow))
				{
					for (int32_t lIdx = 0; lIdx < (int32_t)mLights.size(); lIdx++)
					{
						if (mLights[lIdx]->IsShadowCaster())
							AddToTree(lRenderable, lMaterial, lIdx, EPassType::Shadow);
					}
				}
			}
		}
	}

	CRenderTree * CRenderQueue::GetTree(EPassType pPassType, int32_t liLightIdx)
	{
		switch (pPassType)
		{
		case EPassType::Diffuse:
			return mDiffuseTree.get();

		case EPassType::Light:
			return mLightTree[liLightIdx].get();

		case EPassType::Shadow:
			return mShadowTree[liLightIdx].get();
		}

		return nullptr;
	}

	void CRenderQueue::AddToTree(CRenderable *pRenderable, IMaterial *pMaterial,
		int32_t liLightIdx, EPassType pPassType)
	{
		CRenderTree *lTree = GetTree(pPassType, liLightIdx);
		lTree->ResetCurrentNode();

		if (lTree != nullptr)
		{
			CRenderNodeState lState;
			lState.StateType = ERenderNodeStateType::Pass;
			lTree->AddRenderNodeState(&lState);

			if (pPassType != EPassType::Shadow)
			{
				AddRenderStatesToTree(lTree, pMaterial, liLightIdx, pPassType);
				AddProgramToTree(lTree, pMaterial, liLightIdx, pPassType);
				AddLightsToTree(lTree, pMaterial, liLightIdx, pPassType);
				AddTexturesToTree(lTree, pMaterial, liLightIdx, pPassType);
				AddRenderableToTree(lTree, pRenderable, pMaterial, liLightIdx, pPassType);
			}
			else
			{
				AddProgramToTree(lTree, pMaterial, liLightIdx, pPassType);
				AddRenderableToTree(lTree, pRenderable, pMaterial, liLightIdx, pPassType);
			}
		}
	}

	void CRenderQueue::AddRenderStatesToTree(CRenderTree * pTree, IMaterial * pMaterial,
		int32_t liLightIdx, EPassType pPassType)
	{
		mState->StateType = ERenderNodeStateType::DepthState;
		mState->DepthState = pMaterial->GetDepthState(pPassType);
		pTree->AddRenderNodeState(mState);

		mState->StateType = ERenderNodeStateType::RasterizerState;
		mState->RasterizerState = pMaterial->GetRasterizerState(pPassType);
		pTree->AddRenderNodeState(mState);

		mState->StateType = ERenderNodeStateType::StencilState;
		mState->StencilState = pMaterial->GetStencilState(pPassType);
		pTree->AddRenderNodeState(mState);

		mState->StateType = ERenderNodeStateType::BlendState;
		mState->BlendState = pMaterial->GetBlendState(pPassType);
		pTree->AddRenderNodeState(mState);
	}

	void CRenderQueue::AddProgramToTree(CRenderTree * pTree, IMaterial * pMaterial,
		int32_t liLightIdx, EPassType pPassType)
	{	
		mState->StateType = ERenderNodeStateType::Program;
		mState->Pass = pPassType;		
		mState->UniformBlock = mLightUniformBlock.get();
		mState->Program = pMaterial->GetProgram(pPassType);		

		pTree->AddRenderNodeState(mState);
	}

	void CRenderQueue::AddTexturesToTree(CRenderTree * pTree, IMaterial * pMaterial,
		int32_t liLightIdx, EPassType pPassType)
	{
		mState->StateType = ERenderNodeStateType::Texture;
		mState->Program = pMaterial->GetProgram(pPassType);
		mState->Textures.resize(pMaterial->GetNumTextures(pPassType));

		for (uint32_t lIdx = 0; lIdx < mState->Textures.size(); lIdx++)
		{
			uint32_t luiLoc = 0;
			ITexture *lTexture = pMaterial->GetTexture(pPassType,
				lIdx, &luiLoc);
						
			mState->Textures[lIdx].Unit = lIdx;
			mState->Textures[lIdx].Location = luiLoc;
			mState->Textures[lIdx].Texture = lTexture;
		}

		pTree->AddRenderNodeState(mState);
		mState->Textures.clear();
	}

	void CRenderQueue::AddLightsToTree(CRenderTree * pTree, IMaterial * pMaterial, 
		int32_t liLightIdx, EPassType pPassType)
	{
		if (liLightIdx != -1)
		{
			mState->StateType = ERenderNodeStateType::Light;
			mState->Light = mLights[liLightIdx];
			mState->Program = pMaterial->GetProgram(pPassType);			

			pTree->AddRenderNodeState(mState);
		}
	}

	void CRenderQueue::AddRenderableToTree(CRenderTree * pTree, CRenderable *pRenderable,
		IMaterial * pMaterial, int32_t liLightIdx, EPassType pPassType)
	{		
		CCamera *lCamera = mCamera;

		if (pPassType == EPassType::Shadow)
		{
			if (liLightIdx != -1)
				lCamera = mLights[liLightIdx]->GetCamera();
		}

		mState->StateType = ERenderNodeStateType::DrawOperation;
		mState->DrawOperation = pRenderable->GetDrawOperation();
		pTree->AddRenderNodeState(mState);

		mState->StateType = ERenderNodeStateType::Matrix;
		mState->GlobalMatrix = pRenderable->GetGlobalTransformPtr();
		mState->UniformBlock = mPerFrameUniformBlock.get();
		mState->Camera = lCamera;
		pTree->AddRenderNodeState(mState);

		mState->StateType = ERenderNodeStateType::Draw;
		mState->Pass = pPassType;
		mState->Material = pMaterial;
		mState->Camera = lCamera;
		mState->Program = pMaterial->GetProgram(pPassType);
		mState->Renderable = pRenderable;
		pTree->AddRenderNodeState(mState);
	}
}