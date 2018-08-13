#pragma once

#include "Graphics/UniformBuffer.h"
#include <GL/gl3w.h>

namespace FDX
{
	class FDX_EXPORT CGLUniformBuffer : public CUniformBuffer
	{
	public:

		CGLUniformBuffer(const String &pstrName);
		~CGLUniformBuffer();

		GLuint GetHandle() const
		{
			return muiHandle;
		}

		const String& GetName() const override
		{
			return mstrName;
		}

		EBufferUsage GetUsage()
		{
			return mUsage;
		}

		uint32_t GetSize() const
		{
			return muiSize;
		}

		uint32_t GetBindingIndex() const override
		{
			return muiBindingIndex;
		}

		void SetBindingIndex(uint32_t puiIndex) override;

		EResult Create(EBufferUsage pUsage, uint32_t puiSize, uint32_t puiBindingIndex);
		EResult UploadData(uint32_t puiOffset, uint32_t puiSize,
			const void *pData) override;

		EResult Map(EMapFlags pMapFlags, uint32_t puiOffset,
			uint32_t puiSize, void **pData) override;
		EResult Unmap() override;

	private:

		String mstrName;
		GLuint muiHandle;
		EBufferUsage mUsage;
		uint32_t muiBindingIndex;	
		uint32_t muiSize;
	};
}