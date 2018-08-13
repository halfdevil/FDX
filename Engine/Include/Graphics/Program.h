#pragma once

#include "Graphics/VertexFormat.h"
#include "Graphics/ProgramConstants.h"
#include "Core/BinaryResource.h"

namespace FDX
{
	class FDX_EXPORT IProgram
	{
	public:

		virtual ~IProgram()
		{
		}

		virtual const String& GetName() const = 0;
		virtual IProgramConstants* GetConstants() const = 0;

		virtual bool IsShaderLoaded(EShaderType pType) = 0;
		virtual EResult LoadShader(EShaderType pType, CBinaryResource *pResource) = 0;

		virtual EResult Build() = 0;
		virtual EResult Bind() = 0;
	};

	using IProgramPtr = SmartPtr<IProgram>;
}
