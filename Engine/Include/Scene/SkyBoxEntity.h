#pragma once

#include "Scene/BaseMaterial.h"
#include "Scene/Renderable.h"

namespace FDX
{
	class FDX_EXPORT CSkyBoxMaterial : public CBaseMaterial
	{
	public:

		CSkyBoxMaterial();
		~CSkyBoxMaterial() = default;

		uint8_t GetNumPasses() const override;

		bool UsesPass(EPassType pType) override;
		bool IsTransparent() override;

		EResult Create() override;

	private:

		uint32_t muiNumPasses;
	};

	class FDX_EXPORT CSkyBoxEntity : public CSceneEntity
	{
	public:

		CSkyBoxEntity();
		~CSkyBoxEntity();

		EResult Load(const Array<String> &pImages);

		void StartPass(EPassType pPassType, IProgram *pProgram);
		void EndPass(EPassType pPassType);

		void Draw(CCamera *pCamera);

	private:

		EResult LoadRenderData();

	private:

		IMaterialPtr mMaterial;
		IDrawOperationPtr mDrawOperation;
	};

	using CSkyBoxEntityPtr = SmartPtr<CSkyBoxEntity>;
}