#pragma once

#include "Math/Matrix4F.h"
#include "Math/DualQuaternion.h"

namespace FDX
{
	class CJoint;
	class CJointTransform;
	class CKeyFrame;
	class CAnimationClip;
	class CSkinningData;
	class CAnimationPlayer;

	using CJointPtr = SmartPtr<CJoint>;
	using CJointTransformPtr = SmartPtr<CJointTransform>;
	using CKeyFramePtr = SmartPtr<CKeyFrame>;
	using CAnimationClipPtr = SmartPtr<CAnimationClip>;
	using CSkinningDataPtr = SmartPtr<CSkinningData>;
	using CAnimationPlayerPtr = SmartPtr<CAnimationPlayer>;

	class FDX_EXPORT CJoint
	{
	public:

		CJoint(uint32_t puiIndex, int32_t piParent, 
			const CMatrix4F &pLocalTransform);
		
		CJoint();
		~CJoint();

		uint32_t GetIndex() const
		{
			return muiIndex;
		}

		int32_t GetParent() const
		{
			return miParent;
		}

		bool IsAnimated() const
		{
			return mbAnimated;
		}

		const CMatrix4F& GetLocalTransform() const
		{
			return mLocalTransform;
		}

		const CMatrix4F& GetInvLocalTransform() const
		{
			return mInvLocalTransform;
		}

		const CMatrix4F& GetAnimatedTransform() const
		{
			return mAnimatedTransform;
		}

		inline void SetIndex(uint32_t puiIndex);
		inline void SetParent(int32_t piParent);
		inline void SetAnimated(bool pbAnimated);
		inline void SetLocalTransform(const CMatrix4F &pLocalTransform);
		inline void SetInvLocalTransform(const CMatrix4F &pInvLocalTransform);
		inline void SetAnimatedTransform(const CMatrix4F &pAnimatedTransform);

	private:

		uint32_t muiIndex;
		int32_t miParent;
		bool mbAnimated;
		CMatrix4F mLocalTransform;
		CMatrix4F mInvLocalTransform;
		CMatrix4F mAnimatedTransform;
	};

	class FDX_EXPORT CJointTransform
	{
	public:

		CJointTransform(const CVector3F &pPosition,
			const CQuaternion &pRotation);

		CJointTransform();
		~CJointTransform();

		const CVector3F& GetPosition() const
		{
			return mPosition;
		}

		const CVector3F& GetScale() const
		{
			return mScale;
		}

		const CQuaternion& GetOrientation() const
		{
			return mOrientation;
		}

		inline CMatrix4F GetMatrix4() const;		

		inline void SetPosition(const CVector3F &pPosition);
		inline void SetScale(const CVector3F &pScale);
		inline void SetOrientation(const CQuaternion &pRotation);

	public:

		static CJointTransform Interpolate(
			const CJointTransform &pJointA,
			const CJointTransform &pJointB,
			float pfFactor);

	private:

		CVector3F mPosition;
		CVector3F mScale;
		CQuaternion mOrientation;
	};

	class FDX_EXPORT CKeyFrame
	{
	public:

		CKeyFrame(float pfTime, Array<CJointTransformPtr> &&pJointTransforms);

		CKeyFrame();
		~CKeyFrame();

		CKeyFrame(const CKeyFrame&) = delete;
		CKeyFrame& operator=(const CKeyFrame&) = delete;

		float GetTime() const
		{
			return mfTime;
		}

		const Array<CJointTransformPtr>& GetJointTransforms() const
		{
			return mJointTransforms;
		}

		inline void SetTime(float pfTime);
		inline void SetJointTransforms(Array<CJointTransformPtr> &&pJointTransforms);

	private:

		float mfTime;
		Array<CJointTransformPtr> mJointTransforms;
	};

	class FDX_EXPORT CAnimationClip
	{
	public:

		CAnimationClip(const String &pstrName, float pfDuration,
			Array<CKeyFramePtr> &&pKeyFrames);

