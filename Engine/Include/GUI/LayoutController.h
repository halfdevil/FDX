#pragma once

#include "GUI/Control.h"
#include "Core/Singleton.h"

namespace FDX
{
	enum class EControlLayoutType : uint32_t
	{ 
		Default = 0,
		Vertical,
		Horizontal
	};

	class FDX_EXPORT CVerticalLayout : public CControl
	{
	public:

		CVerticalLayout(const String &pstrName);
		~CVerticalLayout();

		EControlLayoutType GetLayoutType() const
		{
			return EControlLayoutType::Vertical;
		}

		void LayoutChildControls() override;
	};

	class FDX_EXPORT CHorizontalLayout : public CControl
	{
	public:

		CHorizontalLayout(const String &pstrName);
		~CHorizontalLayout();

		EControlLayoutType GetLayoutType() const
		{
			return EControlLayoutType::Horizontal;
		}

		void LayoutChildControls() override;
	};

	class FDX_EXPORT CLayoutFactory : public CSingleton<CLayoutFactory>
	{
	public:

		CControlPtr CreateLayout(EControlLayoutType pLayoutType,
			const String &pstrName);
	};
}