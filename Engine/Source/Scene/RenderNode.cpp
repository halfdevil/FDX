
#include "Scene/RenderNode.h"
#include "Scene/RenderQueue.h"

namespace FDX
{
	bool CRenderNodeCompare::operator()(CRenderNode * pA, CRenderNode * pB) const
	{
		if (pA->State->Compare(pB->State) > 0)
			return true;

		return false;
	}

	CRenderNode::CRenderNode()
		: State(nullptr)
	{
	}

	CRenderNode::~CRenderNode()
	{
	}

	void CRenderNode::Draw()
	{
		if (State)
			State->Apply();

		for (const auto &lNode : Children)
			lNode->Draw();
	}

	void CRenderNode::DeleteChildren()
	{
		for (const auto &lNode : Children)
		{
			lNode->DeleteChildren();

			if (lNode->State != nullptr)
				CRenderQueue::RenderNodeStatePool->ReleaseObject(lNode->State);

			CRenderQueue::RenderNodePool->ReleaseObject(lNode);
		}

		Children.clear();
	}
}