		CAnimationClip();
		~CAnimationClip();

		CAnimationClip(const CAnimationClip&) = delete;
		CAnimationClip& operator=(const CAnimationClip&) = delete;

		const String& GetName() const
		{
			return mstrName;
		}

		float GetDuration() const
		{
			return mfDuration;
		}

		const Array<CKeyFramePtr>& GetKeyFrames() const
		{
			return mKeyFrames;
		}

		inline void SetName(const String &pstrName);
		inline void SetDuration(float pfDuration);
		inline void SetKeyFrames(Array<CKeyFramePtr> &&pKeyFrames);

	private:

		String mstrName;
		float mfDuration;
		Array<CKeyFramePtr> mKeyFrames;
	};

	class FDX_EXPORT CSkinningData
	{
	public:

		CSkinningData(Array<CAnimationClipPtr> &&pAnimationClips,
			Array<CJointPtr> &&pSkeleton);

		CSkinningData();
		~CSkinningData();

		CSkinningData(const CSkinningData&) = delete;
		CSkinningData& operator=(const CSkinningData&) = delete;

		const CMatrix4F& GetRootTransform() const
		{
			return mRootTransform;
		}
		
		const CMatrix4F& GetInvRootTransform() const
		{
			return mInvRootTransform;
		}

		const Array<CAnimationClipPtr>& GetAnimationClips() const
		{
			return mAnimationClips;
		}

		const Array<CJointPtr>& GetSkeleton() const
		{
			return mSkeleton;
		}

		inline void SetRootTransform(const CMatrix4F &pTransform);
		inline void SetInvRootTransform(const CMatrix4F &pTransform);
		inline void SetAnimationClips(Array<CAnimationClipPtr> &&pAnimationClips);
		inline void SetSkeleton(Array<CJointPtr> &&pSkeleton);

	private:
		
		CMatrix4F mRootTransform;
		CMatrix4F mInvRootTransform;
		Array<CAnimationClipPtr> mAnimationClips;
		Array<CJointPtr> mSkeleton;
	};

	class FDX_EXPORT CAnimationPlayer
	{
	public:

		CAnimationPlayer(CSkinningDataPtr pSkinningData);
		~CAnimationPlayer();

		CAnimationPlayer(const CAnimationPlayer&) = delete;
		CAnimationPlayer& operator=(const CAnimationPlayer&) = delete;

		CSkinningData* GetSkinningData() const
		{
			return mSkinningData.get();
		}

		CAnimationClip* GetCurrentClip() const
		{
			return mCurrentClip;
		}

		float GetCurrentTime() const
		{
			return mfCurrentTime;
		}

		uint32_t GetCurrentKeyFrame() const
		{
			return muiCurrentKeyFrame;
		}

		const Array<CMatrix4F>& GetAnimatedTransforms() const
		{
			return mAnimatedTransforms;
		}

		void StartClipNumber(uint32_t puiClip);
		void StartClip(CAnimationClip *pClip);

		void Update(float pfTime);

	private:

		void IncrAnimationTime(float pfTime);
		void ApplyPoseToJoints(const Array<CMatrix4F> &pCurrentPose);

		Array<CKeyFrame*> GetPrevAndNextKeyFrames();
		Array<CMatrix4F> CalcCurrentAnimationPose();
		Array<CMatrix4F> InterpolatePoses(CKeyFrame *pPrevFrame,
			CKeyFrame *pNextFrame, float pfFactor);

		float CalcProgression(CKeyFrame *pPrevFrame, CKeyFrame *pNextFrame);		
		void UpdateAnimatedTransforms();

	private:

		CSkinningDataPtr mSkinningData;
		CAnimationClip *mCurrentClip;
		float mfCurrentTime;
		float mfTime;
		uint32_t muiCurrentKeyFrame;
		Array<CMatrix4F> mAnimatedTransforms;
		Array<CMatrix4F> mWorldTransforms;
	};
}