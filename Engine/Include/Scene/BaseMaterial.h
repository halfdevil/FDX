#pragma once

#include "Scene/Material.h"

namespace FDX
{
	class FDX_EXPORT CBaseMaterial : public IMaterial
	{
	public:

		CBaseMaterial();
		virtual ~CBaseMaterial();

		virtual const CColor4F& GetAmbient() const;
		virtual const CColor4F& GetDiffuse() const;
		virtual const CColor4F& GetEmissive() const;
		virtual const CColor4F& GetSpecular() const;
		virtual float GetSpecPower() const;

		virtual uint32_t GetNumTextures(EPassType pType);
		virtual ITexture* GetTexture(EPassType pType,
			uint32_t puiUnit, uint32_t *puiLoc);

		virtual IProgram* GetProgram(EPassType pType);
		virtual IBlendState* GetBlendState(EPassType pType);
		virtual IRasterizerState* GetRasterizerState(EPassType pType);
		virtual IDepthState* GetDepthState(EPassType pType);
		virtual IStencilState* GetStencilState(EPassType pType);

		virtual void SetAmbient(const CColor4F &pColor);
		virtual void SetDiffuse(const CColor4F &pColor);
		virtual void SetEmissive(const CColor4F &pColor);
		virtual void SetSpecular(const CColor4F &pColor);
		virtual void SetSpecPower(float pfPower);

		virtual void SetBlendState(IBlendState *pBlendState);
		virtual void SetRasterizerState(IRasterizerState *pRasterizerState);
		virtual void SetDepthState(IDepthState *pDepthState);
		virtual void SetStencilState(IStencilState *pStencilState);

		virtual void SetStartPassCallback(StartPassCallback pStartPass);
		virtual void SetEndPassCallback(EndPassCallback pEndPass);
		virtual void StartPass(EPassType pType);
		virtual void EndPass(EPassType pType);

		virtual void AddTexture(EPassType pType, ITexture *pTexture, 
			const String &pstrUniform);

		virtual EResult Create() = 0;

	protected:

		CColor4F mAmbient;
		CColor4F mDiffuse;
		CColor4F mEmissive;
		CColor4F mSpecular;
		float mfSpecPower;

		UnorderedMap<EPassType, Array<ITexture*>> mTextures;
		UnorderedMap<EPassType, Array<uint32_t>> mTextureLocations;
		UnorderedMap<EPassType, IProgram*> mPrograms;

		IBlendState *mBlendState;
		IRasterizerState *mRasterizerState;
		IDepthState *mDepthState;
		IStencilState *mStencilState;
		StartPassCallback mStartPass;
		EndPassCallback mEndPass;
	};
}