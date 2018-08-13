
#include "OGL/GLIndexBuffer.h"
#include "OGL/GLUtil.h"
#include "OS/GameLogger.h"
#include "OS/System.h"

namespace FDX
{
	CGLIndexBuffer::CGLIndexBuffer(const String & pstrName)
		: mstrName(pstrName),
		muiNumIndices(0),
		muiHandle(0)
	{
	}

	CGLIndexBuffer::~CGLIndexBuffer()
	{
		if (muiHandle != 0)
			glDeleteBuffers(1, &muiHandle);
	}

	EResult CGLIndexBuffer::Create(EBufferUsage pUsage, EIndexType pIndexType, uint32_t puiNumIndices)
	{
		FDX_ASSERT(muiHandle == 0, "Buffer already created");
		FDX_ASSERT(puiNumIndices != 0, "Number of indices cannot be zero");

		EResult lResult = EResult::Success;

		glGenBuffers(1, &muiHandle);
		if (!muiHandle)
		{
			LOG_DEBUG("glGenBuffers() failed: %d", glGetError());
			lResult = EResult::CreateFailed;
		}
		else
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, muiHandle);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, ((pIndexType == EIndexType::SixteenBit) ? 2 : 4) * puiNumIndices,
				nullptr, CGLUtil::GetBufferUsage(pUsage));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			mIndexType = pIndexType;
			mUsage = pUsage;
			muiNumIndices = puiNumIndices;
		}

		return lResult;
	}

	EResult CGLIndexBuffer::UploadData(uint32_t puiOffset, uint32_t puiSize, const void * pData)
	{
		FDX_ASSERT(muiHandle != 0, "Buffer not created");

		EResult lResult = EResult::Success;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, muiHandle);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, puiOffset, puiSize, pData);

		GLenum lglError = glGetError();
		if (lglError != GL_NO_ERROR)
		{
			LOG_DEBUG("glBufferSubData() failed: %d", lglError);
			lResult = EResult::UploadFailed;
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return lResult;
	}

	EResult CGLIndexBuffer::Map(EMapFlags pMapFlags, uint32_t puiOffset, uint32_t puiSize, void ** pData)
	{
		FDX_ASSERT(muiHandle != 0, "Buffer not created");

		EResult lResult = EResult::Success;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, muiHandle);

		*pData = nullptr;
		*pData = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, puiOffset, puiSize,
			CGLUtil::GetMapFlags(pMapFlags));

		GLenum lglError = glGetError();
		if (lglError != GL_NO_ERROR)
		{
			LOG_DEBUG("glMapBufferRange() failed: %d", lglError);
			lResult = EResult::MapFailed;
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return lResult;
	}

	EResult CGLIndexBuffer::Unmap()
	{
		FDX_ASSERT(muiHandle != 0, "Buffer not created");

		EResult lResult = EResult::Success;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, muiHandle);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

		GLenum lglError = glGetError();
		if (lglError != GL_NO_ERROR)
		{
			LOG_DEBUG("glUnmapBuffer() failed: %d", lglError);
			lResult = EResult::UnmapFailed;
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return lResult;
	}
}