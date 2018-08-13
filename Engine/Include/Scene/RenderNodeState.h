#pragma once

#include "Scene/Material.h"
#include "Graphics/DrawOperation.h"
#include "Graphics/UniformBlock.h"
#include "Graphics/ShadowMap.h"
#include "Scene/Renderable.h"
#include "Scene/Light.h"
#include "Scene/Camera.h"

namespace FDX
{
	enum class ERenderNodeStateType
	{
		Pass,
		BlendState,
		RasterizerState,
		DepthState,
		StencilState,
		Program,
		Light,
		Texture,
		DrawOperation,
		Matrix,
		Draw
	};

	struct STexture
	{
		ITexture *Texture;
		uint32_t Unit;
		uint32_t Location;

		STexture()
			: Texture(nullptr),
			Unit(0),
			Location(0)
		{
		}

		STexture(ITexture *pTexture, uint32_t puiUnit, uint32_t puiLoc)
			: Texture(pTexture),
			Unit(puiUnit),
			Location(puiLoc)
		{
		}
	};

	class CRenderNodeState
	{
	public:

		CRenderNodeState();
		~CRenderNodeState();

		int32_t Compare(const CRenderNodeState *pState);

		void CopyOf(const CRenderNodeState *pState);
		void Apply();

	private:

		int32_t ComparePass(const CRenderNodeState *pState);
		int32_t CompareBlendState(const CRenderNodeState *pState);
		int32_t CompareRasterizerState(const CRenderNodeState *pState);
		int32_t CompareDepthState(const CRenderNodeState *pState);
		int32_t CompareStencilState(const CRenderNodeState *pState);
		int32_t CompareTextures(const CRenderNodeState *pState);
		int32_t CompareProgram(const CRenderNodeState *pState);
		int32_t CompareLight(const CRenderNodeState *pState);
		int32_t CompareDrawOperation(const CRenderNodeState *pState);
		int32_t CompareGlobalMatrix(const CRenderNodeState *pState);
		int32_t CompareDraw(const CRenderNodeState *pState);

		void ApplyPass();
		void ApplyBlendState();
		void ApplyRasterizerState();
		void ApplyDepthState();
		void ApplyStencilState();
		void ApplyTextures();
		void ApplyProgram();
		void ApplyLight();
		void ApplyDrawOperation();
		void ApplyGlobalMatrix();
		void ApplyDraw();

	public:

		ERenderNodeStateType StateType;
		EPassType Pass;
		IBlendState *BlendState;
		IRasterizerState *RasterizerState;
		IDepthState *DepthState;
		IStencilState *StencilState;
		Array<STexture> Textures;
		IProgram *Program;
		ILight *Light;
		IMaterial *Material;
		IDrawOperation *DrawOperation;
		CRenderable *Renderable;
		CMatrix4F *GlobalMatrix;
		CCamera *Camera;
		IUniformBlock *UniformBlock;
	};
}