#pragma once

#include "Scene/RenderNodeState.h"

namespace FDX
{
	class CRenderNode;

	class CRenderNodeCompare
	{
	public:

		bool operator()(CRenderNode *pA, CRenderNode *pB) const;
	};

	class CRenderNode
	{
	public:

		CRenderNode();
		~CRenderNode();

		void Draw();
		void DeleteChildren();

	public:

		CRenderNodeState *State;
		Set<CRenderNode*, CRenderNodeCompare> Children;
	};

	using CRenderNodePtr = SmartPtr<CRenderNode>;
}