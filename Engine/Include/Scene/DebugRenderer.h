#pragma once

#include "Core/Result.h"
#include "Core/Singleton.h"
#include "Math/Line.h"
#include "Math/AxisAlignedBox.h"
#include "Scene/Camera.h"
#include "Math/Color4F.h"
#include "Graphics/Program.h"
#include "Graphics/DrawOperation.h"

namespace FDX
{
	class FDX_EXPORT CDebugRenderer : public CSingleton<CDebugRenderer>
	{
	public:

		CDebugRenderer();
		~CDebugRenderer();

		const CColor4F& GetDebugColor() const
		{
			return mDebugColor;
		}
		
		EResult Initialize();		
		EResult Prepare();
		EResult Draw(CCamera *pCamera);

		void AddAxisAlignedBox(const CAxisAlignedBox &pAabb);
		void AddLine(const CLine &pLine);
		void AddLine(const CVector3F &pStart, const CVector3F &pEnd);
		void SetDebugColor(const CColor4F &pDebugColor);

	private:

		CColor4F mDebugColor;
		IProgram *mProgram;
		IDrawOperationPtr mDrawOperation;
		Array<CLine> mDebugLines;
	};

	using CDebugRendererPtr = SmartPtr<CDebugRenderer>;
}