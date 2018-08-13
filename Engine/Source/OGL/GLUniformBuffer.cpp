
#include "OGL/GLUniformBuffer.h"
#include "OS/GameLogger.h"
#include "OGL/GLUtil.h"
#include "OS/System.h"

namespace FDX
{
	CGLUniformBuffer::CGLUniformBuffer(const String & pstrName)
		: mstrName(pstrName),
		muiHandle(0),
		mUsage(EBufferUsage::Dynamic),
		muiBindingIndex(0),
		muiSize(0)
	{
	}

	CGLUniformBuffer::~CGLUniformBuffer()
	{
		if (muiHandle != 0)
			glDeleteBuffers(1, &muiHandle);
	}

	void CGLUniformBuffer::SetBindingIndex(uint32_t puiIndex)
	{
		muiBindingIndex;
	}

	EResult CGLUniformBuffer::Create(EBufferUsage pUsage, uint32_t puiSize, uint32_t puiBindingIndex)
	{
		FDX_ASSERT(muiHandle == 0, "Buffer already created");

		EResult lResult = EResult::Success;
		GLenum lGLError = GL_NO_ERROR;

		EMPTY_GL_ERROR();

		glGenBuffers(1, &muiHandle);
		if (!muiHandle)
		{
			LOG_DEBUG("glGenBuffers() failed: %d", glGetError());
			lResult = EResult::CreateFailed;
		}
		else
		{
			glBindBuffer(GL_UNIFORM_BUFFER, muiHandle);
			glBufferData(GL_UNIFORM_BUFFER, puiSize,
				nullptr, CGLUtil::GetBufferUsage(pUsage));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindBufferRange(GL_UNIFORM_BUFFER, puiBindingIndex, muiHandle, 0, puiSize);

			mUsage = pUsage;
			muiSize = puiSize;
			muiBindingIndex = puiBindingIndex;
		}

		return lResult;
	}

	EResult CGLUniformBuffer::UploadData(uint32_t puiOffset, uint32_t puiSize, const void * pData)
	{
		FDX_ASSERT(muiHandle != 0, "Buffer not created");

		EResult lResult = EResult::Success;
		EMPTY_GL_ERROR();

		glBindBuffer(GL_UNIFORM_BUFFER, muiHandle);
		glBufferSubData(GL_UNIFORM_BUFFER, puiOffset, puiSize, pData);

		GLenum lglError = glGetError();
		if (lglError != GL_NO_ERROR)
		{
			LOG_DEBUG("glBufferSubData() failed: %d", lglError);
			lResult = EResult::UploadFailed;
		}

		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		return lResult;
	}

	EResult CGLUniformBuffer::Map(EMapFlags pMapFlags, uint32_t puiOffset, uint32_t puiSize, void ** pData)
	{
		FDX_ASSERT(muiHandle != 0, "Buffer not created");
		EMPTY_GL_ERROR();

		EResult lResult = EResult::Success;
		glBindBuffer(GL_UNIFORM_BUFFER, muiHandle);

		*pData = nullptr;
		*pData = glMapBufferRange(GL_UNIFORM_BUFFER, puiOffset, puiSize,
			CGLUtil::GetMapFlags(pMapFlags));

		GLenum lglError = glGetError();
		if (lglError != GL_NO_ERROR)
		{
			LOG_DEBUG("glMapBufferRange() failed: %d", lglError);
			lResult = EResult::MapFailed;
		}

		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		return lResult;
	}

	EResult CGLUniformBuffer::Unmap()
	{
		FDX_ASSERT(muiHandle != 0, "Buffer not created");
		EMPTY_GL_ERROR();

		EResult lResult = EResult::Success;

		glBindBuffer(GL_UNIFORM_BUFFER, muiHandle);
		glUnmapBuffer(GL_UNIFORM_BUFFER);

		GLenum lglError = glGetError();
		if (lglError != GL_NO_ERROR)
		{
			LOG_DEBUG("glUnmapBuffer() failed: %d", lglError);
			lResult = EResult::UnmapFailed;
		}

		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		return lResult;
	}
}