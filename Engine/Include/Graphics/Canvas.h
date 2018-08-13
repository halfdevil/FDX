#pragma once

#include "Core/Bitmap.h"
#include "Core/Font.h"
#include "Graphics/VertexTypes.h"
#include "OS/GameWindow.h"
#include "Graphics/BatchRenderer.h"
#include "Graphics/CanvasMaterial.h"
#include "Graphics/SpriteBatch.h"
#include "Graphics/GraphicsDevice.h"

namespace FDX
{
	enum EBorderType
	{
		NoBorder = 0x0,
		Left = 0x1,
		Top = 0x2,
		Right = 0x4, 
		Bottom = 0x8,
		AllBorder = 0xF
	};

	class FDX_EXPORT CCanvas : public CSpriteBatch
	{
	public:

		CCanvas();
		~CCanvas();

		CCanvas(const CCanvas&) = delete;
		CCanvas& operator=(const CCanvas&) = delete;
		
		EResult Create(IGraphicsDevice *pGraphicsDevice, uint32_t puiMaxNumVertices,
			uint32_t puiMaxNumIndices) override;

		EResult Begin() override;
		EResult End() override;
		
		EResult DrawRect(const CVector2F &pPosition, const CVector2F &pSize, const CColor4F &pColor);

		EResult DrawBorder(const CVector2F &pPosition, const CVector2F &pSize,
			uint32_t pRectBorder, float pfBorderWidth, 
			UnorderedMap<EBorderType, CColor4F> &pBorderColors);

		EResult DrawRectWithBorder(const CVector2F &pPosition, const CVector2F &pSize, 
			const CColor4F &pColor, uint32_t pRectBorder, float pfBorderWidth,
			UnorderedMap<EBorderType, CColor4F> &pBorderColors);
				
		EResult DrawText(const CVector2F &pPosition, const CVector2F &pRectSize,
			const String &pstrString, uint32_t puiSize, const CColor4F &pColor, IFont *pFont);

		EResult DrawText(const CVector2F &pPosition, const CVector2F &pSrcPosition, 
			const CVector2F &pSrcSize, const String &pstrString, uint32_t puiSize, 
			const CColor4F &pColor, IFont *pFont);

		void StartPass(EPassType pPassType, IProgram *pProgram) override;
		void EndPass(EPassType pPassType) override;

	private:

		EResult SetupStates(IGraphicsDevice *pGraphicsDevice);
		EResult UpdateFontTexture(CBitmap *pBitmap);

	private:

		CTexture2DPtr mFontTexture;
		IBlendStatePtr mEnableBlendState;
	};

	using CCanvasPtr = SmartPtr<CCanvas>;
}