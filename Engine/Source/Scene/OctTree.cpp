
#include "Scene/OctTree.h"

namespace FDX
{
	COctTreeNode::COctTreeNode(float pfX, float pfY, float pfZ, float pfSize, float pfMinSize)
		: mfSize(pfSize),
		mfMinSize(pfMinSize)
	{
		mAxisAlignedBox = CAxisAlignedBox(pfX - pfSize / 2, pfY - pfSize / 2,
			pfZ - pfSize / 2, pfX + pfSize / 2, pfY + pfSize / 2, pfZ + pfSize / 2);

		if (pfSize > pfMinSize)
		{
			auto lChild1 = std::make_unique<COctTreeNode>(pfX - pfSize / 2, pfY - pfSize / 2, 
				pfZ - pfSize / 2, pfSize / 2, pfMinSize);

			auto lChild2 = std::make_unique<COctTreeNode>(pfX + pfSize / 2, pfY - pfSize / 2,
				pfZ - pfSize / 2, pfSize / 2, pfMinSize);

			auto lChild3 = std::make_unique<COctTreeNode>(pfX - pfSize / 2, pfY - pfSize / 2,
				pfZ + pfSize / 2, pfSize / 2, pfMinSize);

			auto lChild4 = std::make_unique<COctTreeNode>(pfX + pfSize / 2, pfY - pfSize / 2,
				pfZ + pfSize / 2, pfSize / 2, pfMinSize);

			auto lChild5 = std::make_unique<COctTreeNode>(pfX - pfSize / 2, pfY + pfSize / 2,
				pfZ - pfSize / 2, pfSize / 2, pfMinSize);

			auto lChild6 = std::make_unique<COctTreeNode>(pfX + pfSize / 2, pfY + pfSize / 2,
				pfZ - pfSize / 2, pfSize / 2, pfMinSize);

			auto lChild7 = std::make_unique<COctTreeNode>(pfX - pfSize / 2, pfY + pfSize / 2,
				pfZ + pfSize / 2, pfSize / 2, pfMinSize);

			auto lChild8 = std::make_unique<COctTreeNode>(pfX + pfSize / 2, pfY + pfSize / 2,
				pfZ + pfSize / 2, pfSize / 2, pfMinSize);

			AddChild(std::move(lChild1));
			AddChild(std::move(lChild2));
			AddChild(std::move(lChild3));
			AddChild(std::move(lChild4));
			AddChild(std::move(lChild5));
			AddChild(std::move(lChild6));
			AddChild(std::move(lChild7));
			AddChild(std::move(lChild8));
		}
	}

	COctTreeNode::~COctTreeNode()
	{
	}

	void COctTreeNode::AddRenderable(CRenderable * pRenderable)
	{
		if (mAxisAlignedBox.Contains(pRenderable->GetAxisAlignedBox()) != EContainmentType::Culled)
		{
			if (IsLeafNode())
			{
				mRenderables.push_back(pRenderable);
			}
			else
			{
				for (uint32_t lIdx = 0; lIdx < GetNumChildren(); lIdx++)				
					GetChild(lIdx)->AddRenderable(pRenderable);
			}
		}
	}

	void COctTreeNode::RemoveRenderable(CRenderable * pRenderable)
	{
		if (mAxisAlignedBox.Contains(pRenderable->GetAxisAlignedBox()) != EContainmentType::Culled)
		{
			if (IsLeafNode())
			{
				auto lIt = std::find(mRenderables.begin(), 
					mRenderables.end(), pRenderable);

				if (lIt != mRenderables.end())
					mRenderables.erase(lIt);
			}
			else
			{
				for (uint32_t lIdx = 0; lIdx < GetNumChildren(); lIdx++)				
					GetChild(lIdx)->RemoveRenderable(pRenderable);				
			}
		}
	}

	void COctTreeNode::GetVisible(const CFrustum & pFrustum, CRenderQueue * pRenderQueue)
	{
		if (pFrustum.Contains(mAxisAlignedBox) != EContainmentType::Culled)
		{
			if (IsLeafNode())
			{
				for (const auto &lRenderable : mRenderables)
					pRenderQueue->AddRenderable(lRenderable);
			}
			else
			{
				for (uint32_t lIdx = 0; lIdx < GetNumChildren(); lIdx++)
					GetChild(lIdx)->GetVisible(pFrustum, pRenderQueue);
			}
		}
	}

	void COctTreeNode::Clear()
	{
		if (IsLeafNode())
		{
			mRenderables.clear();
		}
		else
		{
			for (uint32_t lIdx = 0; lIdx < GetNumChildren(); lIdx++)
				GetChild(lIdx)->Clear();
		}
	}
}