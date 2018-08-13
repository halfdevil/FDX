#pragma once

#include "Scene/SceneNode.h"
#include "Scene/Renderable.h"
#include "Scene/RenderQueue.h"

namespace FDX
{	
	class FDX_EXPORT COctTreeNode : public CSceneNode
	{
	public:

		COctTreeNode(float pfX, float pfY, float pfZ, float pfSize, float pfMinSize);
		~COctTreeNode();
		
		COctTreeNode* GetChild(uint32_t puiIdx) const
		{
			return GetChildAs<COctTreeNode>(puiIdx);
		}

		bool IsLeafNode() const
		{
			return GetNumChildren() == 0;				
		}

		void AddRenderable(CRenderable *pRenderable);
		void RemoveRenderable(CRenderable *pRenderable);

		void GetVisible(const CFrustum &pFrustum,
			CRenderQueue *pRenderQueue);

		void Clear();

	private:

		float mfSize;
		float mfMinSize;
		Array<CRenderable*> mRenderables;
	};

	using COctTreeNodePtr = SmartPtr<COctTreeNode>;
}