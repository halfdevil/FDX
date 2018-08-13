#pragma once

#include "Graphics/Buffer.h"

namespace FDX
{
	class FDX_EXPORT CUniformBuffer : public IBuffer
	{
	public:

		virtual ~CUniformBuffer()
		{
		}

		virtual uint32_t GetBindingIndex() const = 0;
		virtual void SetBindingIndex(uint32_t puiIndex) = 0;
	};

	using CUniformBufferPtr = SmartPtr<CUniformBuffer>;
}
