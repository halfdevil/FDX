#pragma once

#include "Core/Core.h"
#include "Graphics/RenderState.h"
#include "Graphics/Texture.h"
#include "Graphics/Program.h"

namespace FDX
{
	enum class EPassType
	{
		Diffuse,
		Light,
		Shadow
	};

	using StartPassCallback = std::function<void(EPassType, IProgram*)>;
	using EndPassCallback = std::function<void(EPassType)>;

	class FDX_EXPORT IMaterial
	{
	public:

		virtual ~IMaterial()
		{
		}

		virtual const CColor4F& GetAmbient() const = 0;
		virtual const CColor4F& GetDiffuse() const = 0;
		virtual const CColor4F& GetEmissive() const = 0;
		virtual const CColor4F& GetSpecular() const = 0;
		virtual float GetSpecPower() const = 0;

		virtual uint8_t GetNumPasses() const = 0;
		virtual bool UsesPass(EPassType pType) = 0;

		virtual bool IsTransparent() = 0;

		virtual uint32_t GetNumTextures(EPassType pType) = 0;
		virtual ITexture* GetTexture(EPassType pType, 
			uint32_t puiUnit, uint32_t *puiLoc) = 0;

		virtual IProgram* GetProgram(EPassType pType) = 0;
		virtual IBlendState* GetBlendState(EPassType pType) = 0;
		virtual IRasterizerState* GetRasterizerState(EPassType pType) = 0;
		virtual IDepthState* GetDepthState(EPassType pType) = 0;
		virtual IStencilState* GetStencilState(EPassType pType) = 0;

		virtual void StartPass(EPassType pType) = 0;
		virtual void EndPass(EPassType pType) = 0;
	};

	using IMaterialPtr = SmartPtr<IMaterial>;
}