
#include "Scene/Renderable.h"

namespace FDX
{
	CRenderable::CRenderable()
		: mbRendered(false),
		muiRenderCount(0),
		mMaterial(nullptr),
		mDrawOperation(nullptr),
		mRenderableType(ERenderableType::Normal)
	{
	}

	CRenderable::~CRenderable()
	{
	}

	void CRenderable::SetRendered(bool pbRendered)
	{
		mbRendered = pbRendered;
	}

	void CRenderable::SetRenderCount(uint32_t puiCount)
	{
		muiRenderCount = puiCount;
	}

	bool CRenderable::CollidesWithAabb(const CAxisAlignedBox & pAabb)
	{
		return mAxisAlignedBox.Contains(pAabb) != EContainmentType::Culled;
	}

	bool CRenderable::CollidesWithFrustum(const CFrustum & pFrustum)
	{
		return pFrustum.Contains(mAxisAlignedBox) != EContainmentType::Culled;
	}

	void CRenderable::UpdateGraphics(CCamera * pCamera, IProgram * pProgram)
	{
	}
}