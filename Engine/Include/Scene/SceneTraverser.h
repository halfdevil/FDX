#pragma once

#include "Scene/SceneNode.h"
#include "Scene/Renderable.h"
#include "Scene/RenderQueue.h"

namespace FDX
{
	class FDX_EXPORT ISceneTraverser
	{
	public:

		virtual ~ISceneTraverser()
		{
		}

		virtual void Add(CRenderable *pRenderable) = 0;
		virtual void Remove(CRenderable *pRenderable) = 0;

		virtual void GetVisible(const CFrustum &pFrustum, 
			CRenderQueue *pRenderQueue) = 0;
	};

	using ISceneTraverserPtr = SmartPtr<ISceneTraverser>;
}