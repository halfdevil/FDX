#pragma once

#include "Core/Core.h"
#include "Core/Result.h"
#include "OS/GameWindow.h"

namespace FDX
{
	class FDX_EXPORT CContextProperties
	{
	public:

		CContextProperties(uint8_t puiRedBits,
			uint8_t puiGreenBits,
			uint8_t puiBlueBits,
			uint8_t puiAlphaBits,
			uint8_t puiDepthBits,
			uint8_t puiStencilBits, 
			uint8_t puiMSAA)
			: muiRedBits(puiRedBits),
			  muiGreenBits(puiGreenBits),
			  muiBlueBits(puiBlueBits),
              muiAlphaBits(puiAlphaBits),
              muiDepthBits(puiDepthBits),
              muiStencilBits(puiStencilBits),
              muiMSAA(puiMSAA)
		{
		}

		uint8_t GetRedBits() const
		{
			return muiRedBits;
		}

        uint8_t GetGreenBits() const
        {
            return muiGreenBits;
        }

        uint8_t GetBlueBits() const
        {
            return muiBlueBits;
        }

        uint8_t GetAlphaBits() const
        {
            return muiAlphaBits;
        }

		uint8_t GetDepthBits() const
		{
			return muiDepthBits;
		}

		uint8_t GetStencilBits() const
		{
			return muiStencilBits;
		}

		uint8_t GetMSAA() const
		{
			return muiMSAA;
		}

	private:

		uint8_t muiRedBits;
		uint8_t muiGreenBits;
		uint8_t muiBlueBits;
		uint8_t muiAlphaBits;
		uint8_t muiDepthBits;
		uint8_t muiStencilBits;
		uint8_t muiMSAA;
	};

	class FDX_EXPORT IGraphicsContext
	{
	public:

		virtual ~IGraphicsContext() = default;

		virtual const CContextProperties& GetProperties() const = 0;
		virtual IGameWindow* GetWindow() const = 0;

		virtual EResult Activate() = 0;
		virtual EResult Deactivate() = 0;
		virtual EResult SwapBuffers() = 0;
	};

	using IGraphicsContextPtr = SmartPtr<IGraphicsContext>;
}