
#include "Scene/RenderNodeState.h"
#include "Scene/PerFrameUniformBlock.h"

namespace FDX
{
	template <typename T> int32_t CompareValue(T pA, T pB)
	{
		if (pA == pB)
			return 0;

		return pA < pB ? -1 : 1;
	}

	CRenderNodeState::CRenderNodeState()
		: BlendState(nullptr),
		RasterizerState(nullptr),
		DepthState(nullptr),
		StencilState(nullptr),
		Program(nullptr),
		Light(nullptr),		
		DrawOperation(nullptr),
		Material(nullptr),
		GlobalMatrix(nullptr),
		Renderable(nullptr)
	{
	}

	CRenderNodeState::~CRenderNodeState()
	{
	}

	int32_t CRenderNodeState::Compare(const CRenderNodeState * pState)
	{
		switch (pState->StateType)
		{
		case ERenderNodeStateType::Pass:
			return ComparePass(pState);

		case ERenderNodeStateType::BlendState:
			return CompareBlendState(pState);

		case ERenderNodeStateType::RasterizerState:
			return CompareRasterizerState(pState);

		case ERenderNodeStateType::DepthState:
			return CompareDepthState(pState);

		case ERenderNodeStateType::StencilState:
			return CompareStencilState(pState);

		case ERenderNodeStateType::Program:
			return CompareProgram(pState);

		case ERenderNodeStateType::Light:
			return CompareLight(pState);

		case ERenderNodeStateType::Texture:
			return CompareTextures(pState);

		case ERenderNodeStateType::DrawOperation:
			return CompareDrawOperation(pState);

		case ERenderNodeStateType::Matrix:
			return CompareGlobalMatrix(pState);

		case ERenderNodeStateType::Draw:
			return CompareDraw(pState);
		}

		return -1;
	}

	void CRenderNodeState::CopyOf(const CRenderNodeState * pState)
	{
		StateType = pState->StateType;

		switch (pState->StateType)
		{
		case ERenderNodeStateType::Pass:
			Pass = pState->Pass;
			break;

		case ERenderNodeStateType::BlendState:
			BlendState = pState->BlendState;
			break;

		case ERenderNodeStateType::RasterizerState:
			RasterizerState = pState->RasterizerState;
			break;

		case ERenderNodeStateType::DepthState:
			DepthState = pState->DepthState;
			break;

		case ERenderNodeStateType::StencilState:
			StencilState = pState->StencilState;
			break;

		case ERenderNodeStateType::Program:
			Program = pState->Program;
			Pass = pState->Pass;
			UniformBlock = pState->UniformBlock;
			break;

		case ERenderNodeStateType::Light:
			Light = pState->Light;
			Program = pState->Program;
			break;

		case ERenderNodeStateType::Texture:
			Program = pState->Program;
			Textures = pState->Textures;
			break;

		case ERenderNodeStateType::DrawOperation:
			Pass = pState->Pass;
			Material = pState->Material;
			DrawOperation = pState->DrawOperation;
			break;

		case ERenderNodeStateType::Matrix:
			GlobalMatrix = pState->GlobalMatrix;
			Program = pState->Program;
			Camera = pState->Camera;
			UniformBlock = pState->UniformBlock;
			break;

		case ERenderNodeStateType::Draw:
			Pass = pState->Pass;
			Material = pState->Material;
			Renderable = pState->Renderable;
			Program = pState->Program;
			Camera = pState->Camera;
			break;
		}
	}

	void CRenderNodeState::Apply()
	{
		switch (StateType)
		{
		case ERenderNodeStateType::Pass:
			ApplyPass();
			break;

		case ERenderNodeStateType::BlendState:
			ApplyBlendState();
			break;

		case ERenderNodeStateType::RasterizerState:
			ApplyRasterizerState();
			break;

		case ERenderNodeStateType::DepthState:
			ApplyDepthState();
			break;

		case ERenderNodeStateType::StencilState:
			ApplyStencilState();
			break;

		case ERenderNodeStateType::Program:
			ApplyProgram();
			break;

		case ERenderNodeStateType::Light:
			ApplyLight();
			break;

		case ERenderNodeStateType::Texture:
			ApplyTextures();
			break;

		case ERenderNodeStateType::DrawOperation:
			ApplyDrawOperation();
			break;

		case ERenderNodeStateType::Matrix:
			ApplyGlobalMatrix();
			break;

		case ERenderNodeStateType::Draw:
			ApplyDraw();
			break;
		}
	}

	int32_t CRenderNodeState::ComparePass(const CRenderNodeState * pState)
	{
		return CompareValue(Pass, pState->Pass);
	}

	int32_t CRenderNodeState::CompareBlendState(const CRenderNodeState * pState)
	{
		return CompareValue(BlendState, pState->BlendState);
	}

	int32_t CRenderNodeState::CompareRasterizerState(const CRenderNodeState * pState)
	{
		return CompareValue(RasterizerState, pState->RasterizerState);
	}

	int32_t CRenderNodeState::CompareDepthState(const CRenderNodeState * pState)
	{
		return CompareValue(DepthState, pState->DepthState);
	}

	int32_t CRenderNodeState::CompareStencilState(const CRenderNodeState * pState)
	{
		return CompareValue(StencilState, pState->StencilState);
	}

