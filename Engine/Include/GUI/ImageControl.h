#pragma once

#include "GUI/Control.h"

namespace FDX
{
	class FDX_EXPORT CImageControl : public CControl
	{
	public:

		CImageControl(const String &pstrName);
		virtual ~CImageControl();

		virtual void SetTexture(CTexture2D *pTexture);

	protected:

		CTexture2D * mTexture;
	};
}