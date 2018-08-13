#pragma once

#include "Scene/BaseMaterial.h"

namespace FDX
{
	class FDX_EXPORT CSpriteBatchMaterial : public CBaseMaterial
	{
	public:

		CSpriteBatchMaterial();
		virtual ~CSpriteBatchMaterial();

		virtual uint8_t GetNumPasses() const;

		virtual bool UsesPass(EPassType pType);
		virtual bool IsTransparent();

		virtual void SetTransparent(bool pbIsTransparent);
		virtual void SetCustomProgram(IProgram *pProgram);
		virtual void SetTexture(ITexture *pTexture);

		virtual EResult Create() override;

	private:

		uint32_t muiNumPasses;
		bool mbIsTransparent;
	};

	using CSpriteBatchMaterialPtr = SmartPtr<CSpriteBatchMaterial>;
}