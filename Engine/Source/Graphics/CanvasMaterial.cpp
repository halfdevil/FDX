
#include "Graphics/CanvasMaterial.h"
#include "Graphics/ProgramManager.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CCanvasMaterial::CCanvasMaterial()
	{
	}

	CCanvasMaterial::~CCanvasMaterial()
	{
	}

	EResult CCanvasMaterial::Create()
	{
		EResult lResult = EResult::Success;
		IProgram *lProgram = nullptr;

		lResult = CProgramManager::Get().GetProgram("CanvasProgram", &lProgram);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("GetProgram() failed for Canvas program");
		}
		else
		{
			mPrograms[EPassType::Diffuse] = lProgram;
			mTextures[EPassType::Diffuse].push_back(nullptr);
		}

		return lResult;
	}
}