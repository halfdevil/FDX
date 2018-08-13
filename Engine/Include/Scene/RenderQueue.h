#pragma once

#include "Core/ObjectPool.h"
#include "Graphics/DrawOperation.h"
#include "Graphics/ShadowMap.h"
#include "Scene/Renderable.h"
#include "Scene/MeshEntity.h"
#include "Scene/Material.h"
#include "Scene/Light.h"
#include "Scene/RenderNodeState.h"
#include "Scene/RenderNode.h"
#include "Scene/RenderTree.h"
#include "Scene/LightUniformBlock.h"
#include "Scene/PerFrameUniformBlock.h"

namespace FDX
{
	class FDX_EXPORT CRenderQueue
	{
	public:

		CRenderQueue();
		~CRenderQueue();

		CCamera* GetCamera() const
		{
			return mCamera;
		}

		CRenderQueue(const CRenderQueue&) = delete;
		CRenderQueue& operator=(const CRenderQueue&) = delete;

		EResult Create(IGraphicsDevice *pGraphicsDevice);

		void AddRenderable(CRenderable *pRenderable);
		void AddLight(ILight *pLight);
		void SetCamera(CCamera *pCamera);

		void RenderDiffuse(IGraphicsDevice *pGraphicsDevice);
		void RenderLight(IGraphicsDevice *pGraphicsDevice);
		void RenderShadow(IGraphicsDevice *pGraphicsDevice);

		void Reset();
		void Prepare();

	private:

		CRenderTree* GetTree(EPassType pPassType, int32_t liLightIdx);

		void AddToTree(CRenderable *pRenderable, IMaterial *pMaterial, 
			int32_t liLightIdx, EPassType pPassType);
		
		void AddRenderStatesToTree(CRenderTree *pTree, IMaterial *pMaterial, int32_t liLightIdx, 
			EPassType pPassType);

		void AddProgramToTree(CRenderTree *pTree, IMaterial *pMaterial, int32_t liLightIdx,
			EPassType pPassType);

		void AddTexturesToTree(CRenderTree *pTree, IMaterial *pMaterial, int32_t liLightIdx,
			EPassType pPassType);

		void AddLightsToTree(CRenderTree *pTree, IMaterial *pMaterial, int32_t liLightIdx,
			EPassType pPassType);

		void AddRenderableToTree(CRenderTree *pTree, CRenderable *pRenderable, IMaterial *pMaterial,
			int32_t liLightIdx, EPassType pPassType);

	public:

		static const uint32_t MaxRenderNodes = 2000;
		
		static CObjectPoolPtr<CRenderNodeState> RenderNodeStatePool;
		static CObjectPoolPtr<CRenderNode> RenderNodePool;

	private:
		
		CRenderTreePtr mDiffuseTree;	
		Array<CRenderTreePtr> mLightTree;
		Array<CRenderTreePtr> mShadowTree;
		Array<CRenderable*> mRenderables;
		Array<ILight*> mLights;
		CLightUniformBlockPtr mLightUniformBlock;
		CPerFrameUnifromBlockPtr mPerFrameUniformBlock;
		CCamera *mCamera;
		CRenderNodeState *mState;
		uint32_t muiRenderCount;
	};

	using CRenderQueuePtr = SmartPtr<CRenderQueue>;
}
