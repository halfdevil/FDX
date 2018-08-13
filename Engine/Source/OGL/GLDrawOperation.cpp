
#include "OGL/GLDrawOperation.h"
#include "OGL/GLUtil.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CGLDrawOperation::CGLDrawOperation(CGLRenderData * pRenderData)
		: mRenderData(pRenderData),
		muiStartVertex(0),
		muiStartIndex(0),
		muiNumVertices(0),
		muiNumIndices(0),
		muiNumPatchVertices(0),
		mfLineWidth(1.0f),
		miActiveVertexBuffer(-1),
		miActiveIndexBuffer(-1)
	{
	}

	CGLDrawOperation::~CGLDrawOperation()
	{
	}

	void CGLDrawOperation::SetStartVertex(uint32_t puiIndex)
	{
		muiStartVertex = puiIndex;
	}

	void CGLDrawOperation::SetStartIndex(uint32_t puiIndex)
	{
		muiStartIndex = puiIndex;
	}

	void CGLDrawOperation::SetNumVertices(uint32_t puiNumVertices)
	{
		muiNumVertices = puiNumVertices;
	}

	void CGLDrawOperation::SetNumIndices(uint32_t puiNumIndices)
	{
		muiNumIndices = puiNumIndices;
	}

	void CGLDrawOperation::SetLineWidth(float pfWidth)
	{
		if (pfWidth > 0.0f)
		{
			mfLineWidth = pfWidth;
			glLineWidth(mfLineWidth);
		}
	}

	void CGLDrawOperation::SetNumPatchVertices(uint32_t puiNumVertices)
	{
		muiNumPatchVertices = puiNumVertices;
		glPatchParameteri(GL_PATCH_VERTICES, puiNumVertices);
	}

	void CGLDrawOperation::SetPrimitiveType(EPrimitiveType pType)
	{
		mPrimitiveType = pType;
		mGLPrimitiveType = CGLUtil::GetPrimitiveType(pType);
	}

	void CGLDrawOperation::SetActiveVertexBuffer(int32_t puiId)
	{
		miActiveVertexBuffer = puiId;
	}

	void CGLDrawOperation::SetActiveIndexBuffer(int32_t puiId)
	{
		miActiveIndexBuffer = puiId;
	}

	EResult CGLDrawOperation::Draw()
	{
		EResult lResult = EResult::Success;
		
		if (miActiveIndexBuffer != -1)
		{
			CIndexBuffer *lIndexBuffer = mRenderData->GetIndexBuffer(miActiveIndexBuffer);

			if (mPrimitiveType == EPrimitiveType::PointList)
			{
				glDrawArrays(mGLPrimitiveType, muiStartVertex,
					muiNumVertices);
			}
			else
			{
				GLenum lGLIndexType = (lIndexBuffer->GetIndexType() == EIndexType::SixteenBit) ?
					GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;

				uint32_t luiIndexOffset = muiStartIndex *
					(lIndexBuffer->GetIndexType() == EIndexType::SixteenBit ? 2 : 4);

				glDrawElementsBaseVertex(mGLPrimitiveType,
					muiNumIndices,
					lGLIndexType,
					reinterpret_cast<void*>(static_cast<uint64_t>(luiIndexOffset)),
					muiStartVertex);
			}
		}
		else
		{
			glDrawArrays(mGLPrimitiveType, muiStartVertex,
				muiNumVertices);
		}

		return lResult;
	}

	EResult CGLDrawOperation::Draw(int32_t piActiveVertexBuffer, int32_t piActiveIndexBuffer)
	{
		EResult lResult = EResult::Success;

		lResult = mRenderData->Activate(piActiveVertexBuffer, 
			piActiveIndexBuffer);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("RenderData Activate() failed");
		}
		else
		{
			if (piActiveIndexBuffer != -1)
			{
				CIndexBuffer *lIndexBuffer = mRenderData->GetIndexBuffer(piActiveIndexBuffer);

				if (mPrimitiveType == EPrimitiveType::PointList)
				{
					glDrawArrays(mGLPrimitiveType, muiStartVertex,
						muiNumVertices);
				}
				else
				{
					GLenum lGLIndexType = (lIndexBuffer->GetIndexType() == EIndexType::SixteenBit) ?
						GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;

					uint32_t luiIndexOffset = muiStartIndex *
						(lIndexBuffer->GetIndexType() == EIndexType::SixteenBit ? 2 : 4);

					glDrawElementsBaseVertex(mGLPrimitiveType,
						muiNumIndices,
						lGLIndexType,
						reinterpret_cast<void*>(static_cast<uint64_t>(luiIndexOffset)), 
						muiStartVertex);
				}
			}
			else
			{
				glDrawArrays(mGLPrimitiveType, muiStartVertex,
					muiNumVertices);
			}
		}

		return lResult;
	}
}