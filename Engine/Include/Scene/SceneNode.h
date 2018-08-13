#pragma once

#include "Math/AxisAlignedBox.h"
#include "Scene/Component.h"
#include "Scene/SceneEntity.h"

namespace FDX
{
	class ISceneTraverser;
	class CSceneNode;

	using CSceneNodePtr = SmartPtr<CSceneNode>;

	class FDX_EXPORT CSceneNode
	{
	public:

		CSceneNode();
		virtual ~CSceneNode();
		
		CSceneNode(const CSceneNode&) = delete;
		CSceneNode& operator=(const CSceneNode&) = delete;
		
		CSceneNode* GetParent()
		{
			return mParent;
		}

		const CVector3F& GetPosition() const
		{
			return mPosition;
		}

		const CVector3F& GetScale() const
		{
			return mScale;
		}

		const CQuaternion& GetRotation() const
		{
			return mRotation;
		}

		const CMatrix4F& GetLocalTransform() const
		{
			if (!mbTransformUpdated)
				const_cast<CSceneNode*>(this)->UpdateTransform();

			return mLocalTransform;
		}

		const CMatrix4F& GetGlobalTransform() const
		{
			if (!mbTransformUpdated)
				const_cast<CSceneNode*>(this)->UpdateTransform();

			return mGlobalTransform;
		}

		bool IsActive() const
		{
			return mbActive;
		}

		bool IsVisible() const
		{
			return mbVisible;
		}

		template <typename T>
		T* GetParentAs() const
		{
			if (mParent != nullptr)
				return static_cast<T*>(mParent);

			return nullptr;
		}
				
		template <typename T>
		T* GetChildAs(uint32_t puiIdx) const
		{
			if (puiIdx < mChildNodes.size())
				return static_cast<T*>(mChildNodes[puiIdx].get());

			return nullptr;
		}

		uint32_t GetNumChildren() const
		{
			return mChildNodes.size();
		}

		template <typename T>
		T* GetEntityAs(uint32_t puiIdx) const
		{
			if (puiIdx < mEntities.size())
				return static_cast<T*>(mEntities[puiIdx].get());

			return nullptr;
		}

		uint32_t GetNumEntity() const
		{
			return mEntities.size();
		}

		const CAxisAlignedBox& GetAxisAlignedBox() const
		{
			return mAxisAlignedBox;
		}
				
		virtual void SetParent(CSceneNode *pNode);
		virtual void SetActive(bool pbActive);
		virtual void SetVisible(bool pbVisible);
		virtual void UpdateTransform();

		virtual void SetPosition(const CVector3F &pPosition);
		virtual void SetScale(const CVector3F &pScale);
		virtual void SetRotation(const CQuaternion &pRotation);
		virtual void SetAxisAlignedBox(const CAxisAlignedBox &pAabb);

		virtual CSceneNode* AddChild(CSceneNodePtr pNode);
		virtual CSceneNodePtr RemoveChild(CSceneNode *pNode);
		virtual CSceneEntity* AddEntity(CSceneEntityPtr pEntity);
		virtual CSceneEntityPtr RemoveEntity(CSceneEntity *pEntity);

		virtual void Start();
		virtual void Stop();

		virtual void Update();
		virtual void FixedUpdate();

	protected:

		bool mbActive;
		bool mbVisible;
		CSceneNode *mParent;
		CVector3F mPosition;
		CVector3F mScale;
		CQuaternion mRotation;
		CAxisAlignedBox mAxisAlignedBox;
		CMatrix4F mLocalTransform;
		CMatrix4F mGlobalTransform;
		bool mbTransformUpdated;
		Array<CSceneEntityPtr> mEntities;
		Array<CSceneNodePtr> mChildNodes;
	};
}