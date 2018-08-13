
#include "GUI/Control.h"

namespace FDX
{
	CControl::CControl(const String &pstrName)
		: CControlEventHandler(this),
		mstrName(pstrName),
		mbIsEnabled(true),
		mbIsVisible(true),
		mParent(nullptr),
		miX(0),
		miY(0),
		miScreenX(0),
		miScreenY(0),
		muiWidth(0),
		muiHeight(0),
		miDepth(0),
		mfParentFractionX(1.0f),
		mfParentFractionY(1.0f),
		mAlignV(EAlignVertical::DontCare),
		mAlignH(EAlignHorizontal::DontCare),
		muiFillMode(EControlFillMode::DontCare),
		mSkin(nullptr)
	{
	}

	CControl::~CControl()
	{
	}

	EResult CControl::Create(CControlContainer * pContainer)
	{
		EResult lResult = EResult::Success;
		return lResult;
	}

	bool CControl::IsEnabled() const
	{
		if (mParent != nullptr)
		{
			if (!mParent->IsEnabled())
				return false;
		}

		return mbIsEnabled;
	}

	bool CControl::IsVisble() const
	{
		if (mParent != nullptr)
		{
			if (!mParent->IsVisble())
				return false;
		}

		return mbIsVisible;
	}

	bool CControl::IsPointOver(int32_t puiX, int32_t puiY) const
	{
		return (puiX >= miScreenX && puiX <= miScreenX + (int32_t)muiWidth) &&
			(puiY >= miScreenY && puiY <= miScreenY + (int32_t)muiHeight);
	}

	void CControl::SetName(const String & pstrName)
	{
		mstrName = pstrName;
	}

	void CControl::SetX(int32_t puiX)
	{
		miX = puiX;
	}

	void CControl::SetY(int32_t puiY)
	{
		miY = puiY;
	}

	void CControl::SetPosition(int32_t puiX, int32_t puiY)
	{
		miX = puiX;
		miY = puiY;

		if (mParent != nullptr)
			UpdateScreenPosition(mParent->GetScreenX(), mParent->GetScreenY());
		else
			UpdateScreenPosition(0, 0);
	}

	void CControl::SetSize(uint32_t puiWidth, uint32_t puiHeight)
	{
		muiWidth = puiWidth;
		muiHeight = puiHeight;
	}

	void CControl::SetDepth(int32_t piDepth)
	{
		miDepth = piDepth;
	}

	void CControl::SetWidth(uint32_t puiWidth)
	{
		muiWidth = puiWidth;
	}

	void CControl::SetHeight(uint32_t puiHeight)
	{
		muiHeight = puiHeight;
	}

	void CControl::SetParentFractionX(float pfFractionX)
	{
		mfParentFractionX = pfFractionX;
	}

	void CControl::SetParentFractionY(float pfFractionY)
	{
		mfParentFractionY = pfFractionY;
	}

	void CControl::SetAlignV(EAlignVertical pAlignV)
	{
		mAlignV = pAlignV;
	}

	void CControl::SetAlignH(EAlignHorizontal pAlignH)
	{
		mAlignH = pAlignH;
	}

	void CControl::SetFillMode(uint32_t pFillMode)
	{
		muiFillMode = pFillMode;
	}

	void CControl::SetEnabled(bool pbEnabled)
	{
		mbIsEnabled = pbEnabled;
	}

	void CControl::SetVisible(bool pbVisible)
	{
		mbIsVisible = pbVisible;
	}

	void CControl::SetSkin(CControlSkinPtr pSkin)
	{
		mSkin = std::move(pSkin);
	}

	void CControl::AddChild(CControl *pControl)
	{
		if (pControl)
		{
			if (pControl->mParent != nullptr)
				pControl->mParent->RemoveChild(pControl);

			pControl->mParent = this;
			pControl->miDepth = mChildControls.size();
			mChildControls.push_back(pControl);
		}
	}

	void CControl::RemoveChild(CControl * pControl)
	{
		if (pControl)
		{
			if (pControl->mParent != this)
				return;

			auto lIt = std::find_if(
				mChildControls.begin(),
				mChildControls.end(),
				[pControl](const CControl *pV) {
					return pV == pControl;
				}
			);

			if (lIt != mChildControls.end())
				mChildControls.erase(lIt);
		}
	}

	void CControl::Update(float pfDelta)
	{
		if (mSkin != nullptr)
			mSkin->Update(pfDelta);

		for (auto &lChildControl : mChildControls)
			lChildControl->Update(pfDelta);
	}

	void CControl::Draw(CCanvas * pCanvas)
	{
		if (!IsVisble())
			return;

		PreDraw(pCanvas);

		auto lIt = mChildControls.rbegin();
		while (lIt != mChildControls.rend())
		{
			(*lIt)->Draw(pCanvas);
			lIt++;
		}

		PostDraw(pCanvas);
	}

	void CControl::UpdateScreenPosition(uint32_t puiParentX, uint32_t puiParentY)
	{
		miScreenX = puiParentX + miX;
		miScreenY = puiParentY + miY;

		for (const auto &lChildControl : mChildControls)
			lChildControl->UpdateScreenPosition(miScreenX, miScreenY);
	}

