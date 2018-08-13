
#include "Scene/RenderTree.h"
#include "Scene/RenderQueue.h"

namespace FDX
{
	CRenderTree::CRenderTree()
	{
		mRoot = CRenderQueue::RenderNodePool->GetObject();
		mTempNode = CRenderQueue::RenderNodePool->GetObject();
		mCurrentNode = mRoot;
	}

	CRenderTree::~CRenderTree()
	{
		if (mRoot != nullptr)
			mRoot->DeleteChildren();

		CRenderQueue::RenderNodePool->ReleaseObject(mTempNode);
		CRenderQueue::RenderNodePool->ReleaseObject(mRoot);
	}

	void CRenderTree::AddRenderNodeState(CRenderNodeState * pState)
	{
		mTempNode->State = pState;

		auto lIt = mCurrentNode->Children.find(mTempNode);
		if (lIt != mCurrentNode->Children.end())
		{
			mCurrentNode = (*lIt);
		}
		else
		{
			CRenderNode *lNode = CRenderQueue::RenderNodePool->GetObject();
			CRenderNodeState *lState = CRenderQueue::RenderNodeStatePool->GetObject();

			lState->CopyOf(pState);
			lNode->State = lState;

			mCurrentNode->Children.insert(lNode);
			mCurrentNode = lNode;
		}
	}

	void CRenderTree::ResetCurrentNode()
	{
		mCurrentNode = mRoot;
	}

	void CRenderTree::Draw()
	{
		if (mRoot != nullptr)
			mRoot->Draw();
	}

	void CRenderTree::Clear()
	{
		if (mRoot != nullptr)
			mRoot->DeleteChildren();

		mCurrentNode = mRoot;
	}
}