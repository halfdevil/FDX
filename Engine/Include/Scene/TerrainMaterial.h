#pragma once

#include "Scene/BaseMaterial.h"

namespace FDX
{
	class FDX_EXPORT CTerrainMaterial : public CBaseMaterial
	{
	public:

		CTerrainMaterial();
		~CTerrainMaterial();

		uint8_t GetNumPasses() const;

		bool UsesPass(EPassType pType);
		bool IsTransparent();

		EResult Create() override;

	private:

		uint32_t muiNumPasses;
	};
}