#pragma once

#include "GUI/Control.h"
#include "GUI/ControlSkin.h"

namespace FDX
{
	enum class ECursorType : uint32_t
	{
		Arrow,
		Cross,
		Hand,
		Wait
	};

	class FDX_EXPORT CCursor : public CControl
	{
	public:

		CCursor(const String &pstrName);
		virtual ~CCursor();

		virtual void SetCursorType(ECursorType pCursorType);

	protected:
		
		virtual bool OnMouseMove(CMouseMoveControlEvent *pEvent) override;
	};

	using CCursorPtr = SmartPtr<CCursor>;
}