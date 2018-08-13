
#include "Graphics/ProgramManager.h"
#include "Graphics/VertexCacheManager.h"
#include "Core/ResourceManager.h"
#include "OS/GameLogger.h"
#include "OS/System.h"

namespace FDX
{
	CProgramManager::CProgramManager(IGraphicsDevice * pGraphicsDevice)
		: mGraphicsDevice(pGraphicsDevice)
	{
	}

	CProgramManager::~CProgramManager()
	{
		RemoveAll();
	}

	EResult CProgramManager::GetProgram(const String & pstrName, IProgram ** pProgram)
	{
		EResult lResult = EResult::Success;

		auto lIt = mPrograms.find(pstrName);
		if (lIt != mPrograms.end())
		{
			*pProgram = (*lIt).second.get();
		}
		else
		{		
			lResult = EResult::NotFound;
		}

		return lResult;
	}

	EResult CProgramManager::AddProgram(const String & pstrName, const ProgramShaders &mShaders,
		CVertexFormat *pFormat, IProgram ** pProgram)
	{
		EResult lResult = EResult::Success;

		auto lIt = mPrograms.find(pstrName);
		if (lIt != mPrograms.end())
		{
			*pProgram = (*lIt).second.get();
		}
		else
		{
			IProgram *lProgram = nullptr;
			lResult = mGraphicsDevice->CreateProgram(pstrName, pFormat, &lProgram);

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CreateProgram() failed");
			}
			else
			{
				auto lIt2 = mShaders.begin();
				while (lIt2 != mShaders.end())
				{
					CBinaryResource *lResource = nullptr;
					lResult = CResourceManager::Get().LoadResource(EResourceType::Binary, (*lIt2).second, &lResource);

					if (lResult != EResult::Success)
					{
						LOG_DEBUG("GetResource() failed");
						break;
					}
					else
					{
						lResult = lProgram->LoadShader((*lIt2).first, lResource);
						if (lResult != EResult::Success)
						{
							LOG_DEBUG("LoadShader() failed");
							break;
						}
					}

					lIt2++;
				}

				if (lResult == EResult::Success)
				{
					lResult = lProgram->Build();
					if (lResult != EResult::Success)
					{
						LOG_DEBUG("ProgramBuild() failed");
					}
				}

				if (lResult != EResult::Success)
				{
					SAFE_DELETE(lProgram);
				}
				else
				{
					*pProgram = lProgram;
					mPrograms[pstrName] = IProgramPtr(lProgram);					
				}
			}
		}

		return lResult;
	}

	EResult CProgramManager::AddProgram(const String & pstrName, const CProgramParameters & pParameters, 
		IProgram ** pProgram)
	{
		EResult lResult = EResult::Success;
		CVertexFormat *lVertexFormat = nullptr;

		lResult = CVertexCacheManager::Get().GetVertexFormat(pParameters.GetVertexFormat(), &lVertexFormat);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("GetVertexFormat() failed for: %s", pParameters.GetVertexFormat().c_str());
		}
		else
		{
			lResult = AddProgram(pstrName, pParameters.GetShaders(), 
				lVertexFormat, pProgram);

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("AddProgram() failed for %s", pstrName.c_str());
			}
		}

		return lResult;
	}

	EResult CProgramManager::RemoveProgram(const String & pstrName)
	{
		EResult lResult = EResult::Success;

		auto lIt = mPrograms.find(pstrName);
		if (lIt != mPrograms.end())
		{
			mPrograms.erase(lIt);
		}
		else
		{
			lResult = EResult::NotFound;
		}

		return lResult;
	}

	void CProgramManager::RemoveAll()
	{
		mPrograms.clear();
	}
}