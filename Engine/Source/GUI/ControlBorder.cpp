
#include "GUI/ControlBorder.h"

namespace FDX
{
	CControlBorder::CControlBorder()
		: muiBorderType(EBorderType::NoBorder),
		muiBorderWidth(1)
	{
	}

	CControlBorder::~CControlBorder()
	{
	}

	void CControlBorder::SetBorderType(uint32_t puiBorderType)
	{
		muiBorderType = puiBorderType;
	}

	void CControlBorder::SetBorderWidth(uint32_t puiBorderWidth)
	{
		muiBorderWidth = puiBorderWidth;
	}

	void CControlBorder::SetBorderColor(EBorderType pBorderType, const CColor4F & pColor)
	{
		mBorderColors[pBorderType] = pColor;
	}
}