
#include "OGL/GLProgram.h"
#include "OS/GameLogger.h"
#include "OGL/GLUtil.h"
#include "OS/System.h"

namespace FDX
{
	CGLProgram::CGLProgram(const String & pstrName)
	{
	}

	CGLProgram::~CGLProgram()
	{
		if (muiHandle != 0)
			glDeleteProgram(muiHandle);
	}

	EResult CGLProgram::Create(CVertexFormat *pFormat)
	{
		EResult lResult = EResult::Success;

		muiHandle = glCreateProgram();
		if (!muiHandle)
		{
			LOG_DEBUG("glCreateProgram failed for %s", mstrName.c_str());
			lResult = EResult::CreateFailed;
		}
		else
		{
			uint32_t luiNumElements = pFormat->GetNumElements();
			for (uint32_t liIdx = 0; liIdx < luiNumElements; liIdx++)
			{
				const CVertexElement &lElement = (*pFormat)[liIdx];

				glBindAttribLocation(muiHandle,
					lElement.GetIndex(),
					lElement.GetName().c_str());
			}
		}

		return lResult;
	}

	bool CGLProgram::IsShaderLoaded(EShaderType pType)
	{
		return mShaderMap.find(pType) != mShaderMap.end();
	}

	EResult CGLProgram::LoadShader(EShaderType pType, CBinaryResource * pResource)
	{
		EResult lResult = EResult::Success;

		if (!IsShaderLoaded(pType))
		{
			GLuint luiShader = glCreateShader(CGLUtil::GetShaderType(pType));
			if (!luiShader)
			{
				LOG_DEBUG("glCreateShader failed: %d", glGetError());
				lResult = EResult::CreateFailed;
			}
			else
			{
				auto *lstrSource = reinterpret_cast<const GLchar*>(pResource->GetData());
				auto liSize = static_cast<const GLint>(pResource->GetSize());

				glShaderSource(luiShader, 1, &lstrSource, &liSize);
				glCompileShader(luiShader);

				GLint liStatus = GL_FALSE;
				glGetShaderiv(luiShader, GL_COMPILE_STATUS, &liStatus);

				if (liStatus != GL_TRUE)
				{
					LOG_DEBUG("glCompileShader for %s failed: %d", 
						mstrName.c_str(), glGetError());

					GLint liLogLen = 0;
					glGetShaderiv(luiShader, GL_INFO_LOG_LENGTH, &liLogLen);

					if (liLogLen > 0)
					{
						GLchar *lstrLog = new GLchar[liLogLen + 1];
						glGetShaderInfoLog(luiShader, liLogLen, &liLogLen, lstrLog);
						
						LOG_DEBUG("Compilation error: %s", lstrLog);
						SAFE_DELETE(lstrLog);
					}

					lResult = EResult::LoadFailed;
				}
				else
				{
					mShaderMap[pType] = luiShader;
				}
			}
		}

		return lResult;
	}

	EResult CGLProgram::Build()
	{
		EResult lResult = EResult::Success;

		auto lIt = mShaderMap.begin();
		while (lIt != mShaderMap.end())
		{
			glAttachShader(muiHandle, (*lIt).second);
			lIt++;
		}

		glLinkProgram(muiHandle);

		GLint liStatus = GL_FALSE;
		glGetProgramiv(muiHandle, GL_LINK_STATUS, &liStatus);

		if (liStatus != GL_TRUE)
		{
			LOG_DEBUG("glLinkProgram for %s failed: %d",
				mstrName.c_str(), glGetError());

			GLint liLogLen = 0;
			glGetProgramiv(muiHandle, GL_INFO_LOG_LENGTH, &liLogLen);

			if (liLogLen > 0)
			{
				GLchar *lstrLog = new GLchar[liLogLen + 1];
				glGetProgramInfoLog(muiHandle, liLogLen, &liLogLen, lstrLog);

				LOG_DEBUG("Linking error: %s", lstrLog);
				SAFE_DELETE(lstrLog);
			}

			lResult = EResult::LoadFailed;
		}
		else
		{
			liStatus = GL_FALSE;

			glValidateProgram(muiHandle);
			glGetProgramiv(muiHandle, GL_VALIDATE_STATUS, &liStatus);

			if (liStatus != GL_TRUE)
			{
				LOG_WARN("glValidateProgram for %s failed: %d",
					mstrName.c_str(), glGetError());

				GLint liLogLen = 0;
				glGetProgramiv(muiHandle, GL_INFO_LOG_LENGTH, &liLogLen);

				if (liLogLen > 0)
				{
					auto *lstrLog = new GLchar[liLogLen + 1];
					glGetProgramInfoLog(muiHandle, liLogLen, &liLogLen, lstrLog);

					LOG_WARN("Validation error: %s", lstrLog);
					SAFE_DELETE(lstrLog);
				}
			}

			mConstants = new CGLProgramConstants(muiHandle);
			mConstants->Populate();
		}

		auto lIt2 = mShaderMap.begin();
		while (lIt2 != mShaderMap.end())
		{
			glDetachShader(muiHandle, (*lIt2).second);
			glDeleteShader((*lIt2).second);

			lIt2++;
		}

		mShaderMap.clear();

		return lResult;
	}

	EResult CGLProgram::Bind()
	{
		EResult lResult = EResult::Success;
		
		EMPTY_GL_ERROR();
		glUseProgram(muiHandle);
		
		GLenum lGLError = glGetError();
		if (lGLError != GL_NO_ERROR)
		{
			LOG_DEBUG("glUseProgram() - failed: %d", lGLError);
			lResult = EResult::BindFailed;
		}

		return lResult;
	}
}