#pragma once

#include "Core/Core.h"

namespace FDX
{
	enum class EVertexElementFormat
	{
		Float1,
		Float2,
		Float3,
		Float4,
		HalfFloat2,
		HalfFloat4,
		Rgba64,
		Color,
		Rgba32,
		Short2,
		Short4,
		Byte4,
	};

	class FDX_EXPORT CVertexElement
	{
	public:

		CVertexElement(const String &pstrName,
			uint8_t puiIndex,
			uint16_t puiOffset,
			EVertexElementFormat pFormat) : mstrName(pstrName),
			muiIndex(puiIndex),
			muiOffset(puiOffset),
			mFormat(pFormat)
		{
		}

		const String& GetName() const
		{
			return mstrName;
		}

		uint8_t GetIndex() const
		{
			return muiIndex;
		}

		uint16_t GetOffset() const
		{
			return muiOffset;
		}

		EVertexElementFormat GetFormat() const
		{
			return mFormat;
		}

		uint32_t GetSize() const;

	private:

		String mstrName;
		uint8_t muiIndex;
		uint16_t muiOffset;
		EVertexElementFormat mFormat;
	};

	class FDX_EXPORT CVertexFormat
	{
	public:

		CVertexFormat();
		~CVertexFormat();

		const String& GetName() const
		{
			return mstrName;
		}

		uint32_t GetSize() const
		{
			if (mbNeedsUpdate)
				const_cast<CVertexFormat*>(this)->Update();

			return muiSize;
		}

		const Array<CVertexElement>& GetElements() const
		{
			return mElements;
		}

		uint32_t GetNumElements() const
		{
			return mElements.size();
		}

		const CVertexElement& operator[](uint8_t puiIndex) const;

		void AddElement(const CVertexElement &pElement);
		void AddElement(const String &pstrName,
			uint8_t puiIndex,
			uint16_t puiOffset,
			EVertexElementFormat pFormat);

		void AddElementRange(const Array<CVertexElement> &pElements);
		void AddElementRange(const CVertexElement *pElements, uint32_t puiNumElements);
		void Update();
		void Clear();

	private:

		String mstrName;
		uint32_t muiSize;
		Array<CVertexElement> mElements;
		bool mbNeedsUpdate;
	};

	using CVertexFormatPtr = SmartPtr<CVertexFormat>;
}
