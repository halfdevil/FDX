#pragma once

#include "Graphics/DrawOperation.h"
#include "OGL/GLRenderData.h"

namespace FDX
{
	class CGLDrawOperation : public IDrawOperation
	{
	public:

		CGLDrawOperation(CGLRenderData *pRenderData);
		~CGLDrawOperation();

		IRenderData * GetRenderData() const override
		{
			return mRenderData;
		}

		EPrimitiveType GetPrimitiveType() const override
		{
			return mPrimitiveType;
		}

		uint32_t GetStartVertex() const override
		{
			return muiStartVertex;
		}

		uint32_t GetStartIndex() const override
		{
			return muiStartIndex;
		}

		uint32_t GetNumVertices() const override
		{
			return muiNumVertices;
		}

		uint32_t GetNumIndices() const override
		{
			return muiNumIndices;
		}

		uint32_t GetNumPatchVertices() const override
		{
			return muiNumPatchVertices;
		}

		float GetLineWidth() const override
		{
			return mfLineWidth;
		}

		int32_t GetActiveVertexBuffer() const override
		{
			return miActiveVertexBuffer;
		}

		int32_t GetActiveIndexBuffer() const override
		{
			return miActiveIndexBuffer;
		}

		void SetStartVertex(uint32_t puiIndex) override;
		void SetStartIndex(uint32_t puiIndex) override;
		void SetNumVertices(uint32_t puiNumVertices) override;
		void SetNumIndices(uint32_t puiNumIndices) override;
		void SetLineWidth(float pfWidth) override;
		void SetNumPatchVertices(uint32_t puiNumVertices) override;
		void SetPrimitiveType(EPrimitiveType pType) override;
		void SetActiveVertexBuffer(int32_t puiId) override;
		void SetActiveIndexBuffer(int32_t puiId) override;

		EResult Draw() override;
		EResult Draw(int32_t piActiveVertexBuffer, 
			int32_t piActiveIndexBuffer) override;

	private:

		CGLRenderData *mRenderData;
		EPrimitiveType mPrimitiveType;
		GLenum mGLPrimitiveType;
		uint32_t muiStartVertex;
		uint32_t muiStartIndex;
		uint32_t muiNumVertices;
		uint32_t muiNumIndices;
		uint32_t muiNumPatchVertices;
		float mfLineWidth;
		int32_t miActiveVertexBuffer;
		int32_t miActiveIndexBuffer;
	};
}