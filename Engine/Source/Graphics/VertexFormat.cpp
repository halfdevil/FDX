
#include "Graphics/VertexFormat.h"

namespace FDX
{
	CVertexFormat::CVertexFormat()
		: muiSize(0), mbNeedsUpdate(true)
	{
	}

	CVertexFormat::~CVertexFormat()
	{
		Clear();
	}

	const CVertexElement & CVertexFormat::operator[](uint8_t puiIndex) const
	{
		return mElements[puiIndex];
	}

	void CVertexFormat::AddElement(const CVertexElement & pElement)
	{
		mElements.emplace_back(pElement);
		mbNeedsUpdate = true;
	}

	void CVertexFormat::AddElement(const String & pstrName, uint8_t puiIndex, 
		uint16_t puiOffset, EVertexElementFormat pFormat)
	{
		mElements.emplace_back(CVertexElement(pstrName, puiIndex, puiOffset, pFormat));
		mbNeedsUpdate = true;
	}

	void CVertexFormat::AddElementRange(const Array<CVertexElement>& pElements)
	{
		Array<CVertexElement>::const_iterator lIt = pElements.cbegin();

		while (lIt != pElements.cend())
		{
			mElements.emplace_back(*lIt);
			lIt++;
		}

		mbNeedsUpdate = true;
	}

	void CVertexFormat::AddElementRange(const CVertexElement * pElements, uint32_t puiNumElements)
	{
		for (uint32_t lIdx = 0; lIdx < puiNumElements; lIdx++)
			mElements.emplace_back(pElements[lIdx]);

		mbNeedsUpdate = true;
	}

	void CVertexFormat::Update()
	{
		if (!mbNeedsUpdate)
			return;

		muiSize = 0;
		mstrName.clear();

		Array<CVertexElement>::const_iterator lIt = mElements.cbegin();
		while (lIt != mElements.cend())
		{
			muiSize += (*lIt).GetSize();
			mstrName += (*lIt).GetName();

			lIt++;
		}

		mbNeedsUpdate = false;
	}

	void CVertexFormat::Clear()
	{
	}

	uint32_t CVertexElement::GetSize() const
	{
		switch (mFormat)
		{
		case EVertexElementFormat::Float1:
			return 4;

		case EVertexElementFormat::Float2:
			return 8;

		case EVertexElementFormat::Float3:
			return 12;

		case EVertexElementFormat::Float4:
			return 16;

		case EVertexElementFormat::HalfFloat2:
			return 4;

		case EVertexElementFormat::HalfFloat4:
			return 8;

		case EVertexElementFormat::Rgba32:
			return 4;

		case EVertexElementFormat::Rgba64:
			return 8;

		case EVertexElementFormat::Byte4:
			return 4;

		case EVertexElementFormat::Color:
			return 4;

		case EVertexElementFormat::Short2:
			return 4;

		case EVertexElementFormat::Short4:
			return 8;

		default:
			return 0;
		}

		return 0;
	}
}