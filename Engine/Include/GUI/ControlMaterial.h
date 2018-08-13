#pragma once

#include "GUI/ControlAnimation.h"
#include "Graphics/Texture2D.h"
#include "Graphics/RenderState.h"
#include "Graphics/Canvas.h"
#include "GUI/ControlFont.h"
#include "GUI/ControlBorder.h"

namespace FDX
{
	enum class EControlMaterialType : uint32_t
	{
		Default,
		Disabled,
		Hover,
		Pressed
	};

	class FDX_EXPORT CControlMaterial
	{
	public:

		CControlMaterial(EControlMaterialType pMaterialType, const String &pstrName);
		virtual ~CControlMaterial();

		CControlMaterial(const CControlMaterial&) = delete;
		CControlMaterial& operator=(const CControlMaterial&) = delete;

		EControlMaterialType GetMaterialType() const
		{
			return mMaterialType;
		}

		const String& GetName() const
		{
			return mstrName;
		}

		const Array<CTexture2D*>& GetTextures() const
		{
			return mTextures;
		}

		const Array<IControlAnimationPtr>& GetAnimations() const
		{
			return mAnimations;
		}

		IBlendState* GetBlendState() const
		{
			return mBlendState;
		}

		const CColor4F& GetColor() const
		{
			return mColor;
		}

		IControlAnimation* GetCurrentAnimation() const
		{
			return mCurrAnimation;
		}

		uint32_t GetNumTextures() const
		{
			return mTextures.size();
		}

		uint32_t GetNumAnimations() const
		{
			return mAnimations.size();
		}

		CTexture2D* GetCurrentTexture() const
		{
			return mCurrTexture;
		}

		CControlBorder* GetBorder() const
		{
			return mBorder.get();
		}

		CControlFont* GetFont() const
		{
			return mFont.get();
		}

		CTexture2D* GetTexture(uint32_t puiIdx) const
		{
			if (puiIdx < mTextures.size())
				return mTextures[puiIdx];

			return nullptr;
		}

		IControlAnimation* GetAnimation(uint32_t puiIdx) const
		{
			if (puiIdx < mAnimations.size())
				return mAnimations[puiIdx].get();

			return nullptr;
		}

		virtual void AddTexture(CTexture2D *pTexture);
		virtual void AddAnimation(IControlAnimationPtr pAnimation);

		virtual void SetTexture(uint32_t puiIdx, CTexture2D *pTexture);
		virtual void SetCurrentAnimation(uint32_t puiCurrAnimation);
		virtual void SetCurrentTexture(uint32_t puiCurrTexture);
		virtual void SetBlendState(IBlendState *pBlendState);

		virtual void SetColor(const CColor4F &pColor);
		virtual void SetBorder(CControlBorderPtr pBorder);
		virtual void SetFont(CControlFontPtr pFont);
		virtual void Update(float pfDelta);

	protected:

		String mstrName;
		EControlMaterialType mMaterialType;
		Array<CTexture2D*> mTextures;
		Array<IControlAnimationPtr> mAnimations;
		CControlBorderPtr mBorder;
		CControlFontPtr mFont;
		IControlAnimation *mCurrAnimation;
		CTexture2D *mCurrTexture;
		CColor4F mColor;
		IBlendState *mBlendState;		
	};

	using CControlMaterialPtr = SmartPtr<CControlMaterial>;
}