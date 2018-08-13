
#include "GUI/ControlMaterial.h"

namespace FDX
{
	CControlMaterial::CControlMaterial(EControlMaterialType pMaterialType, const String &pstrName)
		: mMaterialType(pMaterialType),
		mstrName(pstrName),
		mBlendState(nullptr),
		mCurrAnimation(nullptr),
		mCurrTexture(nullptr),
		mBorder(nullptr),
		mFont(nullptr)
	{
	}

	CControlMaterial::~CControlMaterial()
	{
	}

	void CControlMaterial::AddTexture(CTexture2D * pTexture)
	{
		mTextures.push_back(pTexture);
	}

	void CControlMaterial::SetTexture(uint32_t puiIdx, CTexture2D * pTexture)
	{
		if (puiIdx < mTextures.size())
			mTextures[puiIdx] = pTexture;
	}

	void CControlMaterial::AddAnimation(IControlAnimationPtr pAnimation)
	{
		mAnimations.push_back(std::move(pAnimation));
	}

	void CControlMaterial::SetCurrentAnimation(uint32_t puiCurrAnimation)
	{
		if (puiCurrAnimation < mAnimations.size())
			mCurrAnimation = mAnimations[puiCurrAnimation].get();
	}

	void CControlMaterial::SetCurrentTexture(uint32_t puiCurrTexture)
	{
		if (puiCurrTexture < mTextures.size())
			mCurrTexture = mTextures[puiCurrTexture];
	}

	void CControlMaterial::SetBlendState(IBlendState * pBlendState)
	{
		mBlendState = pBlendState;
	}

	void CControlMaterial::SetColor(const CColor4F & pColor)
	{
		mColor = pColor;
	}

	void CControlMaterial::SetBorder(CControlBorderPtr pBorder)
	{
		mBorder = std::move(pBorder);
	}

	void CControlMaterial::SetFont(CControlFontPtr pFont)
	{
		mFont = std::move(pFont);
	}

	void CControlMaterial::Update(float pfDelta)
	{
		if (mCurrAnimation != nullptr)
		{
			mCurrAnimation->Update(pfDelta, mTextures.size());
			mCurrTexture = mTextures[mCurrAnimation->GetActiveFrame()];
		}
	}
}