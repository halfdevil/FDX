
#include "Math/Frustum.h"

namespace FDX
{
	void CFrustum::Set(const CMatrix4F & pViewProjMat)
	{
		mPlanes[0].Normal.X = (pViewProjMat.M2[0][3] + pViewProjMat.M2[0][2]);
		mPlanes[0].Normal.Y = (pViewProjMat.M2[1][3] + pViewProjMat.M2[1][2]);
		mPlanes[0].Normal.Z = (pViewProjMat.M2[2][3] + pViewProjMat.M2[2][2]);
		mPlanes[0].D = (pViewProjMat.M2[3][3] + pViewProjMat.M2[3][2]);

		mPlanes[1].Normal.X = (pViewProjMat.M2[0][3] - pViewProjMat.M2[0][2]);
		mPlanes[1].Normal.Y = (pViewProjMat.M2[1][3] - pViewProjMat.M2[1][2]);
		mPlanes[1].Normal.Z = (pViewProjMat.M2[2][3] - pViewProjMat.M2[2][2]);
		mPlanes[1].D = (pViewProjMat.M2[3][3] - pViewProjMat.M2[3][2]);

		mPlanes[2].Normal.X = (pViewProjMat.M2[0][3] + pViewProjMat.M2[0][0]);
		mPlanes[2].Normal.Y = (pViewProjMat.M2[1][3] + pViewProjMat.M2[1][0]);
		mPlanes[2].Normal.Z = (pViewProjMat.M2[2][3] + pViewProjMat.M2[2][1]);
		mPlanes[2].D = (pViewProjMat.M2[3][3] + pViewProjMat.M2[3][0]);

		mPlanes[3].Normal.X = (pViewProjMat.M2[0][3] - pViewProjMat.M2[0][0]);
		mPlanes[3].Normal.Y = (pViewProjMat.M2[1][3] - pViewProjMat.M2[1][0]);
		mPlanes[3].Normal.Z = (pViewProjMat.M2[2][3] - pViewProjMat.M2[2][0]);
		mPlanes[3].D = (pViewProjMat.M2[3][3] - pViewProjMat.M2[3][0]);

		mPlanes[4].Normal.X = (pViewProjMat.M2[0][3] - pViewProjMat.M2[0][1]);
		mPlanes[4].Normal.Y = (pViewProjMat.M2[1][3] - pViewProjMat.M2[1][1]);
		mPlanes[4].Normal.Z = (pViewProjMat.M2[2][3] - pViewProjMat.M2[2][1]);
		mPlanes[4].D = (pViewProjMat.M2[3][3] - pViewProjMat.M2[3][1]);

		mPlanes[5].Normal.X = (pViewProjMat.M2[0][3] + pViewProjMat.M2[0][1]);
		mPlanes[5].Normal.Y = (pViewProjMat.M2[1][3] + pViewProjMat.M2[1][1]);
		mPlanes[5].Normal.Z = (pViewProjMat.M2[2][3] + pViewProjMat.M2[2][1]);
		mPlanes[5].D = (pViewProjMat.M2[3][3] + pViewProjMat.M2[3][1]);

		for (int i = 0; i < 6; i++)
			mPlanes[i].Normalize();
	}

	void CFrustum::Set(const CMatrix4F & pView, const CMatrix4F & pProj)
	{
		Set(pProj * pView);
	}

	EContainmentType CFrustum::Contains(const CAxisAlignedBox & pAabb) const
	{
		CVector3F lMin, lMax;
		bool lbIntersects = false;

		for (uint32_t lIdx = 0; lIdx < 6; lIdx++)
		{
			if (mPlanes[lIdx].Normal.X > 0.0f)
			{
				lMin.X = pAabb.Min.X;
				lMax.X = pAabb.Max.X;
			}
			else
			{
				lMin.X = pAabb.Max.X;
				lMax.X = pAabb.Min.X;
			}

			if (mPlanes[lIdx].Normal.Y > 0.0f)
			{
				lMin.Y = pAabb.Min.Y;
				lMax.Y = pAabb.Max.Y;
			}
			else
			{
				lMin.Y = pAabb.Max.Y;
				lMax.Y = pAabb.Min.Y;
			}

			if (mPlanes[lIdx].Normal.Z > 0.0f)
			{
				lMin.Z = pAabb.Min.Z;
				lMax.Z = pAabb.Max.Z;
			}
			else
			{
				lMin.Z = pAabb.Max.Z;
				lMax.Z = pAabb.Min.Z;
			}

			if (mPlanes[lIdx].Dot(lMin) > 0.0f)
				return EContainmentType::Culled;

			if (mPlanes[lIdx].Dot(lMax) >= 0.0f)
				lbIntersects = true;
		}

		if (lbIntersects)
			return EContainmentType::Clipped;

		return EContainmentType::Visible;
	}

	bool CFrustum::IsVisible(const CAxisAlignedBox & pAabb) const
	{
		return Contains(pAabb) != EContainmentType::Culled;
	}
}