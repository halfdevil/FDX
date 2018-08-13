#pragma once

#include "Scene/Renderable.h"
#include "Core/Model.h"

namespace FDX
{
	class FDX_EXPORT CSubMeshEntity : public CRenderable
	{
	public:

		CSubMeshEntity();
		~CSubMeshEntity();

		void SetMaterial(IMaterial *pMaterial);
		void SetDrawOperation(IDrawOperation *pDrawOperation);
	};

	using CSubMeshEntityPtr = SmartPtr<CSubMeshEntity>;
}