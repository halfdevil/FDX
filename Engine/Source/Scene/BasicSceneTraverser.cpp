
#include "Scene/BasicSceneTraverser.h"

namespace FDX
{
	CBasicSceneTraverser::CBasicSceneTraverser()
	{		
	}

	CBasicSceneTraverser::~CBasicSceneTraverser()
	{
	}

	void CBasicSceneTraverser::Add(CRenderable * pRenderable)
	{
		auto lIt = std::find(mRenderables.begin(), mRenderables.end(), pRenderable);
		if (lIt == mRenderables.end())
		{
			mRenderables.push_back(pRenderable);
		}
	}

	void CBasicSceneTraverser::Remove(CRenderable * pRenderable)
	{
		auto lIt = std::find(mRenderables.begin(), mRenderables.end(), pRenderable);
		if (lIt != mRenderables.end())
		{
			mRenderables.erase(lIt);
		}
	}

	void CBasicSceneTraverser::GetVisible(const CFrustum & pFrustum, CRenderQueue * pRenderQueue)
	{
		for (const auto &lRenderable : mRenderables)
		{
			CAxisAlignedBox lAabb = lRenderable->GetAxisAlignedBox();
			lAabb = lRenderable->GetGlobalTransform().Transform(lAabb);

			if (pFrustum.IsVisible(lAabb))
			{
				pRenderQueue->AddRenderable(lRenderable);
			}
		}
	}
}