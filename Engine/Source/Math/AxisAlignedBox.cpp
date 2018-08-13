
#include "Math/AxisAlignedBox.h"
#include "Math/MathUtil.h"

namespace FDX
{
	CAxisAlignedBox CAxisAlignedBox::CreateFromPoints(const Array<CVector3F>& pPoints)
	{
		CAxisAlignedBox lAabb(99999.9f, 99999.9f, 99999.9f,
			-99999.9f, -99999.9f, -99999.9f);

		for (uint32_t lIdx = 0; lIdx < pPoints.size(); lIdx++)
		{
			if (pPoints[lIdx].X < lAabb.Min.X) lAabb.Min.X = pPoints[lIdx].X;
			if (pPoints[lIdx].Y < lAabb.Min.Y) lAabb.Min.Y = pPoints[lIdx].Y;
			if (pPoints[lIdx].Z < lAabb.Min.Z) lAabb.Min.Z = pPoints[lIdx].Z;

			if (pPoints[lIdx].X > lAabb.Max.X) lAabb.Max.X = pPoints[lIdx].X;
			if (pPoints[lIdx].Y > lAabb.Max.Y) lAabb.Max.Y = pPoints[lIdx].Y;
			if (pPoints[lIdx].Z > lAabb.Max.Z) lAabb.Max.Z = pPoints[lIdx].Z;
		}

		return lAabb;
	}
}