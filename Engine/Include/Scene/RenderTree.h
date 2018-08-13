#pragma once

#include "RenderNode.h"

namespace FDX
{
	class FDX_EXPORT CRenderTree
	{
	public:

		CRenderTree();
		~CRenderTree();

		CRenderNode* GetRoot() const
		{
			return mRoot;
		}

		CRenderNode* GetCurrentNode() const
		{
			return mCurrentNode;
		}

		void AddRenderNodeState(CRenderNodeState *pState);
		void ResetCurrentNode();
		void Draw();
		void Clear();

	private:

		CRenderNode *mRoot;
		CRenderNode *mTempNode;
		CRenderNode *mCurrentNode;
	};

	using CRenderTreePtr = SmartPtr<CRenderTree>;
}