
#include "GUI/Menu.h"
#include "GUI/UIManager.h"

namespace FDX
{
	CMenuItem::CMenuItem(const String &pstrName)
		: CButton(pstrName), 
		mbIsMenuOpen(false)
	{
	}

	CMenuItem::~CMenuItem()
	{
	}

	void CMenuItem::UpdatePosition(uint32_t puiX, uint32_t puiY)
	{
		uint32_t luiBorderWidth = 0;

		if (mSkin != nullptr)
		{
			CControlMaterial *lMaterial = mSkin->GetActiveMaterial();
			if (lMaterial != nullptr)
			{
				auto lBorder = lMaterial->GetBorder();
				if (lBorder != nullptr)
					luiBorderWidth = lBorder->GetBorderWidth();
			}
		}

		SetPosition(puiX, puiY);

		uint32_t luiY = 0;
		for (const auto &lMenuItem : mMenuItems)
		{
			lMenuItem->UpdatePosition(muiWidth - luiBorderWidth, luiY);
			luiY += muiHeight;
		}
	}

	void CMenuItem::AddMenuItem(CMenuItem *pMenuItem)
	{
		mMenuItems.push_back(pMenuItem);
	}

	void CMenuItem::RemoveMenuItem(CMenuItem *pMenuItem)
	{
		auto lIt = std::find(mMenuItems.begin(), 
			mMenuItems.end(), pMenuItem);

		if (lIt != mMenuItems.end())
			mMenuItems.erase(lIt);
	}

	void CMenuItem::Close()
	{
		SetVisible(false);

		for (const auto &lMenuItem : mMenuItems)
			lMenuItem->Close();

		mbIsMenuOpen = false;
	}

	void CMenuItem::Open()
	{
		SetVisible(true);
	}

	void CMenuItem::PreDraw(CCanvas * pCanvas)
	{
		CButton::PreDraw(pCanvas);
	}

	void CMenuItem::PostDraw(CCanvas * pCanvas)
	{
		if (mbIsMenuOpen && mMenuItems.size() > 0)
		{
			if (mSkin != nullptr)
			{
				CControlMaterial *lMaterial = mSkin->GetActiveMaterial();
				if (lMaterial != nullptr)
				{
					auto lBorder = lMaterial->GetBorder();
					if (lBorder != nullptr)
					{
						float lfX = (float)(miScreenX + muiWidth - lBorder->GetBorderWidth());
						float lfY = (float)miScreenY;
						float lfWidth = (float)mMenuItems[0]->GetWidth();
						float lfHeight = (float)mMenuItems[0]->GetHeight();

						for (uint32_t lIdx = 1; lIdx < mMenuItems.size(); lIdx++)
							lfHeight += (float)mMenuItems[lIdx]->GetHeight();

						pCanvas->DrawBorder(CVector2F(lfX, lfY), CVector2F(lfWidth, lfHeight),
							lBorder->GetBorderType(), (float)lBorder->GetBorderWidth(),
							lBorder->GetBorderColors());
					}
				}
			}
		}
	}

	bool CMenuItem::OnMouseEnter(CMouseEnterControlEvent * pEvent)
	{
		CButton::OnMouseEnter(pEvent);

		for (const auto &lMenuItem : mMenuItems)
			lMenuItem->Open();

		mbIsMenuOpen = true;

		return true;
	}

	bool CMenuItem::OnMouseLeave(CMouseLeaveControlEvent * pEvent)
	{
		CButton::OnMouseLeave(pEvent);

		bool lbIsChildVisible = false;
		for (const auto &lMenuItem : mMenuItems)
		{
			if (lMenuItem->IsMouseOver())
			{
				lbIsChildVisible = true;
				break;
			}
		}

		if (!lbIsChildVisible)
		{
			for (const auto &lMenuItem : mMenuItems)
				lMenuItem->Close();

			mbIsMenuOpen = false;
		}

		return true;
	}

	bool CMenuItem::OnMouseDown(CMouseDownControlEvent * pEvent)
	{
		CButton::OnMouseDown(pEvent);
		return true;
	}

	bool CMenuItem::OnMouseUp(CMouseUpControlEvent * pEvent)
	{
		CButton::OnMouseUp(pEvent);

		for (const auto &lMenuItem : mMenuItems)
			lMenuItem->Close();

		mbIsMenuOpen = false;

		
		
		return false;
	}

	bool CMenuItem::OnLostFocus(CLostFocusControlEvent * pEvent)
	{
		CButton::OnLostFocus(pEvent);

		for (const auto &lMenuItem : mMenuItems)
			lMenuItem->Close();

		mbIsMenuOpen = false;

		return true;
	}

	CMenu::CMenu(const String &pstrName)
		: CMenuItem(pstrName)
	{
	}

	CMenu::~CMenu()
	{
	}

	void CMenu::LayoutChildControls()
	{
		uint32_t luiBorderWidth = 0;

		if (mSkin != nullptr)
		{
			CControlMaterial *lMaterial = mSkin->GetActiveMaterial();
			if (lMaterial != nullptr)
			{
				auto lBorder = lMaterial->GetBorder();
				if (lBorder != nullptr)
					luiBorderWidth = lBorder->GetBorderWidth();
			}
		}

		uint32_t luiPosY = muiHeight - luiBorderWidth;

		for (const auto &lMenuItem : mMenuItems)
		{
			lMenuItem->UpdatePosition(0, luiPosY);
			luiPosY += lMenuItem->GetHeight();
		}
	}

	void CMenu::PreDraw(CCanvas * pCanvas)
	{
		CMenuItem::PreDraw(pCanvas);
	}

	void CMenu::PostDraw(CCanvas * pCanvas)
	{
		CMenuItem::PostDraw(pCanvas);
	}

	bool CMenu::OnMouseEnter(CMouseEnterControlEvent * pEvent)
	{
		CButton::OnMouseEnter(pEvent);
		return true;
	}

	bool CMenu::OnMouseLeave(CMouseLeaveControlEvent * pEvent)
	{
		CButton::OnMouseLeave(pEvent);
		return true;
	}

	bool CMenu::OnMouseUp(CMouseUpControlEvent * pEvent)
	{
		if (pEvent->GetButton() == EMouseButton::Left)
		{
			for (const auto &lMenuItem : mMenuItems)
			{
				if (mbIsMenuOpen)
					lMenuItem->Close();
				else
					lMenuItem->Open();
			}

			mbIsMenuOpen = !mbIsMenuOpen;
		}

		return true;
	}
}