
#include "Scene/OctTreeTraverser.h"

namespace FDX
{
	COctTreeTraverser::COctTreeTraverser(COctTreeNode * pOctTree)
		: mOctTree(pOctTree)
	{
	}

	COctTreeTraverser::~COctTreeTraverser()
	{
	}

	void COctTreeTraverser::Add(CRenderable * pRenderable)
	{
		mOctTree->AddRenderable(pRenderable);
	}

	void COctTreeTraverser::Remove(CRenderable * pRenderable)
	{
		mOctTree->RemoveRenderable(pRenderable);
	}

	void COctTreeTraverser::GetVisible(const CFrustum & pFrustum, CRenderQueue * pRenderQueue)
	{
		mOctTree->GetVisible(pFrustum, pRenderQueue);
	}
}