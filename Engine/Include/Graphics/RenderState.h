#pragma once

#include "Core/Result.h"
#include "Math/Vector3F.h"
#include "Math/Color4F.h"

namespace FDX
{
	enum class EBlendOption
	{
		Zero,
		One,
		SrcColor,
		OneMinusSrcColor,
		DestColor,
		OneMinusDestColor,
		SrcAlpha,
		OneMinusSrcAlpha,
		DestAlpha,
		OneMinusDestAlpha,
		SrcAlphaSaturate
	};

	enum class EBlendEquation
	{
		Add,
		Subtract,
		RevSubtract
	};

	enum EColorMask
	{
		None = 0x0,
		Red = 0x1,
		Green = 0x2,
		Blue = 0x4,
		Alpha = 0x8,
		All = 0xF
	};

	enum class ECompareFunction
	{
		Never,
		Less,
		LessOrEqual,
		Greater,
		GreaterOrEqual,
		Equal,
		NotEqual,
		Always
	};

	enum class EStencilOption
	{
		Keep,
		Zero,
		Replace,
		Increment,
		Decrement,
		Invert,
		IncrementWrap,
		DecrementWrap
	};

	enum class EStencilFace
	{
		Front,
		Back,
		FrontAndBack
	};

	enum class ECullMode
	{
		Clockwise,
		CounterClockwise,
		None
	};

	enum class EFillMode
	{
		Point,
		Solid,
		Wireframe
	};

	struct FDX_EXPORT SBlendStateProperties
	{
		SBlendStateProperties()
			: BlendEnabled(false),
			SourceColorBlend(EBlendOption::One),
			DestinationColorBlend(EBlendOption::Zero),
			SourceAlphaBlend(EBlendOption::One),
			DestinationAlphaBlend(EBlendOption::Zero),
			ColorBlendEquation(EBlendEquation::Add),
			AlphaBlendEquation(EBlendEquation::Add)
		{
		}

		bool BlendEnabled;
		EBlendOption SourceColorBlend;
		EBlendOption DestinationColorBlend;
		EBlendOption SourceAlphaBlend;
		EBlendOption DestinationAlphaBlend;
		EBlendEquation ColorBlendEquation;
		EBlendEquation AlphaBlendEquation;
		CColor4F BlendColor;
	};

	struct FDX_EXPORT SRasterizerStateProperties
	{
		SRasterizerStateProperties()
			: CullMode(ECullMode::Clockwise),
			FillMode(EFillMode::Solid),
			ColorMask(EColorMask::All)
		{
		}

		ECullMode CullMode;
		EFillMode FillMode;
		EColorMask ColorMask;
	};

	struct FDX_EXPORT SDepthStateProperties
	{
		SDepthStateProperties()
			: Enabled(true),
			Writeable(true),
			CompareFunction(ECompareFunction::Less)
		{
		}

		bool Enabled;
		bool Writeable;
		ECompareFunction CompareFunction;
	};

	struct FDX_EXPORT SStencilStateProperties
	{
		SStencilStateProperties()
			: Enabled(false),
			Face(EStencilFace::FrontAndBack),
			CompareFunction(ECompareFunction::Always),
			Reference(0),
			Mask(0xFFFFFFFF),
			WriteMask(0xFFFFFFFF),
			OnFail(EStencilOption::Keep),
			OnZFail(EStencilOption::Keep),
			OnZPass(EStencilOption::Keep)
		{
		}

		bool Enabled;
		EStencilFace Face;
		ECompareFunction CompareFunction;
		unsigned int Reference;
		unsigned int Mask;
		unsigned int WriteMask;
		EStencilOption OnFail;
		EStencilOption OnZFail;
		EStencilOption OnZPass;
	};

	class FDX_EXPORT IBlendState
	{
	public:

		virtual const SBlendStateProperties& GetProperties() const = 0;
		virtual void SetProperties(const SBlendStateProperties &pProperties) = 0;

		virtual EResult Apply() = 0;
	};

	class FDX_EXPORT IRasterizerState
	{
	public:

		virtual const SRasterizerStateProperties& GetProperties() const = 0;
		virtual void SetProperties(const SRasterizerStateProperties &pProperties) = 0;

		virtual EResult Apply() = 0;
	};

	class FDX_EXPORT IDepthState
	{
	public:

		virtual const SDepthStateProperties& GetProperties() const = 0;
		virtual void SetProperties(const SDepthStateProperties &pProperties) = 0;

		virtual EResult Apply() = 0;
	};

	class FDX_EXPORT IStencilState
	{
	public:

		virtual const SStencilStateProperties& GetProperties() const = 0;
		virtual void SetProperties(const SStencilStateProperties &pProperties) = 0;

		virtual EResult Apply() = 0;
	};

	using IBlendStatePtr = SmartPtr<IBlendState>;
	using IRasterizerStatePtr = SmartPtr<IRasterizerState>;
	using IDepthStatePtr = SmartPtr<IDepthState>;
	using IStencilStatePtr = SmartPtr<IStencilState>;
}