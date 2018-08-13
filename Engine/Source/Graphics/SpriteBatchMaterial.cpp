
#include "Graphics/SpriteBatchMaterial.h"
#include "Graphics/ProgramManager.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CSpriteBatchMaterial::CSpriteBatchMaterial()
		: muiNumPasses(1),
		mbIsTransparent(false)
	{
	}

	CSpriteBatchMaterial::~CSpriteBatchMaterial()
	{
	}

	uint8_t CSpriteBatchMaterial::GetNumPasses() const
	{
		return muiNumPasses;
	}

	bool CSpriteBatchMaterial::UsesPass(EPassType pType)
	{
		return pType == EPassType::Diffuse;
	}

	bool CSpriteBatchMaterial::IsTransparent()
	{
		return mbIsTransparent;
	}

	void CSpriteBatchMaterial::SetTransparent(bool pbIsTransparent)
	{
		mbIsTransparent = pbIsTransparent;
	}

	void CSpriteBatchMaterial::SetCustomProgram(IProgram * pProgram)
	{
		mPrograms[EPassType::Diffuse] = pProgram;
	}

	void CSpriteBatchMaterial::SetTexture(ITexture * pTexture)
	{
		mTextures[EPassType::Diffuse][0] = pTexture;
	}

	EResult CSpriteBatchMaterial::Create()
	{
		EResult lResult = EResult::Success;
		IProgram *lProgram = nullptr;

		lResult = CProgramManager::Get().GetProgram("SpriteBatchProgram", &lProgram);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("GetProgram() failed for Sprite batch program");
		}
		else
		{
			mPrograms[EPassType::Diffuse] = lProgram;
			mTextures[EPassType::Diffuse].push_back(nullptr);
		}

		return lResult;
	}
}