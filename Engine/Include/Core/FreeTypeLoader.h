#pragma once

#include "Core/FontLoader.h"
#include "ft2build.h"

#include FT_FREETYPE_H
#include FT_ERRORS_H
#include FT_GLYPH_H
#include FT_CACHE_H
#include FT_SIZES_H

namespace FDX
{
	class CFreeTypeLoader;

	struct SFreeTypeChar
	{
		FT_UInt Char;
		FT_UInt Index;
		FT_Glyph Glyph;
		FT_F26Dot6 Advance;
		FT_F26Dot6 Width;
		FTC_Node CacheNode;

		SFreeTypeChar()
			: Char(0), Index((FT_UInt)-1),
			Glyph(nullptr), Advance(0), Width(0),
			CacheNode(nullptr)
		{
		}
	};

	class CUTF8Decoder
	{
	public:

		CUTF8Decoder();
		~CUTF8Decoder();

		EResult DecodeUTF8(const String &pstrString, 
			Array<SFreeTypeChar> &pFTString);
		
	private:
		
		uint32_t GetNextUTF8Byte();
		uint32_t GetUTF8ContByte();
		uint32_t DecodeNextUTF8Char();

	private:

		static const uint32_t UTF8_LINE_END = 0;
		static const uint32_t UTF8_DECODE_ERROR = -1;

		String mstrString;
		const char *mstrBuffer;
		uint32_t muiBufferLen;
		uint32_t muiIndex;
	};

	class CFreeTypeFont : public IFont
	{
	public:

		CFreeTypeFont(CFreeTypeLoader *pLoader);
		~CFreeTypeFont();

		EFontType GetFontType() const
		{
			return EFontType::TTF;
		}

		FT_Face GetFontFace() const
		{
			return mFontFace;
		}

		uint32_t GetDataSize() const
		{
			return muiDataSize;
		}

		uint8_t* GetData() const
		{
			return mData.get();
		}

		const String& GetFilePath() const
		{
			return mstrFilePath;
		}

		bool GetMaskMode() const
		{
			return mbMaskMode;
		}

		void SetMaskMode(bool pbMaskMode)
		{
			mbMaskMode = pbMaskMode;
		}

		EResult Create(const String &pstrFilePath);

		EResult DrawText(const String &pstrString, int32_t puiHeight,
			const CColor4F &pColor, bool pbLeftToRight, CBitmap **pBitmap);

		EResult DrawText(const String &pstrString, int32_t puiHeight,
			int32_t puiStartX, int32_t puiStartY, const CColor4F &pColor,
			bool pbLeftToRight, CBitmap *pBitmap);

		EResult CalculateLineParameters(const String &pstrString, 
			uint32_t puiHeight,	SLineParameters *pParameters);

	private:

		EResult LoadString(const String &pstrString, uint32_t puiHeight);

		void Kern(FT_Face pFace, SFreeTypeChar &pLeft, const SFreeTypeChar &pRight);
		void SetAdvance(SFreeTypeChar &pChar);
		void FreeString();

		FT_UInt GetCharIndex(FT_UInt pChar);
		FT_Glyph GetGlyph(uint32_t puiHeight, FT_UInt pChar,
			FT_UInt pLoadFlags, FTC_Node *pCNode);

		FT_Face GetSizedFace(uint32_t puiHeight);
		EResult DrawGlyphOnBitmap(FT_Bitmap *pFTBitmap, int32_t pX0, int32_t pY0, 
			const CColor4F &pColor, CBitmap *pOutBitmap);

	private:

		CFreeTypeLoader *mLoader;
		String mstrFilePath;
		uint32_t muiDataSize;
		SmartPtr<uint8_t[]> mData;
		FT_Face mFontFace;
		bool mbMaskMode;
		Array<SFreeTypeChar> mFTString;
		SmartPtr<CUTF8Decoder> mUTF8Decoder;
	};

	class CFreeTypeLoader : public IFontLoader
	{
	public:

		CFreeTypeLoader();
		~CFreeTypeLoader();

		EFontLoaderType GetFontLoaderType() const
		{
			return EFontLoaderType::FreeType;
		}

		FT_Library GetLibrary() const
		{
			return mLibrary;
		}

		FTC_Manager GetManager() const
		{
			return mManager;
		}

		FTC_ImageCache GetImageCache() const
		{
			return mImageCache;
		}

		FTC_CMapCache GetCMapCache() const
		{
			return mCMapCache;
		}

		EResult Create();

		bool CanLoadFontType(EFontType pType);
		bool CanLoadFont(const String &pstrFilePath);

		EResult Load(const String &pstrFilePath, IFont **pFont);

	private:

		static FT_Error FaceRequester(FTC_FaceID pFaceId, FT_Library pLibrary, 
			FT_Pointer pRequestData, FT_Face *pFace);

	private:

		FT_Library mLibrary;
		FTC_Manager mManager;
		FTC_ImageCache mImageCache;
		FTC_CMapCache mCMapCache;
	};
}