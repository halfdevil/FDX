
#include "Scene/SceneNode.h"
#include "Scene/SceneTraverser.h"
#include "OS/System.h"

namespace FDX
{
	CSceneNode::CSceneNode()
		: mParent(nullptr),
		mScale(1.0f),
		mbActive(true),
		mbVisible(false)
	{
	}

	CSceneNode::~CSceneNode()
	{
	}

	CSceneNode* CSceneNode::AddChild(CSceneNodePtr pNode)
	{
		pNode->mParent = this;
		mChildNodes.push_back(std::move(pNode));

		return mChildNodes[mChildNodes.size() - 1].get();
	}

	CSceneNodePtr CSceneNode::RemoveChild(CSceneNode * pNode)
	{
		auto lIt = std::find_if(
			mChildNodes.begin(), 
			mChildNodes.end(),
			[pNode](const CSceneNodePtr &pV) { 
				return pV.get() == pNode; 
			}
		);

		CSceneNodePtr lNode = std::move(*lIt);
		mChildNodes.erase(lIt);
	
		return lNode;
	}

	void CSceneNode::SetParent(CSceneNode * pNode)
	{
		if (mParent != pNode)		
			pNode->AddChild(mParent->RemoveChild(this));		
	}

	void CSceneNode::SetActive(bool pbActive)
	{
		mbActive = pbActive;
	}

	void CSceneNode::SetVisible(bool pbVisible)
	{
		mbVisible = pbVisible;
	}

	void CSceneNode::UpdateTransform()
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

		mbTransformUpdated = true;

		for (const auto &lEntity : mEntities)
			lEntity->UpdateTransform();

		for (const auto &lChild : mChildNodes)
			lChild->UpdateTransform();
	}

	void CSceneNode::SetPosition(const CVector3F & pPosition)
	{
		mPosition = pPosition;
		mbTransformUpdated = false;
	}

	void CSceneNode::SetScale(const CVector3F & pScale)
	{
		mScale = pScale;
		mbTransformUpdated = false;
	}

	void CSceneNode::SetRotation(const CQuaternion & pRotation)
	{
		mRotation = pRotation;
		mbTransformUpdated = false;
	}

	void CSceneNode::SetAxisAlignedBox(const CAxisAlignedBox & pAabb)
	{
		mAxisAlignedBox = pAabb;
	}

	CSceneEntity * CSceneNode::AddEntity(CSceneEntityPtr pEntity)
	{
		pEntity->mParent = this;
		mEntities.push_back(std::move(pEntity));

		return mEntities[mEntities.size() - 1].get();
	}

	CSceneEntityPtr CSceneNode::RemoveEntity(CSceneEntity * pEntity)
	{
		auto lIt = std::find_if(
			mEntities.begin(),
			mEntities.end(),
			[pEntity](const CSceneEntityPtr &pV) {
				return pV.get() == pEntity;
			}
		);

		CSceneEntityPtr lEntity = std::move(*lIt);
		mEntities.erase(lIt);

		return lEntity;
	}

	void CSceneNode::Start()
	{
		for (const auto &lEntity : mEntities)
			lEntity->Start();

		for (const auto &lChild : mChildNodes)
			lChild->Start();
	}

	void CSceneNode::Stop()
	{
		for (const auto &lEntity : mEntities)
			lEntity->Stop();

		for (const auto &lChild : mChildNodes)
			lChild->Stop();
	}

	void CSceneNode::Update()
	{
		for (const auto &lEntity : mEntities)
			lEntity->Update();

		for (const auto &lChild : mChildNodes)
			lChild->Update();
	}

	void CSceneNode::FixedUpdate()
	{
		for (const auto &lEntity : mEntities)
			lEntity->FixedUpdate();

		for (const auto &lChild : mChildNodes)
			lChild->FixedUpdate();
	}
}