
#include "OGL/GLVertexBuffer.h"
#include "OS/GameLogger.h"
#include "OGL/GLUtil.h"
#include "OS/System.h"

namespace FDX
{
	CGLVertexBuffer::CGLVertexBuffer(const String & pstrName)
		: mstrName(pstrName),
		muiNumVertices(0),
		mFormat(nullptr),
		muiHandle(0)
	{
	}

	CGLVertexBuffer::~CGLVertexBuffer()
	{
		if (muiHandle != 0)
			glDeleteBuffers(1, &muiHandle);
	}

	EResult CGLVertexBuffer::Create(EBufferUsage pUsage, CVertexFormat * pFormat, uint32_t puiNumVertices)
	{
		FDX_ASSERT(muiHandle == 0, "Buffer already created");
		FDX_ASSERT(puiNumVertices != 0, "Number of vertices cannot be zero");

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
			glBindBuffer(GL_ARRAY_BUFFER, muiHandle);

			for (const auto &lElement : pFormat->GetElements())
			{
				glVertexAttribPointer(lElement.GetIndex(),
					lElement.GetSize() / 4,
					CGLUtil::GetVertexElementFormat(lElement.GetFormat()),
					GL_FALSE,					
					pFormat->GetSize(),
					(const GLvoid*)lElement.GetOffset());

				glEnableVertexAttribArray(lElement.GetIndex());
			}			

			glBufferData(GL_ARRAY_BUFFER, pFormat->GetSize() * puiNumVertices, 
				nullptr, CGLUtil::GetBufferUsage(pUsage));

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			mFormat = pFormat;
			mUsage = pUsage;
			muiNumVertices = puiNumVertices;
		}

		return lResult;
	}

	EResult CGLVertexBuffer::UploadData(uint32_t puiOffset, uint32_t puiSize, const void * pData)
	{
		FDX_ASSERT(muiHandle != 0, "Buffer not created");

		EResult lResult = EResult::Success;

		EMPTY_GL_ERROR();

		glBindBuffer(GL_ARRAY_BUFFER, muiHandle);
		glBufferSubData(GL_ARRAY_BUFFER, puiOffset, puiSize, pData);

		GLenum lglError = glGetError();
		if (lglError != GL_NO_ERROR)
		{
			LOG_DEBUG("glBufferSubData() failed: %d", lglError);
			lResult = EResult::UploadFailed;
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return lResult;
	}

	EResult CGLVertexBuffer::Map(EMapFlags pMapFlags, uint32_t puiOffset, uint32_t puiSize, void ** pData)
	{
		FDX_ASSERT(muiHandle != 0, "Buffer not created");

		EMPTY_GL_ERROR();

		EResult lResult = EResult::Success;		
		glBindBuffer(GL_ARRAY_BUFFER, muiHandle);

		*pData = nullptr;
		*pData = glMapBufferRange(GL_ARRAY_BUFFER, puiOffset, puiSize, 
			CGLUtil::GetMapFlags(pMapFlags));

		GLenum lglError = glGetError();
		if (lglError != GL_NO_ERROR)
		{
			LOG_DEBUG("glMapBufferRange() failed: %d", lglError);
			lResult = EResult::MapFailed;
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return lResult;
	}

	EResult CGLVertexBuffer::Unmap()
	{
		FDX_ASSERT(muiHandle != 0, "Buffer not created");

		EMPTY_GL_ERROR();

		EResult lResult = EResult::Success;

		glBindBuffer(GL_ARRAY_BUFFER, muiHandle);
		glUnmapBuffer(GL_ARRAY_BUFFER);

		GLenum lglError = glGetError();
		if (lglError != GL_NO_ERROR)
		{
			LOG_DEBUG("glUnmapBuffer() failed: %d", lglError);
			lResult = EResult::UnmapFailed;
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return lResult;
	}
}