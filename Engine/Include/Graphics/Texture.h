#pragma once

#include "Core/Image.h"
#include "Graphics/RenderState.h"

namespace FDX
{
	enum class ETextureType
	{
		TwoD,
		Cube,
		TwoDArray
	};

	enum class ETextureUsage
	{
		Normal,
		ColorTarget,
		DepthStencilTarget
	};
	
	enum class ETextureWrap
	{
		Repeat,
		Clamp,
		ClampToEdge,
		ClamToBorder
	};

	enum class ETextureFilter
	{
		Nearest,
		Linear,
		Bilinear,
		Trilinear
	};

	enum class ETextureCompareMode
	{
		None,
		RefToTexture		
	};

	class FDX_EXPORT CTextureProperties
	{
	public:

		CTextureProperties()
			: mFilter(ETextureFilter::Linear),
			mWrapS(ETextureWrap::ClampToEdge),
			mWrapT(ETextureWrap::ClampToEdge),
			mWrapR(ETextureWrap::ClampToEdge),
			mCompareMode(ETextureCompareMode::None),
			mCompareFunction(ECompareFunction::Never),
			mbGenerateMipMaps(true),
			mfAnisotropy(0.0f)
		{
		}
		 
		~CTextureProperties()
		{
		}

		ETextureFilter GetFilter() const
		{
			return mFilter;
		}

		ETextureWrap GetWrapS() const
		{
			return mWrapS;
		}

		ETextureWrap GetWrapT() const
		{
			return mWrapT;
		}

		ETextureWrap GetWrapR() const
		{
			return mWrapR;
		}

		ETextureCompareMode GetCompareMode() const
		{
			return mCompareMode;
		}

		ECompareFunction GetCompareFunction() const
		{
			return mCompareFunction;
		}

		bool GetGenerateMipMaps() const
		{
			return mbGenerateMipMaps;
		}

		float GetAnisotropy() const
		{
			return mfAnisotropy;
		}

		void SetFilter(ETextureFilter pFilter)
		{
			mFilter = pFilter;
		}

		void SetWrapS(ETextureWrap pWrapS)
		{
			mWrapS = pWrapS;
		}

		void SetWrapT(ETextureWrap pWrapT)
		{
			mWrapT = pWrapT;
		}

		void SetWrapR(ETextureWrap pWrapR)
		{
			mWrapR = pWrapR;
		}

		void SetCompareMode(ETextureCompareMode pCompareMode)
		{
			mCompareMode = pCompareMode;
		}

		void SetCompareFunction(ECompareFunction pCompareFunction)
		{
			mCompareFunction = pCompareFunction;
		}

		void SetGenerateMipMaps(bool pbGenerate)
		{
			mbGenerateMipMaps = pbGenerate;
		}

		void SetAnisotropy(float pfAnisotropy)
		{
			mfAnisotropy = pfAnisotropy;
		}

	private:

		ETextureFilter mFilter;
		ETextureWrap mWrapS;
		ETextureWrap mWrapT;
		ETextureWrap mWrapR;
		ETextureCompareMode mCompareMode;
		ECompareFunction mCompareFunction;
		bool mbGenerateMipMaps;
		float mfAnisotropy;
	};

	class FDX_EXPORT ITexture
	{
	public:

		virtual ~ITexture()
		{
		}

		virtual const String& GetName() const = 0;

		virtual ETextureType GetType() const = 0;
		virtual ETextureUsage GetUsage() const = 0;

		virtual bool HasMipMaps() const = 0;

		virtual const CTextureProperties& GetProperties() const = 0;
		virtual void SetProperties(const CTextureProperties &pProperties) = 0;

		virtual void Activate(uint32_t puiUnit) = 0;
	};

	using ITexturePtr = SmartPtr<ITexture>;
}