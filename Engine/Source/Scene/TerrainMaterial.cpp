

#include "Scene/TerrainMaterial.h"
#include "Graphics/ProgramManager.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CTerrainMaterial::CTerrainMaterial()
		: muiNumPasses(1)
	{
	}

	CTerrainMaterial::~CTerrainMaterial()
	{
	}

	uint8_t CTerrainMaterial::GetNumPasses() const
	{
		return muiNumPasses;
	}

	bool CTerrainMaterial::UsesPass(EPassType pType)
	{
		return pType == EPassType::Diffuse ||
			pType == EPassType::Light;
	}

	bool CTerrainMaterial::IsTransparent()
	{
		return false;
	}

	EResult CTerrainMaterial::Create()
	{
		EResult lResult = EResult::Success;
		IProgram *lProgram = nullptr;

		lResult = CProgramManager::Get().GetProgram("TerrainDiffuseProgram", &lProgram);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("GetProgram() failed for Terrain");
		}
		else
		{
			mPrograms[EPassType::Diffuse] = lProgram;

			lResult = CProgramManager::Get().GetProgram("TerrainLightProgram", &lProgram);
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("GetProgram() failed for Terrain");
			}
			else
			{
				mPrograms[EPassType::Light] = lProgram;
			}
		}

		return lResult;
	}
}