	void CControl::LayoutChildControls()
	{
		uint32_t luiWidth = muiWidth;
		uint32_t luiHeight = muiHeight;
		
		for (const auto &lChildControl : mChildControls)
		{
			if (lChildControl->GetFillMode() & EControlFillMode::Vertical)
				lChildControl->SetHeight(muiHeight - lChildControl->GetY());

			if (lChildControl->GetFillMode() & EControlFillMode::Horizontal)
				lChildControl->SetWidth(muiWidth - lChildControl->GetX());

			if (lChildControl->GetParentFractionX() < 1.0f)
				lChildControl->SetWidth(static_cast<uint32_t>(lChildControl->GetParentFractionX() * muiWidth));

			if (lChildControl->GetParentFractionY() < 1.0f)
				lChildControl->SetHeight(static_cast<uint32_t>(lChildControl->GetParentFractionY() * muiHeight));

			switch (lChildControl->GetAlignV())
			{
			case EAlignVertical::Top:
				lChildControl->SetPosition(lChildControl->GetX(), 0);
				break;

			case EAlignVertical::Bottom:
				lChildControl->SetPosition(lChildControl->GetX(), muiHeight - lChildControl->GetHeight());
				break;

			case EAlignVertical::Center:
				lChildControl->SetPosition(lChildControl->GetX(), (muiHeight - lChildControl->GetHeight()) / 2);
				break;

			case EAlignVertical::DontCare:
				break;
			}

			switch (lChildControl->GetAlignH())
			{
			case EAlignHorizontal::Left:
				lChildControl->SetPosition(0, lChildControl->GetY());
				break;

			case EAlignHorizontal::Right:
				lChildControl->SetPosition(muiWidth - lChildControl->GetWidth(), lChildControl->GetY());
				break;

			case EAlignHorizontal::Center:
				lChildControl->SetPosition((muiWidth - lChildControl->GetWidth()) / 2, lChildControl->GetY());
				break;

			case EAlignHorizontal::DontCare:
				break;
			}

			lChildControl->LayoutChildControls();
		}

		SortChildren();
	}

	void CControl::BringToFront()
	{
		if (mParent != nullptr)
		{
			int32_t liMaxDepth = GetDepth();

			for (const auto &lChildControl : mParent->mChildControls)
			{
				if (lChildControl != this)
				{
					if (lChildControl->GetDepth() > liMaxDepth)
						liMaxDepth = lChildControl->GetDepth();

					if (lChildControl->GetDepth() > miDepth)
						lChildControl->SetDepth(lChildControl->GetDepth() - 1);
				}
			}

			miDepth = liMaxDepth;
			mParent->SortChildren();
		}			
	}

	void CControl::SendToBack()
	{
		if (mParent != nullptr)
		{
			int32_t liMinDepth = GetDepth();

			for (const auto &lChildControl : mParent->mChildControls)
			{
				if (lChildControl != this)
				{
					if (lChildControl->GetDepth() < liMinDepth)
						liMinDepth = lChildControl->GetDepth();

					if (lChildControl->GetDepth() < miDepth)
						lChildControl->SetDepth(lChildControl->GetDepth() + 1);
				}
			}

			miDepth = liMinDepth;
			mParent->SortChildren();
		}
	}

	void CControl::SortChildren()
	{
		std::sort(mChildControls.begin(), mChildControls.end(),
			[](const CControl *pA, const CControl *pB) -> bool {
				if (pA->GetDepth() > pB->GetDepth())
					return true;
				else
					return false;
			}
		);
	}

	void CControl::PreDraw(CCanvas * pCanvas)
	{
		if (mSkin != nullptr)
		{
			CControlMaterial *lMaterial = nullptr;

			if (!mbIsEnabled)
				lMaterial = mSkin->GetMaterial(EControlMaterialType::Disabled);
			else
				lMaterial = mSkin->GetActiveMaterial();

			if (lMaterial != nullptr)
			{
				float lfX = (float)miScreenX;
				float lfY = (float)miScreenY;
				float lfWidth = (float)muiWidth;
				float lfHeight = (float)muiHeight;

				CTexture2D *lTexture = lMaterial->GetCurrentTexture();
				if (lTexture != nullptr)
				{
					pCanvas->Draw(lTexture, CVector2F(lfX, lfY),
						CVector2F(lfWidth, lfHeight));
				}
				else
				{
					pCanvas->DrawRect(CVector2F(lfX, lfY), CVector2F(lfWidth, lfHeight),
						lMaterial->GetColor());
				}
			}
		}
	}

	void CControl::PostDraw(CCanvas * pCanvas)
	{
		if (mSkin != nullptr)
		{
			CControlMaterial *lMaterial = nullptr;

			if (!mbIsEnabled)
				lMaterial = mSkin->GetMaterial(EControlMaterialType::Disabled);
			else
				lMaterial = mSkin->GetActiveMaterial();

			if (lMaterial != nullptr)
			{
				float lfX = (float)miScreenX;
				float lfY = (float)miScreenY;
				float lfWidth = (float)muiWidth;
				float lfHeight = (float)muiHeight;

				CControlBorder *lBorder = lMaterial->GetBorder();
				if (lBorder != nullptr)
				{
					pCanvas->DrawBorder(CVector2F(lfX, lfY), CVector2F(lfWidth, lfHeight),
						lBorder->GetBorderType(), (float)lBorder->GetBorderWidth(),
						lBorder->GetBorderColors());
				}
			}
		}
	}
}