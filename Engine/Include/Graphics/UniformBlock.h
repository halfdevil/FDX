#pragma once

#include "Graphics/UniformBuffer.h"
#include "Graphics/Program.h"

namespace FDX
{
	class FDX_EXPORT IUniformBlock
	{
	public:

		virtual ~IUniformBlock()
		{
		}

		virtual CUniformBuffer* GetUniformBuffer() const = 0;

		virtual EResult Update() = 0;
		virtual EResult Apply(IProgram *pProgram) = 0;
	};
}