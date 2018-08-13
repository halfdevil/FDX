
#include "OGL/GLRenderData.h"
#include "OS/GameLogger.h"
#include "OGL/GLUtil.h"
#include "OGL/GLGraphicsDevice.h"
#include "OS/System.h"

namespace FDX
{
	CGLRenderData::CGLRenderData(const String &pstrName, CGLGraphicsDevice *pGraphicsDevice)
		: mstrName(pstrName),
		muiHandle(0),
		mGraphicsDevice(pGraphicsDevice)
	{
	}

	CGLRenderData::~CGLRenderData()
	{
		mVertexBuffers.clear();
		mIndexBuffers.clear();

		if (muiHandle != 0)
			glDeleteVertexArrays(1, &muiHandle);
	}

	CVertexBuffer * CGLRenderData::GetVertexBuffer(uint32_t puiId) const
	{
		if (puiId < mVertexBuffers.size())
			return mVertexBuffers[puiId].get();

		return nullptr;
	}

	CIndexBuffer * CGLRenderData::GetIndexBuffer(uint32_t puiId) const
	{
		if (puiId < mIndexBuffers.size())
			return mIndexBuffers[puiId].get();

		return nullptr;
	}

	EResult CGLRenderData::Create()
	{
		EResult lResult = EResult::Success;
		GLenum lglError = GL_NO_ERROR;

		glGenVertexArrays(1, &muiHandle);
		
		lglError = glGetError();
		if (lglError != GL_NO_ERROR)
		{
			LOG_DEBUG("glGenVertexArrays() failed: %d", lglError);
			lResult = EResult::CreateFailed;
		}

		return lResult;
	}

	EResult CGLRenderData::AddVertexBuffer(const String & pstrName, EBufferUsage pUsage, 
		CVertexFormat * pFormat, uint32_t puiNumVertices, int32_t * puiId)
	{
		FDX_ASSERT(muiHandle != 0, "Vertex array not created");

		EResult lResult = EResult::Success;
		CVertexBuffer *lBuffer = nullptr;
		
		glBindVertexArray(muiHandle);

		lResult = mGraphicsDevice->CreateVertexBuffer(pstrName, pUsage,
			pFormat, puiNumVertices, &lBuffer);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CreateVertexBuffer() failed");			
		}
		else
		{
			mVertexBuffers.emplace_back(CVertexBufferPtr(lBuffer));
			*puiId = mVertexBuffers.size() - 1;
		}

		glBindVertexArray(0);

		return lResult;
	}

	EResult CGLRenderData::AddIndexBuffer(const String & pstrName, EBufferUsage pUsage, 
		EIndexType pType, uint32_t puiNumIndices, int32_t * puiId)
	{
		FDX_ASSERT(muiHandle != 0, "Vertex array not created");

		EResult lResult = EResult::Success;
		CIndexBuffer *lBuffer = nullptr;

		glBindVertexArray(muiHandle);

		lResult = mGraphicsDevice->CreateIndexBuffer(pstrName, pUsage,
			pType, puiNumIndices, &lBuffer);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CreateIndexBuffer() failed");
		}
		else
		{
			mIndexBuffers.emplace_back(CIndexBufferPtr(lBuffer));
			*puiId = mIndexBuffers.size() - 1;
		}

		glBindVertexArray(0);

		return lResult;
	}

	EResult CGLRenderData::Activate(int32_t puiVertexBufferId, int32_t puiIndexBufferId)
	{
		FDX_ASSERT(muiHandle != 0, "Vertex array not created");

		EResult lResult = EResult::Success;
		GLenum lGLError = GL_NO_ERROR;

		EMPTY_GL_ERROR();				
		glBindVertexArray(muiHandle);

		lGLError = glGetError();
		if (lGLError != GL_NO_ERROR)
		{
			LOG_DEBUG("glBindVertexArray() failed: %d", lGLError);
		}
		else
		{
			if (puiVertexBufferId >= 0 && puiVertexBufferId < (int32_t)mVertexBuffers.size())
			{
				glBindBuffer(GL_ARRAY_BUFFER,
					dynamic_cast<CGLVertexBuffer*>(mVertexBuffers[puiVertexBufferId].get())->GetHandle());

				lGLError = glGetError();
				if (lGLError != GL_NO_ERROR)
				{
					LOG_DEBUG("glBindBuffer(GL_ARRAY_BUFFER) failed: %d", lGLError);
				}
			}

			if (puiIndexBufferId >= 0 && puiIndexBufferId < (int32_t)mIndexBuffers.size())
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
					dynamic_cast<CGLIndexBuffer*>(mIndexBuffers[puiIndexBufferId].get())->GetHandle());

				lGLError = glGetError();
				if (lGLError != GL_NO_ERROR)
				{
					LOG_DEBUG("glBindBuffer(GL_ELEMENT_ARRAY_BUFFER) failed: %d", lGLError);
				}
			}
		}

		return lResult;
	}
}