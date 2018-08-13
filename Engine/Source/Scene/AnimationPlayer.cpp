
#include "Scene/AnimationPlayer.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CJoint::CJoint(uint32_t puiIndex, int32_t piParent, 
		const CMatrix4F & pLocalTransform)
		: muiIndex(puiIndex), 
		miParent(piParent), 
		mLocalTransform(pLocalTransform)
	{
	}

	CJoint::CJoint()
		: miParent(-1)
	{
	}

	CJoint::~CJoint()
	{
	}

	inline void CJoint::SetIndex(uint32_t puiIndex)
	{
		muiIndex = puiIndex;
	}

	inline void CJoint::SetParent(int32_t piParent)
	{
		miParent = piParent;
	}

	inline void CJoint::SetAnimated(bool pbAnimated)
	{
		mbAnimated = pbAnimated;
	}

	inline void CJoint::SetLocalTransform(const CMatrix4F & pLocalTransform)
	{
		mLocalTransform = pLocalTransform;
	}

	inline void CJoint::SetInvLocalTransform(const CMatrix4F & pInvLocalTransform)
	{
		mInvLocalTransform = pInvLocalTransform;
	}

	inline void CJoint::SetAnimatedTransform(const CMatrix4F & pAnimatedTransform)
	{
		mAnimatedTransform = pAnimatedTransform;
	}

	CJointTransform::CJointTransform(const CVector3F & pPosition, const CQuaternion & pRotation)
		: mPosition(pPosition),
		mOrientation(pRotation)
	{
	}

	CJointTransform::CJointTransform()
	{
	}

	CJointTransform::~CJointTransform()
	{
	}

	inline CMatrix4F CJointTransform::GetMatrix4() const
	{
		return CMatrix4F::TranslationMatrix(mPosition) *
			mOrientation.GetMatrix4();
	}

	inline void CJointTransform::SetPosition(const CVector3F & pPosition)
	{
		mPosition = pPosition;
	}

	inline void CJointTransform::SetScale(const CVector3F & pScale)
	{
		mScale = pScale;
	}

	inline void CJointTransform::SetOrientation(const CQuaternion & pRotation)
	{
		mOrientation = pRotation;
	}

	CJointTransform CJointTransform::Interpolate(const CJointTransform & pJointA, const CJointTransform & pJointB, float pfFactor)
	{
		CJointTransform lJointTransform;

		lJointTransform.mPosition.X = pJointA.mPosition.X + (pJointB.mPosition.X - pJointA.mPosition.X) * pfFactor;
		lJointTransform.mPosition.Y = pJointA.mPosition.Y + (pJointB.mPosition.Y - pJointA.mPosition.Y) * pfFactor;
		lJointTransform.mPosition.Z = pJointA.mPosition.Z + (pJointB.mPosition.Z - pJointA.mPosition.Z) * pfFactor;

		lJointTransform.mScale.X = pJointA.mScale.X + (pJointB.mScale.X - pJointA.mScale.X) * pfFactor;
		lJointTransform.mScale.Y = pJointA.mScale.Y + (pJointB.mScale.Y - pJointA.mScale.Y) * pfFactor;
		lJointTransform.mScale.Z = pJointA.mScale.Z + (pJointB.mScale.Z - pJointA.mScale.Z) * pfFactor;

		lJointTransform.mOrientation = pJointA.mOrientation.Lerp(pJointB.mOrientation, pfFactor);

		return lJointTransform;
	}

	CKeyFrame::CKeyFrame(float pfTime, Array<CJointTransformPtr>&& pJointTransforms)
		: mfTime(pfTime), 
		mJointTransforms(std::move(pJointTransforms))
	{
	}

	CKeyFrame::CKeyFrame()
		: mfTime(0.0f)
	{
	}

	CKeyFrame::~CKeyFrame()
	{
	}

	inline void CKeyFrame::SetTime(float pfTime)
	{
		mfTime = pfTime;
	}

	inline void CKeyFrame::SetJointTransforms(Array<CJointTransformPtr>&& pJointTransforms)
	{
		mJointTransforms = std::move(pJointTransforms);
	}

	CAnimationClip::CAnimationClip(const String & pstrName, float pfDuration, Array<CKeyFramePtr>&& pKeyFrames)
		: mstrName(pstrName),
		mfDuration(pfDuration),
		mKeyFrames(std::move(pKeyFrames))
	{
	}

	CAnimationClip::CAnimationClip()
		: mfDuration(0.0f)
	{
	}

	CAnimationClip::~CAnimationClip()
	{
	}

	inline void CAnimationClip::SetName(const String & pstrName)
	{
		mstrName = pstrName;
	}

	inline void CAnimationClip::SetDuration(float pfDuration)
	{
		mfDuration = pfDuration;
	}

	inline void CAnimationClip::SetKeyFrames(Array<CKeyFramePtr>&& pKeyFrames)
	{
		mKeyFrames = std::move(pKeyFrames);
	}

	CSkinningData::CSkinningData(Array<CAnimationClipPtr>&& pAnimationClips, Array<CJointPtr>&& pSkeleton)
		: mAnimationClips(std::move(pAnimationClips)), 
		mSkeleton(std::move(pSkeleton))
	{
	}

	CSkinningData::CSkinningData()
		: mRootTransform(CMatrix4F::IdentityMatrix())
	{
	}

	CSkinningData::~CSkinningData()
	{
	}

	inline void CSkinningData::SetRootTransform(const CMatrix4F & pTransform)
	{
		mRootTransform = pTransform;
	}

	inline void CSkinningData::SetInvRootTransform(const CMatrix4F & pTransform)
	{
		mInvRootTransform = pTransform;
	}

	inline void CSkinningData::SetAnimationClips(Array<CAnimationClipPtr>&& pAnimationClips)
	{
		mAnimationClips = std::move(pAnimationClips);
	}

	inline void CSkinningData::SetSkeleton(Array<CJointPtr>&& pSkeleton)
	{
		mSkeleton = std::move(pSkeleton);
	}

	CAnimationPlayer::CAnimationPlayer(CSkinningDataPtr pSkinningData)
		: mSkinningData(std::move(pSkinningData)),
		mCurrentClip(nullptr),
		mfCurrentTime(0.0f),
		mfTime(0.0f),
		muiCurrentKeyFrame(0)
	{
	}

	CAnimationPlayer::~CAnimationPlayer()
	{
	}

	void CAnimationPlayer::StartClipNumber(uint32_t puiClip)
	{
		if (mSkinningData != nullptr)
		{
			if (puiClip < mSkinningData->GetAnimationClips().size())
				StartClip(mSkinningData->GetAnimationClips()[puiClip].get());
		}
	}

	void CAnimationPlayer::StartClip(CAnimationClip * pClip)
	{
		mCurrentClip = pClip;
		mfCurrentTime = 0.0f;
		muiCurrentKeyFrame = 0;

		mWorldTransforms.resize(mSkinningData->GetSkeleton().size());
		mAnimatedTransforms.resize(mSkinningData->GetSkeleton().size());
	}

	void CAnimationPlayer::Update(float pfTime)
	{
		if (mCurrentClip != nullptr)
		{
			IncrAnimationTime(pfTime);

			ApplyPoseToJoints(CalcCurrentAnimationPose());
			UpdateAnimatedTransforms();
		}
	}

	void CAnimationPlayer::IncrAnimationTime(float pfTime)
	{		
		mfCurrentTime += pfTime;
		mfCurrentTime = fmod(mfCurrentTime, mCurrentClip->GetDuration());
	}

	void CAnimationPlayer::ApplyPoseToJoints(const Array<CMatrix4F>& pCurrentPose)
	{
		const auto &lSkeleton = mSkinningData->GetSkeleton();
		for (auto &lJoint : lSkeleton)
		{
			const auto &lCurrentPose = lJoint->IsAnimated() ?
				pCurrentPose[lJoint->GetIndex()] : lJoint->GetLocalTransform();

			if (lJoint->GetParent() != -1)
			{
				mWorldTransforms[lJoint->GetIndex()] = mWorldTransforms[lJoint->GetParent()] *
					lCurrentPose;
			}
			else
			{
				mWorldTransforms[lJoint->GetIndex()] = lCurrentPose;
			}

			lJoint->SetAnimatedTransform(mSkinningData->GetInvRootTransform() * 
				mWorldTransforms[lJoint->GetIndex()] *
				lJoint->GetInvLocalTransform());
		}
	}

	Array<CKeyFrame*> CAnimationPlayer::GetPrevAndNextKeyFrames()
	{
		const auto &lKeyFrames = mCurrentClip->GetKeyFrames();

		uint32_t luiPrevFrameIdx = 0;
		uint32_t luiNextFrameIdx = 0;

		for (uint32_t lIdx = 0; lIdx < lKeyFrames.size(); lIdx++)
		{
			luiNextFrameIdx = lIdx;

			if (lKeyFrames[lIdx]->GetTime() > mfCurrentTime)
				break;

			luiPrevFrameIdx = lIdx;
		}

		if (luiPrevFrameIdx == luiNextFrameIdx)
			luiNextFrameIdx = (luiNextFrameIdx + 1) % lKeyFrames.size();

		CKeyFrame *lPrevFrame = lKeyFrames[luiPrevFrameIdx].get();
		CKeyFrame *lNextFrame = lKeyFrames[luiNextFrameIdx].get();

		return { lPrevFrame, lNextFrame };
	}

	Array<CMatrix4F> CAnimationPlayer::CalcCurrentAnimationPose()
	{
		Array<CKeyFrame*> lFrames = GetPrevAndNextKeyFrames();
		float pfProgression = CalcProgression(lFrames[0], lFrames[1]);

		return InterpolatePoses(lFrames[0], lFrames[1], pfProgression);
	}

	Array<CMatrix4F> CAnimationPlayer::InterpolatePoses(CKeyFrame * pPrevFrame, CKeyFrame * pNextFrame, float pfFactor)
	{
		Array<CMatrix4F> lCurrPose;

		const auto &lPrevJointTransforms = pPrevFrame->GetJointTransforms();
		const auto &lNextJointTransforms = pNextFrame->GetJointTransforms();

		for (uint32_t lIdx = 0; lIdx < lPrevJointTransforms.size(); lIdx++)
		{
			if (lPrevJointTransforms[lIdx] == nullptr || lNextJointTransforms[lIdx] == nullptr)
			{
				lCurrPose.push_back(mSkinningData->GetSkeleton()[lIdx]->GetLocalTransform());
			}
			else
			{
				CJointTransform lCurrTransform = CJointTransform::Interpolate(
					*lPrevJointTransforms[lIdx], *lNextJointTransforms[lIdx], pfFactor);

				lCurrPose.push_back(lCurrTransform.GetMatrix4());
			}
		}

		return lCurrPose;
	}

	float CAnimationPlayer::CalcProgression(CKeyFrame * pPrevFrame, CKeyFrame * pNextFrame)
	{
		float lfTotalTime = pNextFrame->GetTime() - pPrevFrame->GetTime();
		float lfCurrTime = mfCurrentTime - pPrevFrame->GetTime();

		return lfCurrTime / lfTotalTime;
	}

	void CAnimationPlayer::UpdateAnimatedTransforms()
	{
		const auto &lSkeleton = mSkinningData->GetSkeleton();
		for (auto &lJoint : lSkeleton)
		{
			mAnimatedTransforms[lJoint->GetIndex()] = lJoint->GetAnimatedTransform();
		}
	}
}