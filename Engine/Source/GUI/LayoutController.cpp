
#include "GUI/LayoutController.h"

namespace FDX
{
	CVerticalLayout::CVerticalLayout(const String &pstrName)
		: CControl(pstrName)
	{
	}

	CVerticalLayout::~CVerticalLayout()
	{
	}

	void CVerticalLayout::LayoutChildControls()
	{
		uint32_t luiWidth = GetWidth();
		uint32_t luiHeight = GetHeight();

		uint32_t luiNumFilledControls = 0;

		for (const auto &lControl : mChildControls)
		{
			uint32_t luiMode = lControl->GetFillMode();

			if ((luiMode & EControlFillMode::Vertical) == 0)
				luiHeight -= lControl->GetHeight();
			else
				luiNumFilledControls++;
		}

		uint32_t luiNextPosY = 0;
		for (const auto &lControl : mChildControls)
		{
			uint32_t luiMode = lControl->GetFillMode();

			if (luiMode & EControlFillMode::Vertical)
				lControl->SetHeight(luiHeight / luiNumFilledControls);

			if (luiMode & EControlFillMode::Horizontal)
				lControl->SetWidth(luiWidth - lControl->GetX());

			lControl->SetPosition(lControl->GetX(), luiNextPosY);
			luiNextPosY += lControl->GetHeight();
		}

		for (const auto &lControl : mChildControls)
			lControl->LayoutChildControls();
	}

	CHorizontalLayout::CHorizontalLayout(const String &pstrName)
		: CControl(pstrName)
	{
	}

	CHorizontalLayout::~CHorizontalLayout()
	{
	}

	void CHorizontalLayout::LayoutChildControls()
	{
		uint32_t luiWidth = GetWidth();
		uint32_t luiHeight = GetHeight();

		uint32_t luiNumFilledControls = 0;

		for (const auto &lControl : mChildControls)
		{
			uint32_t luiMode = lControl->GetFillMode();

			if ((luiMode & EControlFillMode::Horizontal) == 0)
				luiWidth -= lControl->GetWidth();
			else
				luiNumFilledControls++;
		}

		uint32_t luiNextPosX = 0;
		for (const auto &lControl : mChildControls)
		{
			uint32_t luiMode = lControl->GetFillMode();

			if (luiMode & EControlFillMode::Vertical)
				lControl->SetHeight(luiHeight - lControl->GetY());

			if (luiMode & EControlFillMode::Horizontal)
				lControl->SetWidth(luiWidth / luiNumFilledControls);

			lControl->SetPosition(luiNextPosX, lControl->GetY());
			luiNextPosX += lControl->GetWidth();
		}

		for (const auto &lControl : mChildControls)
			lControl->LayoutChildControls();
	}

	CControlPtr CLayoutFactory::CreateLayout(EControlLayoutType pLayoutType,
		const String &pstrName)
	{
		switch (pLayoutType)
		{
		case EControlLayoutType::Default:
			return std::make_unique<CControl>(pstrName);

		case EControlLayoutType::Vertical:
			return std::make_unique<CVerticalLayout>(pstrName);

		case EControlLayoutType::Horizontal:
			return std::make_unique<CHorizontalLayout>(pstrName);
		}

		return nullptr;
	}
}