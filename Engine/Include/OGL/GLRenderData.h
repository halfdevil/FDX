#pragma once

#include "Graphics/RenderData.h"
#include "OGL/GLVertexBuffer.h"
#include "OGL/GLIndexBuffer.h"
#include <GL/gl3w.h>

namespace FDX
{
	class CGLGraphicsDevice;

	class CGLRenderData : public IRenderData
	{
	public:

		CGLRenderData(const String &pstrName, CGLGraphicsDevice *pGraphicsDevice);
		~CGLRenderData();
		
		const String & GetName() const override
		{
			return mstrName;
		}

		CVertexBuffer* GetVertexBuffer(uint32_t puiId) const;
		CIndexBuffer* GetIndexBuffer(uint32_t puiId) const;

		EResult Create();		

		EResult AddVertexBuffer(const String &pstrName, EBufferUsage pUsage, 
			CVertexFormat *pFormat,	uint32_t puiNumVertices, int32_t *puiId) override;

		EResult AddIndexBuffer(const String &pstrName, EBufferUsage pUsage, 
			EIndexType pType, uint32_t puiNumIndices, int32_t *puiId) override;

		EResult Activate(int32_t puiVertexBufferId,
			int32_t puiIndexBufferId);

	private:

		String mstrName;
		GLuint muiHandle;
		CGLGraphicsDevice *mGraphicsDevice;
		Array<CVertexBufferPtr> mVertexBuffers;
		Array<CIndexBufferPtr> mIndexBuffers;
	};
}