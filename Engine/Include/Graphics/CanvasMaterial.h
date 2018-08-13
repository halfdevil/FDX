#pragma once

#include "Graphics/SpriteBatchMaterial.h"

namespace FDX
{
	class FDX_EXPORT CCanvasMaterial : public CSpriteBatchMaterial
	{
	public:

		CCanvasMaterial();
		~CCanvasMaterial();

		EResult Create() override;
	};

	using CCanvasMaterialPtr = SmartPtr<CCanvasMaterial>;
}