
#include "Scene/BaseMaterial.h"

namespace FDX
{
	CBaseMaterial::CBaseMaterial()
		: mBlendState(nullptr),
		mDepthState(nullptr),
		mRasterizerState(nullptr),
		mStencilState(nullptr)
	{		
	}

	CBaseMaterial::~CBaseMaterial()
	{
	}

	const CColor4F & CBaseMaterial::GetAmbient() const
	{
		return mAmbient;
	}

	const CColor4F & CBaseMaterial::GetDiffuse() const
	{
		return mDiffuse;
	}

	const CColor4F & CBaseMaterial::GetEmissive() const
	{
		return mEmissive;
	}

	const CColor4F & CBaseMaterial::GetSpecular() const
	{
		return mSpecular;
	}

	float CBaseMaterial::GetSpecPower() const
	{
		return mfSpecPower;
	}

	uint32_t CBaseMaterial::GetNumTextures(EPassType pType)
	{
		auto lIt = mTextures.find(pType);
		if (lIt != mTextures.end())
		{
			return (*lIt).second.size();
		}

		return 0;
	}

	ITexture * CBaseMaterial::GetTexture(EPassType pType, uint32_t puiUnit, uint32_t * puiLoc)
	{
		ITexture *lTexture = nullptr;
		auto lIt = mTextures.find(pType);

		if (lIt != mTextures.end())
		{
			if (puiUnit < (*lIt).second.size())
			{
				*puiLoc = mTextureLocations[pType][puiUnit];
				lTexture = (*lIt).second[puiUnit];
			}
		}		

		return lTexture;
	}

	IProgram * CBaseMaterial::GetProgram(EPassType pType)
	{
		auto lIt = mPrograms.find(pType);
		
		if (lIt != mPrograms.end())
			return (*lIt).second;

		return nullptr;
	}

	IBlendState * CBaseMaterial::GetBlendState(EPassType pType)
	{
		return mBlendState;
	}

	IRasterizerState * CBaseMaterial::GetRasterizerState(EPassType pType)
	{
		return mRasterizerState;
	}

	IDepthState * CBaseMaterial::GetDepthState(EPassType pType)
	{
		return mDepthState;
	}

	IStencilState * CBaseMaterial::GetStencilState(EPassType pType)
	{
		return mStencilState;
	}

	void CBaseMaterial::SetAmbient(const CColor4F & pColor)
	{
		mAmbient = pColor;
	}

	void CBaseMaterial::SetDiffuse(const CColor4F & pColor)
	{
		mDiffuse = pColor;
	}

	void CBaseMaterial::SetEmissive(const CColor4F & pColor)
	{
		mEmissive = pColor;
	}

	void CBaseMaterial::SetSpecular(const CColor4F & pColor)
	{
		mSpecular = pColor;
	}

	void CBaseMaterial::SetSpecPower(float pfPower)
	{
		mfSpecPower = pfPower;
	}

	void CBaseMaterial::AddTexture(EPassType pType, ITexture * pTexture, 
		const String &pstrUniform)
	{
		auto lIt = mPrograms.find(pType);
		if (lIt != mPrograms.end())
		{
			uint32_t luiLoc;
			(*lIt).second->GetConstants()->GetConstantLocation(pstrUniform, luiLoc);

			mTextures[pType].push_back(pTexture);
			mTextureLocations[pType].push_back(luiLoc);
		}
	}

	void CBaseMaterial::SetBlendState(IBlendState * pBlendState)
	{
		mBlendState = pBlendState;
	}

	void CBaseMaterial::SetRasterizerState(IRasterizerState * pRasterizerState)
	{
		mRasterizerState = pRasterizerState;
	}

	void CBaseMaterial::SetDepthState(IDepthState * pDepthState)
	{
		mDepthState = pDepthState;
	}

	void CBaseMaterial::SetStencilState(IStencilState * pStencilState)
	{
		mStencilState = pStencilState;
	}

	void CBaseMaterial::SetStartPassCallback(StartPassCallback pStartPass)
	{
		mStartPass = pStartPass;
	}

	void CBaseMaterial::SetEndPassCallback(EndPassCallback pEndPass)
	{
		mEndPass = pEndPass;
	}

	void CBaseMaterial::StartPass(EPassType pType)
	{
		auto lIt = mPrograms.find(pType);
		if (lIt != mPrograms.end())
		{		
			(*lIt).second->GetConstants()->SetColor4("uMaterial.AmbientColor", mAmbient);
			(*lIt).second->GetConstants()->SetColor4("uMaterial.DiffuseColor", mDiffuse);
			(*lIt).second->GetConstants()->SetColor4("uMaterial.EmissiveColor", mEmissive);
			(*lIt).second->GetConstants()->SetColor4("uMaterial.SpecularColor", mSpecular);
			(*lIt).second->GetConstants()->SetFloat("uMaterial.SpecPower", mfSpecPower);

			if (mStartPass)
				mStartPass(pType, (*lIt).second);
		}
	}

	void CBaseMaterial::EndPass(EPassType pType)
	{
		if (mEndPass)
			mEndPass(pType);
	}
}