
#include "Scene/SceneEntity.h"
#include "Scene/SceneNode.h"

namespace FDX
{
	CSceneEntity::CSceneEntity()
		: mParent(nullptr),
		mScale(1.0f),
		mbTransformUpdated(false)
	{
	}

	CSceneEntity::~CSceneEntity()
	{		
	}

	void CSceneEntity::SetName(const String & pstrName)
	{
		mstrName = pstrName;
	}

	void CSceneEntity::SetParent(CSceneNode * pParent)
	{
		if (mParent != pParent)
			pParent->AddEntity(mParent->RemoveEntity(this));
	}

	void CSceneEntity::SetPosition(const CVector3F & pPosition)
	{
		mPosition = pPosition;
		mbTransformUpdated = false;
	}

	void CSceneEntity::SetScale(const CVector3F & pScale)
	{
		mScale = pScale;
		mbTransformUpdated = false;
	}

	void CSceneEntity::SetRotation(const CQuaternion & pRotation)
	{
		mRotation = pRotation;
		mbTransformUpdated = false;
	}

	void CSceneEntity::SetAxisAlignedBox(const CAxisAlignedBox & pAabb)
	{
		mAxisAlignedBox = pAabb;
	}

	void CSceneEntity::SetTransformUpdated(bool pbUpdated)
	{
		mbTransformUpdated = pbUpdated;
	}

	void CSceneEntity::UpdateTransform()
	{
		mLocalTransform = CMatrix4F::TranslationMatrix(mPosition) *
			mRotation.GetMatrix4() *
			CMatrix4F::ScaleMatrix(mScale);

		if (mParent != nullptr)
		{
			mGlobalTransform = mLocalTransform * mParent->GetGlobalTransform();
		}
		else
		{
			mGlobalTransform = mLocalTransform;
		}

		mAxisAlignedBox = mGlobalTransform.Transform(mAxisAlignedBox);
		mbTransformUpdated = true;
	}

	IComponent * CSceneEntity::AddComponent(IComponentPtr pComponent)
	{
		pComponent->SetEntity(this);
		mComponents.push_back(std::move(pComponent));

		return mComponents[mComponents.size() - 1].get();
	}

	IComponentPtr CSceneEntity::RemoveComponent(IComponent * pComponent)
	{
		auto lIt = std::find_if(
			mComponents.begin(),
			mComponents.end(),
			[pComponent](const IComponentPtr &pV) {
				return pV.get() == pComponent;
			}
		);

		IComponentPtr lComponent = std::move(*lIt);
		mComponents.erase(lIt);

		return lComponent;
	}

	void CSceneEntity::Start()
	{
		for (const auto &lComponent : mComponents)
			lComponent->Start();
	}

	void CSceneEntity::Stop()
	{
		for (const auto &lComponent : mComponents)
			lComponent->Stop();
	}

	void CSceneEntity::Update()
	{
		for (const auto &lComponent : mComponents)
			lComponent->Update();
	}

	void CSceneEntity::FixedUpdate()
	{
		for (const auto &lComponent : mComponents)
			lComponent->FixedUpdate();
	}
}