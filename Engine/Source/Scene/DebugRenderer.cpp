
#include "Scene/DebugRenderer.h"
#include "Graphics/VertexCacheManager.h"
#include "Graphics/ProgramManager.h"
#include "OS/GameLogger.h"
#include "Graphics/VertexTypes.h"

namespace FDX
{
	CDebugRenderer::CDebugRenderer()
	{
	}

	CDebugRenderer::~CDebugRenderer()
	{
	}

	EResult CDebugRenderer::Initialize()
	{
		EResult lResult = EResult::Success;

		lResult = CProgramManager::Get().GetProgram("DebugProgram", &mProgram);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CProgramManager::GetProgram() failed for DebugProgram");
		}

		return lResult;
	}

	EResult CDebugRenderer::Prepare()
	{
		EResult lResult = EResult::Success;

		IDrawOperation *lDrawOperation;
		Array<SVertexP> lVertices;

		for (const auto &lLine : mDebugLines)
		{
			lVertices.push_back(SVertexP(lLine.Start));
			lVertices.push_back(SVertexP(lLine.End));
		}

		lResult = CVertexCacheManager::Get().AddVertices("VertexP", lVertices.size(), 
			lVertices.data(), &lDrawOperation);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CVertexCacheManager::AddVertices() failed for DebugRenderer");
		}
		else
		{
			lDrawOperation->SetPrimitiveType(EPrimitiveType::LineList);
			mDrawOperation = IDrawOperationPtr(lDrawOperation);
		}

		return lResult;
	}

	EResult CDebugRenderer::Draw(CCamera * pCamera)
	{
		EResult lResult = EResult::Success;		

		if (pCamera)
		{
			mProgram->Bind();
			mProgram->GetConstants()->SetColor4("uDebugColor", mDebugColor);
			mProgram->GetConstants()->SetMatrix4("uMVP",
				pCamera->GetProjection() * pCamera->GetView());

			lResult = mDrawOperation->Draw(mDrawOperation->GetActiveVertexBuffer(), 
				mDrawOperation->GetActiveIndexBuffer());
		}

		return lResult;
	}

	void CDebugRenderer::AddAxisAlignedBox(const CAxisAlignedBox & pAabb)
	{
		auto lCorners = pAabb.GetCorners();

		AddLine(lCorners[1], lCorners[0]);
		AddLine(lCorners[2], lCorners[1]);
		AddLine(lCorners[3], lCorners[2]);
		AddLine(lCorners[0], lCorners[3]);

		AddLine(lCorners[4], lCorners[5]);
		AddLine(lCorners[5], lCorners[6]);
		AddLine(lCorners[6], lCorners[7]);
		AddLine(lCorners[7], lCorners[4]);

		AddLine(lCorners[0], lCorners[4]);
		AddLine(lCorners[3], lCorners[7]);
		AddLine(lCorners[1], lCorners[5]);
		AddLine(lCorners[2], lCorners[6]);
	}

	void CDebugRenderer::AddLine(const CLine & pLine)
	{
		mDebugLines.push_back(pLine);
	}

	void CDebugRenderer::AddLine(const CVector3F & pStart, const CVector3F & pEnd)
	{
		AddLine(CLine(pStart, pEnd));
	}

	void CDebugRenderer::SetDebugColor(const CColor4F & pDebugColor)
	{
		mDebugColor = pDebugColor;
	}
}
