
#include "GUI/ControlFont.h"
#include "Core/ResourceManager.h"
#include "Core/FontResource.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CControlFont::CControlFont()
		: mFontColor(1.0f, 1.0f, 1.0f),
		muiFontSize(12),
		muiFontStyle(EFontStyle::Regular),
		mFont(nullptr)
	{
	}

	CControlFont::~CControlFont()
	{
	}

	void CControlFont::SetFont(IFont * pFont)
	{
		mFont = pFont;
	}

	void CControlFont::SetFont(const String & pstrFilePath)
	{		
		EResult lResult = EResult::Success;
		CFontResource *lResource = nullptr;

		lResult = CResourceManager::Get().GetResource(pstrFilePath, &lResource);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CResourceManager::GetResource() failed");
		}
		else
		{
			mFont = lResource->GetFont();
		}
	}

	void CControlFont::SetFontColor(const CColor4F & pColor)
	{
		mFontColor = pColor;
	}

	void CControlFont::SetFontSize(uint32_t puiSize)
	{
		muiFontSize = puiSize;
	}

	void CControlFont::SetFontStyle(uint32_t puiStyle)
	{
		muiFontStyle = puiStyle;
	}
}