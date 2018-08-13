#pragma once

#include "Graphics/Program.h"
#include "OGL/GLProgramConstants.h"

namespace FDX
{
	class CGLProgram : public IProgram
	{
	public:

		CGLProgram(const String &pstrName);
		~CGLProgram();

		const String& GetName() const
		{
			return mstrName;
		}

		IProgramConstants* GetConstants() const
		{
			return mConstants;
		}

		EResult Create(CVertexFormat *pFormat);

		bool IsShaderLoaded(EShaderType pType);
		EResult LoadShader(EShaderType pType, CBinaryResource *pResource);

		EResult Build();
		EResult Bind();

	private:

		String mstrName;
		GLuint muiHandle;
		Map<EShaderType, GLuint> mShaderMap;
		CGLProgramConstants *mConstants;
	};
}