	int32_t CRenderNodeState::CompareTextures(const CRenderNodeState * pState)
	{
		uint32_t luiNumTextures = CMathUtil::Min(Textures.size(),
			pState->Textures.size());

		for (uint32_t lIdx = 0; lIdx < luiNumTextures; lIdx++)
		{
			const STexture &lLeft = Textures[lIdx];
			const STexture &lRight = pState->Textures[lIdx];

			if (lLeft.Texture != lRight.Texture)
				return CompareValue(lLeft.Texture, lRight.Texture);
		}

		if (Textures.size() < pState->Textures.size())
			return -1;
		else if (Textures.size() > pState->Textures.size())
			return 1;

		return 0;
	}

	int32_t CRenderNodeState::CompareProgram(const CRenderNodeState * pState)
	{
		return CompareValue(Program, pState->Program);
	}

	int32_t CRenderNodeState::CompareLight(const CRenderNodeState * pState)
	{
		return CompareValue(Light, pState->Light);
	}

	int32_t CRenderNodeState::CompareDrawOperation(const CRenderNodeState * pState)
	{
		return CompareValue(DrawOperation->GetRenderData(),
			pState->DrawOperation->GetRenderData());
	}

	int32_t CRenderNodeState::CompareGlobalMatrix(const CRenderNodeState * pState)
	{
		return CompareValue(GlobalMatrix, pState->GlobalMatrix);
	}

	int32_t CRenderNodeState::CompareDraw(const CRenderNodeState * pState)
	{
		return CompareValue(Renderable, pState->Renderable);
	}

	void CRenderNodeState::ApplyPass()
	{
	}

	void CRenderNodeState::ApplyBlendState()
	{
		if (BlendState)
			BlendState->Apply();
	}

	void CRenderNodeState::ApplyRasterizerState()
	{
		if (RasterizerState)
			RasterizerState->Apply();
	}

	void CRenderNodeState::ApplyDepthState()
	{
		if (DepthState)
			DepthState->Apply();
	}

	void CRenderNodeState::ApplyStencilState()
	{
		if (StencilState)
			StencilState->Apply();
	}

	void CRenderNodeState::ApplyTextures()
	{
		for (uint32_t lIdx = 0; lIdx < Textures.size(); lIdx++)
		{
			if (Textures[lIdx].Texture != nullptr)
			{
				Program->GetConstants()->SetTexture(Textures[lIdx].Location,
					lIdx, Textures[lIdx].Texture);
			}
			else
			{
				break;
			}
		}
	}

	void CRenderNodeState::ApplyProgram()
	{
		if (Program)
		{			
			Program->Bind();

			if (UniformBlock != nullptr)
			{
				UniformBlock->Update();
				UniformBlock->Apply(Program);
			}
		}
	}

	void CRenderNodeState::ApplyLight()
	{
		if (Light->IsShadowCaster())
		{			
			const CMatrix4F lScaleBias =
			{
				0.5f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.5f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.5f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f
			};

			Program->GetConstants()->SetMatrix4("uShadowMatrix", 
				lScaleBias * 
				Light->GetCamera()->GetProjection() * 
				Light->GetCamera()->GetView());

			Light->GetShadowMap()->Apply(Program);
		}
	}

	void CRenderNodeState::ApplyDrawOperation()
	{
		if (DrawOperation)
		{
			DrawOperation->GetRenderData()->Activate(DrawOperation->GetActiveVertexBuffer(),
				DrawOperation->GetActiveIndexBuffer());
		}
	}

	void CRenderNodeState::ApplyGlobalMatrix()
	{
		if (Program)
		{			
			CMatrix4F lWorldView = Camera->GetView() * *GlobalMatrix;			
			CMatrix4F lWorldViewProj = Camera->GetProjection() * 
				Camera->GetView() * *GlobalMatrix;

			CMatrix3F lNormal = CMatrix3F::InvertTransposeMatrix(
				CMatrix3F::FromMatrix4(lWorldView));

			if (UniformBlock != nullptr)
			{
				auto lPerFrameUniformBlock = dynamic_cast<
					CPerFrameUnifromBlock*>(UniformBlock);

				lPerFrameUniformBlock->SetWorldMatrix(*GlobalMatrix);
				lPerFrameUniformBlock->SetWorldViewMatrix(lWorldView);
				lPerFrameUniformBlock->SetWorldViewProjMatrix(lWorldViewProj);
				lPerFrameUniformBlock->SetViewMatrix(Camera->GetView());
				lPerFrameUniformBlock->SetProjMatrix(Camera->GetProjection());

				lPerFrameUniformBlock->Update();
				lPerFrameUniformBlock->Apply(Program);
			}
			else
			{
				Program->GetConstants()->SetMatrix4("uWorldMatrix", *GlobalMatrix);
				Program->GetConstants()->SetMatrix4("uWorldViewMatrix", lWorldView);
				Program->GetConstants()->SetMatrix4("uWorldViewProjMatrix", lWorldViewProj);
				Program->GetConstants()->SetMatrix4("uViewMatrix", Camera->GetView());
				Program->GetConstants()->SetMatrix4("uProjMatrix", Camera->GetProjection());
			}

			Program->GetConstants()->SetMatrix3("uNormalMatrix", lNormal);
		}
	}

	void CRenderNodeState::ApplyDraw()
	{
		if (Material)		
			Material->StartPass(Pass);		

		if (Renderable)
		{
			Renderable->UpdateGraphics(Camera, Program);
			Renderable->GetDrawOperation()->Draw();
		}

		if (Material)
			Material->EndPass(Pass);
	}
}