#pragma once

#include "Graphics/Buffer.h"

namespace FDX
{
	enum class EIndexType
	{
		SixteenBit,
		ThirtyTwoBit
	};

	class FDX_EXPORT CIndexBuffer : public IBuffer
	{
	public:

		virtual ~CIndexBuffer()
		{
		}

		virtual uint32_t GetNumIndices() const = 0;
		virtual EIndexType GetIndexType() const = 0;
	};

	using CIndexBufferPtr = SmartPtr<CIndexBuffer>;
}