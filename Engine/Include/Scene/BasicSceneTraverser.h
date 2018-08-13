#pragma once

#include "Scene/SceneTraverser.h"
#include "Scene/RenderQueue.h"

namespace FDX
{
	class FDX_EXPORT CBasicSceneTraverser : public ISceneTraverser
	{
	public:

		CBasicSceneTraverser();
		~CBasicSceneTraverser();

		virtual void Add(CRenderable *pRenderable) override;
		virtual void Remove(CRenderable *pRenderable) override;

		virtual void GetVisible(const CFrustum &pFrustum,
			CRenderQueue *pRenderQueue) override;

	private:

		Array<CRenderable*> mRenderables;
	};
}