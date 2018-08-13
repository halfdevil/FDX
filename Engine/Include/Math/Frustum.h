#pragma once

#include "Math/Plane.h"
#include "Math/AxisAlignedBox.h"

namespace FDX
{
	class FDX_EXPORT CFrustum
	{
	public:

		inline CFrustum();
		inline CFrustum(const CMatrix4F &pViewProjMat);
		inline CFrustum(const CMatrix4F &pView, const CMatrix4F &pProj);

		Array<CPlane> GetPlanes() const
		{
			return Array<CPlane>(mPlanes, mPlanes + 6);
		}

		const CPlane& GetNear() const
		{
			return mPlanes[0];
		}

		const CPlane& GetFar() const
		{
			return mPlanes[1];
		}

		const CPlane& GetLeft() const
		{
			return mPlanes[2];
		}

		const CPlane& GetRight() const
		{
			return mPlanes[3];
		}

		const CPlane& GetTop() const
		{
			return mPlanes[4];
		}

		const CPlane& GetBottom() const
		{
			return mPlanes[5];
		}

		void Set(const CMatrix4F &pControlProjMat);
		void Set(const CMatrix4F &pControl, const CMatrix4F &pProj);

		EContainmentType Contains(const CAxisAlignedBox &pAabb) const;
		bool IsVisible(const CAxisAlignedBox &pAabb) const;

	private:

		mutable CPlane mPlanes[6];
	};

	inline CFrustum::CFrustum()
	{
	}

	inline CFrustum::CFrustum(const CMatrix4F & pControlProjMat)
	{
		Set(pControlProjMat);
	}

	inline CFrustum::CFrustum(const CMatrix4F & pControl, const CMatrix4F & pProj)
	{
		Set(pControl, pProj);
	}
}