#pragma once

#include "Scene/BasicSceneTraverser.h"
#include "Scene/OctTree.h"

namespace FDX
{
	class FDX_EXPORT COctTreeTraverser : public CBasicSceneTraverser
	{
	public:

		COctTreeTraverser(COctTreeNode *pOctTree);
		~COctTreeTraverser();

		COctTreeNode* GetOctTree() const
		{
			return mOctTree;
		}

		void Add(CRenderable *pRenderable) override;
		void Remove(CRenderable *pRenderable) override;

		void GetVisible(const CFrustum &pFrustum,
			CRenderQueue *pRenderQueue) override;

	private:

		COctTreeNode *mOctTree;
	};
}