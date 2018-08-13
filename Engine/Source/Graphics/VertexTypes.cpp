
#include "Graphics/VertexTypes.h"

namespace FDX
{
	const uint32_t SVertexP::NumElements = 1;
	const uint32_t SVertexPC::NumElements = 2;
	const uint32_t SVertexPCT::NumElements = 3;
	const uint32_t SVertexPT::NumElements = 2;
	const uint32_t SVertexPNT::NumElements = 3;
	const uint32_t SVertexCanvas::NumElements = 2;
	const uint32_t SModelVertex::NumElements = 7;

	const CVertexElement SVertexP::Elements[1] =
	{
		CVertexElement("vPosition", 0, 0, EVertexElementFormat::Float3)
	};

	const CVertexElement SVertexPC::Elements[2] =
	{
		CVertexElement("vPosition", 0, 0, EVertexElementFormat::Float3),
		CVertexElement("vColor", 1, 12, EVertexElementFormat::Float4)
	};

	const CVertexElement SVertexPCT::Elements[3] =
	{
		CVertexElement("vPosition", 0, 0, EVertexElementFormat::Float3),
		CVertexElement("vColor", 1, 12, EVertexElementFormat::Float4),
		CVertexElement("vTexCoord", 2, 28, EVertexElementFormat::Float2)
	};

	const CVertexElement SVertexPT::Elements[2] =
	{
		CVertexElement("vPosition", 0, 0, EVertexElementFormat::Float3),
		CVertexElement("vTexCoord", 1, 12, EVertexElementFormat::Float2)
	};

	const CVertexElement SVertexPNT::Elements[3] =
	{
		CVertexElement("vPosition", 0, 0, EVertexElementFormat::Float3),
		CVertexElement("vNormal", 1, 12, EVertexElementFormat::Float3),
		CVertexElement("vTexCoord", 2, 24, EVertexElementFormat::Float2)
	};

	const CVertexElement SVertexCanvas::Elements[2] =
	{
		CVertexElement("vPosition", 0, 0, EVertexElementFormat::Float4),
		CVertexElement("vTexCoord", 1, 16, EVertexElementFormat::Float2)
	};
	
	const CVertexElement SModelVertex::Elements[7] =
	{
		CVertexElement("vPosition", 0, 0, EVertexElementFormat::Float3),
		CVertexElement("vNormal", 1, 12, EVertexElementFormat::Float3),
		CVertexElement("vTexCoord1", 2, 24, EVertexElementFormat::Float2),
		CVertexElement("vTexCoord2", 3, 32, EVertexElementFormat::Float2),
		CVertexElement("vTexCoord3", 4, 40, EVertexElementFormat::Float2),
		CVertexElement("vBlendIndices", 5, 48, EVertexElementFormat::Float4),
		CVertexElement("vBlendWeights", 6, 64, EVertexElementFormat::Float4)
	};
}