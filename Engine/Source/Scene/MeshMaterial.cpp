
#include "Scene/MeshMaterial.h"
#include "Graphics/ProgramManager.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CMeshMaterial::CMeshMaterial()
		: mbIsTransparent(false),
		muiNumPasses(1)
	{
	}

	CMeshMaterial::~CMeshMaterial()
	{
	}

	uint8_t CMeshMaterial::GetNumPasses() const
	{
		return muiNumPasses;
	}

	bool CMeshMaterial::UsesPass(EPassType pType)
	{
		return pType == EPassType::Diffuse || 
			pType == EPassType::Light ||
			pType == EPassType::Shadow;
	}

	bool CMeshMaterial::IsTransparent()
	{
		return mbIsTransparent;
	}

	EResult CMeshMaterial::Create()
	{
		EResult lResult = EResult::Success;
		IProgram *lProgram = nullptr;

		lResult = CProgramManager::Get().GetProgram("ModelDiffuseProgram", &lProgram);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("GetProgram() failed for Mesh program");
		}
		else
		{
			mPrograms[EPassType::Diffuse] = lProgram;

			lResult = CProgramManager::Get().GetProgram("ModelLightProgram", &lProgram);
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("GetProgram() failed for Mesh program");
			}
			else
			{
				mPrograms[EPassType::Light] = lProgram;

				lResult = CProgramManager::Get().GetProgram("ModelShadowProgram", &lProgram);
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("GetProgram() failed for Mesh program");
				}
				else
				{
					mPrograms[EPassType::Shadow] = lProgram;
				}
			}
		}

		return lResult;
	}

	void CMeshMaterial::SetTransparent(bool pbIsTransparent)
	{
		mbIsTransparent = pbIsTransparent;
	}
}