#pragma once

#include "Scene/BaseMaterial.h"

namespace FDX
{
	class FDX_EXPORT CMeshMaterial : public CBaseMaterial
	{
	public:

		CMeshMaterial();
		~CMeshMaterial();

		uint8_t GetNumPasses() const;

		bool UsesPass(EPassType pType);
		bool IsTransparent();
		void SetTransparent(bool pbIsTransparent);

		EResult Create() override;
		
	private:

		uint32_t muiNumPasses;
		bool mbIsTransparent;		
	};
}