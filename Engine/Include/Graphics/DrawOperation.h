#pragma once

#include "Graphics/RenderData.h"

namespace FDX
{
	enum class EPrimitiveType
	{
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		TriangleStrip,
		TriangleFan,
		LineListAdjacency,
		LineStripAdjacency,
		TriangleListAdjacency,
		TriangleStripAdjacency,
		Patches
	};

	class FDX_EXPORT IDrawOperation
	{
	public:

		virtual ~IDrawOperation()
		{
		}

		virtual IRenderData* GetRenderData() const = 0;
		virtual EPrimitiveType GetPrimitiveType() const = 0;

		virtual uint32_t GetStartVertex() const = 0;
		virtual uint32_t GetStartIndex() const = 0;
		virtual uint32_t GetNumVertices() const = 0;
		virtual uint32_t GetNumIndices() const = 0;
		virtual uint32_t GetNumPatchVertices() const = 0;
		virtual float GetLineWidth() const = 0;

		virtual int32_t GetActiveVertexBuffer() const = 0;
		virtual int32_t GetActiveIndexBuffer() const = 0;		

		virtual void SetStartVertex(uint32_t puiIndex) = 0;
		virtual void SetStartIndex(uint32_t puiIndex) = 0;
		virtual void SetNumVertices(uint32_t puiNumVertices) = 0;
		virtual void SetNumIndices(uint32_t puiNumIndices) = 0;
		virtual void SetNumPatchVertices(uint32_t puiNumVertices) = 0;
		virtual void SetLineWidth(float pfWidth) = 0;
		virtual void SetPrimitiveType(EPrimitiveType pType) = 0;

		virtual void SetActiveVertexBuffer(int32_t puiId) = 0;
		virtual void SetActiveIndexBuffer(int32_t puiId) = 0;

		virtual EResult Draw() = 0;
		virtual EResult Draw(int32_t piActiveVertexBuffer, 
			int32_t piActiveIndexBuffer) = 0;
	};

	using IDrawOperationPtr = SmartPtr<IDrawOperation>;
}
