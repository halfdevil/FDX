#pragma once

#include "Math/Vector3F.h"
#include "Math/Quaternion.h"
#include "Math/Matrix4F.h"
#include "Math/AxisAlignedBox.h"
#include "Scene/Component.h"

namespace FDX
{
	class CSceneNode;

	class FDX_EXPORT CSceneEntity
	{
	public:

		friend class CSceneNode;

		CSceneEntity();
		virtual ~CSceneEntity();

		CSceneEntity(const CSceneEntity &) = delete;
		CSceneEntity& operator=(const CSceneEntity&) = delete;

		const String& GetName() const
		{
			return mstrName;
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
				const_cast<CSceneEntity*>(this)->UpdateTransform();

			return mLocalTransform;
		}

		const CMatrix4F& GetGlobalTransform() const
		{
			if (!mbTransformUpdated)
				const_cast<CSceneEntity*>(this)->UpdateTransform();

			return mGlobalTransform;
		}

		template <typename T>
		T* GetComponent(uint32_t puiIdx) const
		{
			if (puiIdx < mComponents.size())
				return dynamic_cast<T*>(mComponents[puiIdx].get());

			return nullptr;
		}

		uint32_t GetNumComponents() const
		{
			return mComponents.size();
		}

		const CAxisAlignedBox& GetAxisAlignedBox() const
		{
			if (!mbTransformUpdated)
				const_cast<CSceneEntity*>(this)->UpdateTransform();

			return mAxisAlignedBox;
		}

		bool IsTransformUpdated() const
		{
			return mbTransformUpdated;
		}

		virtual void SetName(const String &pstrName);
		virtual void SetParent(CSceneNode *pParent);
		virtual void SetPosition(const CVector3F &pPosition);
		virtual void SetScale(const CVector3F &pScale);
		virtual void SetRotation(const CQuaternion &pRotation);
		virtual void SetAxisAlignedBox(const CAxisAlignedBox &pAabb);
		virtual void SetTransformUpdated(bool pbUpdated);
		virtual void UpdateTransform();

		virtual IComponent* AddComponent(IComponentPtr pComponent);
		virtual IComponentPtr RemoveComponent(IComponent *pComponent);

		virtual void Start();
		virtual void Stop();

		virtual void Update();
		virtual void FixedUpdate();

	protected:

		String mstrName;
		CSceneNode *mParent;
		Array<IComponentPtr> mComponents;
		CAxisAlignedBox mAxisAlignedBox;
		CVector3F mPosition;
		CVector3F mScale;
		CQuaternion mRotation;
		CMatrix4F mLocalTransform;
		CMatrix4F mGlobalTransform;
		bool mbTransformUpdated;
	};

	using CSceneEntityPtr = SmartPtr<CSceneEntity>;
}