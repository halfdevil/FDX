
#include "Core/FreeTypeLoader.h"
#include "Core/FontResource.h"
#include "FS/FileSystem.h"
#include "OS/System.h"
#include "Core/ResourceManager.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CUTF8Decoder::CUTF8Decoder()
	{
	}

	CUTF8Decoder::~CUTF8Decoder()
	{
	}

	uint32_t CUTF8Decoder::GetNextUTF8Byte()
	{
		if (muiIndex >= muiBufferLen)
			return UTF8_LINE_END;

		return mstrBuffer[muiIndex++] & 0xFF;
	}

	uint32_t CUTF8Decoder::GetUTF8ContByte()
	{
		uint32_t luiChar = GetNextUTF8Byte();

		if ((luiChar & 0xC0) == 0x80)
			return luiChar & 0x3F;

		return UTF8_DECODE_ERROR;
	}

	EResult CUTF8Decoder::DecodeUTF8(const String & pstrString, Array<SFreeTypeChar> &pFTString)
	{
		EResult lResult = EResult::Success;

		muiIndex = 0;
		mstrBuffer = pstrString.c_str();
		muiBufferLen = pstrString.length();

		uint32_t luiNextChar = DecodeNextUTF8Char();
		while (luiNextChar != UTF8_LINE_END && luiNextChar != UTF8_DECODE_ERROR)
		{
			SFreeTypeChar lCh;
			
			lCh.Char = luiNextChar;
			pFTString.push_back(lCh);

			luiNextChar = DecodeNextUTF8Char();
		}

		if (luiNextChar == UTF8_DECODE_ERROR)
			lResult = EResult::LoadFailed;

		return lResult;
	}

	uint32_t CUTF8Decoder::DecodeNextUTF8Char()
	{
		uint32_t luiChar, luiRet;

		if (muiIndex >= muiBufferLen)
		{
			if (muiIndex == muiBufferLen)
				return UTF8_LINE_END;
			else
				return UTF8_DECODE_ERROR;
		}

		luiChar = GetNextUTF8Byte();

		if ((luiChar & 0x80) == 0)
			return luiChar;

		if ((luiChar & 0xE0) == 0xC0)
		{
			uint32_t luiChar1 = GetUTF8ContByte();

			if (luiChar1 == UTF8_DECODE_ERROR)
				return UTF8_DECODE_ERROR;

			luiRet = ((luiChar & 0x1F) << 6) | luiChar1;
			return luiRet >= 128 ? luiRet : UTF8_DECODE_ERROR;
		}

		if ((luiChar & 0xF0) == 0xE0)
		{
			uint32_t luiChar1 = GetUTF8ContByte();
			uint32_t luiChar2 = GetUTF8ContByte();

			if (luiChar1 == UTF8_DECODE_ERROR ||
				luiChar2 == UTF8_DECODE_ERROR)
			{
				return UTF8_DECODE_ERROR;
			}

			luiRet = ((luiChar & 0x0F) << 12) | (luiChar1 << 6) | luiChar2;
			return luiRet >= 2048 && (luiRet < 55296 || luiRet > 57343) ? 
				luiRet : UTF8_DECODE_ERROR;
		}

		if ((luiChar & 0xF8) == 0xF0)
		{
			uint32_t luiChar1 = GetUTF8ContByte();
			uint32_t luiChar2 = GetUTF8ContByte();
			uint32_t luiChar3 = GetUTF8ContByte();

			if (luiChar1 == UTF8_DECODE_ERROR ||
				luiChar2 == UTF8_DECODE_ERROR ||
				luiChar3 == UTF8_DECODE_ERROR)
			{
				return UTF8_DECODE_ERROR;
			}

			luiRet = ((luiChar & 0x0F) << 18) | (luiChar1 << 12) | (luiChar2 << 6) | luiChar3;
			return luiRet >= 65536 && luiRet <= 1114111 ?
				luiRet : UTF8_DECODE_ERROR;
		}

		return UTF8_DECODE_ERROR;
	}

	CFreeTypeFont::CFreeTypeFont(CFreeTypeLoader *pLoader)
		: mLoader(pLoader),
		muiDataSize(0),
		mbMaskMode(false)
	{
	}

	CFreeTypeFont::~CFreeTypeFont()
	{
	} 

	EResult CFreeTypeFont::Create(const String & pstrFilePath)
	{
		EResult lResult = EResult::Success;
		IFileStream *lStream = nullptr;

		lResult = IFileSystem::Get().GetStream(pstrFilePath, &lStream);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("Font GetStream failed: %s", pstrFilePath.c_str());
		}
		else
		{
			auto lFontData = SmartPtr<uint8_t[]>(new uint8_t[lStream->GetSize()]);
			lResult = lStream->Read(lStream->GetSize(), lFontData.get());

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("Font data read from stream failed: %s", pstrFilePath.c_str());
			}
			else
			{
				mData = std::move(lFontData);
				muiDataSize = lStream->GetSize();
				mstrFilePath = pstrFilePath;
				mUTF8Decoder = SmartPtr<CUTF8Decoder>(new CUTF8Decoder());

				FT_Error lFTResult = FT_New_Memory_Face(mLoader->GetLibrary(), 
					(FT_Byte*)mData.get(), muiDataSize, 0, &mFontFace);

				if (lFTResult != 0)
				{
					LOG_DEBUG("FT_New_Memory_Face() failed: %s", pstrFilePath.c_str());
					lResult = EResult::CreateFailed;
				}
			}
		}

		return lResult;
	}

	EResult CFreeTypeFont::DrawText(const String & pstrString, int32_t puiHeight,
		const CColor4F & pColor, bool pbLeftToRight, CBitmap ** pBitmap)
	{
		EResult lResult = EResult::Success;

		CBitmap *lBitmap = nullptr;
		SLineParameters lParameters;
		
		lResult = CalculateLineParameters(pstrString, puiHeight, &lParameters);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CalculateLineParameters() failed");
		}
		else
		{
			lBitmap = new CBitmap();
			lResult = lBitmap->Create(lParameters.Width,
				lParameters.MaxY + lParameters.MinY, ESurfaceFormat::BGRA8);

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("Bitmap Create() failed");
				SAFE_DELETE(lBitmap);
			}
			else
			{
				lResult = DrawText(pstrString, puiHeight, pbLeftToRight ? 0 : lParameters.Width - 1,
					lParameters.MinY, pColor, pbLeftToRight, lBitmap);

				if (lResult != EResult::Success)
				{
					LOG_DEBUG("DrawText() failed");
					SAFE_DELETE(lBitmap);
				}
				else
				{
					*pBitmap = lBitmap;
				}
			}
		}

		return lResult;
	}

	EResult CFreeTypeFont::DrawText(const String & pstrString, int32_t puiHeight,	int32_t puiStartX, 
		int32_t puiStartY, const CColor4F & pColor, bool pbLeftToRight, CBitmap * pBitmap)
	{
		EResult lResult = EResult::Success;

		lResult = LoadString(pstrString, puiHeight);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("LoadString() failed");
		}
		else
		{
			int32_t luiX = puiStartX << 6;
			for (uint32_t lIdx = 0; lIdx < mFTString.size(); lIdx++)
			{
				if (mFTString[lIdx].Glyph != nullptr)
				{
					FT_BitmapGlyph lBmpGlyph = (FT_BitmapGlyph)mFTString[lIdx].Glyph;
					int32_t luiInX = (luiX >> 6) + ((pbLeftToRight ? 1 : -1) * lBmpGlyph->left);
					
					if (!pbLeftToRight)
					{
						luiInX += lBmpGlyph->bitmap.width;
						luiInX = puiStartX + (puiStartX - luiInX);
					}

					lResult = DrawGlyphOnBitmap(&lBmpGlyph->bitmap, luiInX,
						puiStartY - lBmpGlyph->top, pColor, pBitmap);

					if (lResult != EResult::Success)
					{
						LOG_DEBUG("DrawGlyphOnBitmap() failed");
					}
				}

				luiX += mFTString[lIdx].Advance;
			}
		}

		return lResult;
	}

	EResult CFreeTypeFont::CalculateLineParameters(const String & pstrString, uint32_t puiHeight, 
		SLineParameters * pParameters)
	{
		EResult lResult = EResult::Success;

		lResult = LoadString(pstrString, puiHeight);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("FreeTypeFont LoadString() failed");
		}
		else
		{
			int32_t liStrMinY = -1000;
			int32_t liStrMaxY = -1000;

			if (mFTString.empty())
			{
				liStrMinY = 0;
				liStrMaxY = 0;
			}

			int32_t liSizeX = 0;

			for (uint32_t lIdx = 0; lIdx < mFTString.size(); lIdx++)
			{
				if (mFTString[lIdx].Glyph == nullptr)
					continue;

				FT_BitmapGlyph lBmpGlyph = (FT_BitmapGlyph)mFTString[lIdx].Glyph;
				liSizeX += mFTString[lIdx].Advance;

				int32_t liY = lBmpGlyph->top;
				int32_t liH = lBmpGlyph->bitmap.rows;

				if (liY > liStrMinY)
					liStrMinY = liY;

				if (liH - liY > liStrMaxY)
					liStrMaxY = liH - liY;
			}

			if (pParameters)
			{
				pParameters->Width = (liSizeX >> 6);
				pParameters->Baseline = liStrMaxY;
				pParameters->MinY = liStrMinY;
				pParameters->MaxY = liStrMaxY;
			}
		}

		return lResult;
	}

	EResult CFreeTypeFont::LoadString(const String & pstrString, uint32_t puiHeight)
	{
		EResult lResult = EResult::Success;		
		FT_Face lFace = nullptr;
			
		lFace = GetSizedFace(puiHeight);
		if (lFace == nullptr)
		{
			LOG_DEBUG("GetSizedFace() failed");
			lResult = EResult::LoadFailed;
		}
		else
		{
			FreeString();

			lResult = mUTF8Decoder->DecodeUTF8(pstrString, mFTString);
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("DecodeUTF8() failed");
			}
			else
			{
				bool lbUseKerning = FT_HAS_KERNING(lFace) != 0;
				for (uint32_t lIdx = 0; lIdx < mFTString.size(); lIdx++)
				{
					SFreeTypeChar &lChar = mFTString[lIdx];
					FT_UInt lCh = lChar.Char;

					lChar.Index = (lCh != '\n' && lCh != '\r') ? GetCharIndex(lCh) : -1;
					lChar.Glyph = (lChar.Index != -1) ? GetGlyph(puiHeight, lCh, FT_LOAD_RENDER, &lChar.CacheNode) : nullptr;

					if (!lChar.Glyph || lChar.Index == -1)
						continue;

					SetAdvance(lChar);

					if (lIdx > 0 && lbUseKerning)
						Kern(lFace, mFTString[lIdx - 1], lChar);
				}
			}
		}

		return lResult;
	}

	void CFreeTypeFont::Kern(FT_Face pFace, SFreeTypeChar & pLeft, const SFreeTypeChar & pRight)
	{
		if (pLeft.Index == -1 || pRight.Index == -1)
			return;

		FT_Vector lDelta;
		FT_Get_Kerning(pFace, pLeft.Index, pRight.Index, FT_KERNING_DEFAULT, &lDelta);

		pLeft.Advance += lDelta.x;
	}

	void CFreeTypeFont::SetAdvance(SFreeTypeChar & pChar)
	{
		pChar.Advance = 0;
		pChar.Width = 0;

		if (!pChar.Glyph)
			return;

		pChar.Advance = pChar.Glyph->advance.x >> 10;

		FT_BBox lBBox;
		FT_Glyph_Get_CBox(pChar.Glyph, FT_GLYPH_BBOX_GRIDFIT, &lBBox);

		pChar.Width = (lBBox.xMax - lBBox.xMin);

		if (pChar.Width == 0 && pChar.Advance != 0)
			pChar.Width = pChar.Advance;
	}

	void CFreeTypeFont::FreeString()
	{
		for (uint32_t lIdx = 0; lIdx < mFTString.size(); lIdx++)
		{
			if (mFTString[lIdx].CacheNode != nullptr)
			{
				FTC_Node_Unref(mFTString[lIdx].CacheNode, mLoader->GetManager());
				mFTString[lIdx].CacheNode = nullptr;
			}
		}

		mFTString.clear();
	}

	FT_UInt CFreeTypeFont::GetCharIndex(FT_UInt pChar)
	{
		return FTC_CMapCache_Lookup(mLoader->GetCMapCache(), (FTC_FaceID)mstrFilePath.c_str(),
			-1, pChar);
	}

	FT_Glyph CFreeTypeFont::GetGlyph(uint32_t puiHeight, FT_UInt pChar, FT_UInt pLoadFlags, FTC_Node * pCNode)
	{
		FT_UInt lIndex = GetCharIndex(pChar);

		FTC_ImageTypeRec lImageType;
		lImageType.face_id = (FTC_FaceID)mstrFilePath.c_str();
		lImageType.height = puiHeight;
		lImageType.width = 0;
		lImageType.flags = pLoadFlags;

		FT_Glyph lGlyph;		
		if (FTC_ImageCache_Lookup(mLoader->GetImageCache(), &lImageType, lIndex, &lGlyph, pCNode) != 0)
			return nullptr;

		return lGlyph;
	}

	FT_Face CFreeTypeFont::GetSizedFace(uint32_t puiHeight)
	{
		FT_Size lSizedFont;
		FTC_ScalerRec lScaler;

		lScaler.face_id = (FTC_FaceID)mstrFilePath.c_str();
		lScaler.height = puiHeight;
		lScaler.width = 0;
		lScaler.pixel = 1;
		
		if (FTC_Manager_LookupSize(mLoader->GetManager(), &lScaler, &lSizedFont) != 0)
			return nullptr;

		if (FT_Activate_Size(lSizedFont) != 0)
			return nullptr;

		return lSizedFont->face;
	}

	EResult CFreeTypeFont::DrawGlyphOnBitmap(FT_Bitmap * pFTBitmap, int32_t puiX0, int32_t puiY0, 
		const CColor4F & pColor, CBitmap * pOutBitmap)
	{
		EResult lResult = EResult::Success;

		int32_t luiW = pOutBitmap->GetWidth();
		int32_t luiWidth = luiW - puiX0;

		if (luiWidth > (int32_t)pFTBitmap->width)
			luiWidth = (int32_t)pFTBitmap->width;

		for (int32_t luiY = puiY0; luiY < puiY0 + (int32_t)pFTBitmap->rows; luiY++)
		{
			uint8_t *luiSrc = pFTBitmap->buffer + 
				(luiY - puiY0) * pFTBitmap->pitch;

			if (mbMaskMode)
			{
				for (int32_t luiX = puiX0; luiX < puiX0 + luiWidth; luiX++) 
				{
					uint8_t luiIn = *luiSrc++;

					pOutBitmap->SetPixel(luiX, luiY,
						CColor4F(luiIn, luiIn, luiIn));
				}
			}
			else
			{
				for (int32_t luiX = puiX0; luiX < puiX0 + luiWidth; luiX++)
				{
					uint8_t luiIn = *luiSrc++;

					if (luiIn > 0)
					{
						CColor4F lColor = pColor;
						lColor.A = (luiIn / 255.0f);

						pOutBitmap->SetPixel(luiX, luiY, lColor);
					}
				}
			}
		}

		return lResult;
	}

	CFreeTypeLoader::CFreeTypeLoader()
		: mLibrary(nullptr),
		mManager(nullptr),
		mImageCache(nullptr),
		mCMapCache(nullptr)
	{
	}

	CFreeTypeLoader::~CFreeTypeLoader()
	{
		if (mManager)
			FTC_Manager_Done(mManager);

		if (mLibrary)
			FT_Done_FreeType(mLibrary);
	}

	EResult CFreeTypeLoader::Create()
	{
		EResult lResult = EResult::Success;

		if (FT_Init_FreeType(&mLibrary) != 0)
		{
			LOG_DEBUG("FT_Init_FreeType() failed");
			lResult = EResult::CreateFailed;
		}
		else
		{
			if (FTC_Manager_New(mLibrary, 0, 0, 0, FaceRequester, this, &mManager) != 0)
			{
				LOG_DEBUG("FTC_Manager_New() failed");
				lResult = EResult::CreateFailed;
			}
			else
			{
				if (FTC_ImageCache_New(mManager, &mImageCache) != 0)
				{
					LOG_DEBUG("FTC_ImageCache_New() failed");
					lResult = EResult::CreateFailed;
				}
				else
				{
					if (FTC_CMapCache_New(mManager, &mCMapCache) != 0)
					{
						LOG_DEBUG("FTC_CMapCache_New() failed");
						lResult = EResult::CreateFailed;
					}
				}
			}
		}

		return lResult;
	}

	bool CFreeTypeLoader::CanLoadFontType(EFontType pType)
	{
		return pType == EFontType::TTF;
	}

	bool CFreeTypeLoader::CanLoadFont(const String & pstrFilePath)
	{
		bool lbCanLoad = false;
		uint32_t luiDot = pstrFilePath.find_last_of(".");

		if (luiDot != (uint32_t)String::npos)
		{
			String lstrExt = pstrFilePath.substr(luiDot);
			std::transform(lstrExt.begin(), lstrExt.end(), lstrExt.begin(), ::tolower);

			if (lstrExt == ".ttf")
			{
				lbCanLoad = true;
			}
		}

		return lbCanLoad;
	}

	EResult CFreeTypeLoader::Load(const String & pstrFilePath, IFont ** pFont)
	{
		EResult lResult = EResult::Success;

		CFreeTypeFont *lFont = new CFreeTypeFont(this);
		lResult = lFont->Create(pstrFilePath);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CFreeTypeFont Create() failed: %s", pstrFilePath.c_str());
		}
		else
		{
			*pFont = lFont;
		}

		return lResult;
	}

	FT_Error CFreeTypeLoader::FaceRequester(FTC_FaceID pFaceId, FT_Library pLibrary, 
		FT_Pointer pRequestData, FT_Face * pFace)
	{
		EResult lResult = EResult::Success;
		FT_Error lError = 0;

		String lstrFilePath(reinterpret_cast<const char*>(pFaceId));
		CFontResource *lResource = nullptr;

		lResult = CResourceManager::Get().GetResource(lstrFilePath, &lResource);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CResourceManager GetResource() failed for %s", lstrFilePath.c_str());
			lError = 1;
		}
		else
		{
			CFreeTypeFont *lFreeTypeFont = static_cast<CFreeTypeFont*>(lResource->GetFont());
			*pFace = lFreeTypeFont->GetFontFace();
		}

		return lError;
	}
}