#pragma once

#include "Graphics/Texture2D.h"
#include "Graphics/Program.h"

namespace FDX
{
	class FDX_EXPORT IPostProcessor
	{
	public:

		virtual ~IPostProcessor()
		{
		}
		
		virtual CTexture2D* GetTexture() const = 0;
		virtual EResult Process(IProgram *pProgram) = 0;
	};
}