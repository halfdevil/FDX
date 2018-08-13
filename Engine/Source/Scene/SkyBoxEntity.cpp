
#include "Scene/SkyBoxEntity.h"
#include "Graphics/TextureManager.h"
#include "Graphics/ProgramManager.h"
#include "Graphics/VertexCacheManager.h"
#include "Graphics/VertexTypes.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CSkyBoxMaterial::CSkyBoxMaterial()
		: muiNumPasses(1)
	{
	}

	uint8_t CSkyBoxMaterial::GetNumPasses() const
	{
		return muiNumPasses;
	}

	bool CSkyBoxMaterial::UsesPass(EPassType pType)
	{
		return pType == EPassType::Diffuse;
	}

	bool CSkyBoxMaterial::IsTransparent()
	{
		return false;
	}

	EResult CSkyBoxMaterial::Create()
	{
		EResult lResult = EResult::Success;
		IProgram *lProgram = nullptr;

		lResult = CProgramManager::Get().GetProgram("SkyBoxProgram", &lProgram);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("GetProgram() failed for SkyBoxProgram");
		}
		else
		{
			mPrograms[EPassType::Diffuse] = lProgram;
		}

		return lResult;
	}

	CSkyBoxEntity::CSkyBoxEntity()
		: mMaterial(nullptr),
		mDrawOperation(nullptr)
	{
	}

	CSkyBoxEntity::~CSkyBoxEntity()
	{
	}

	EResult CSkyBoxEntity::Load(const Array<String>& pImages)
	{
		EResult lResult = EResult::Success;

		lResult = LoadRenderData();
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("LoadRenderData() failed");
		}
		else
		{
			auto lMaterial = std::make_unique<CSkyBoxMaterial>();
			lResult = lMaterial->Create();

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CSkyBoxMaterial::Create() failed");
			}
			else
			{
				lMaterial->SetStartPassCallback(std::bind(&CSkyBoxEntity::StartPass,
					this, std::placeholders::_1, std::placeholders::_2));

				lMaterial->SetEndPassCallback(std::bind(&CSkyBoxEntity::EndPass,
					this, std::placeholders::_1));

				CTextureProperties lTextureProps;
				lTextureProps.SetFilter(ETextureFilter::Bilinear);
				lTextureProps.SetGenerateMipMaps(false);

				CTextureCube *lCubeMap = nullptr;
				lResult = CTextureManager::Get().AddTextureCube(mstrName, 
					pImages, lTextureProps, &lCubeMap);

				if (lResult != EResult::Success)
				{
					LOG_DEBUG("AddTextureCube() failed");
				}
				else
				{
					lMaterial->AddTexture(EPassType::Diffuse, lCubeMap, "uCubeMap");
					mMaterial = std::move(lMaterial);
				}
			}
		}

		return lResult;
	}

	void CSkyBoxEntity::StartPass(EPassType pPassType, IProgram * pProgram)
	{
	}

	void CSkyBoxEntity::EndPass(EPassType pPassType)
	{
	}

	void CSkyBoxEntity::Draw(CCamera *pCamera)
	{
		auto lProgram = mMaterial->GetProgram(EPassType::Diffuse);

		if (lProgram)
		{
			CMatrix4F lMVP =
				pCamera->GetProjection() * pCamera->GetView() * GetGlobalTransform();

			lProgram->Bind();
			lProgram->GetConstants()->SetMatrix4("uMVP", lMVP);

			for (uint32_t lIdx = 0; lIdx < mMaterial->GetNumTextures(EPassType::Diffuse); lIdx++)
			{
				uint32_t luiLoc = 0;
				ITexture *lTexture = mMaterial->GetTexture(EPassType::Diffuse,
					lIdx, &luiLoc);

				lProgram->GetConstants()->SetTexture(luiLoc, lIdx, lTexture);
			}

			mMaterial->StartPass(EPassType::Diffuse);
			mDrawOperation->Draw(mDrawOperation->GetActiveVertexBuffer(),
				mDrawOperation->GetActiveIndexBuffer());
			mMaterial->EndPass(EPassType::Diffuse);
		}
	}

	EResult CSkyBoxEntity::LoadRenderData()
	{
		EResult lResult = EResult::Success;
		IDrawOperation *lDrawOperation = nullptr;

		Array<SVertexP> lVertices =
		{
			CVector3F(-0.5f, -0.5f, -0.5f),
			CVector3F(0.5f, -0.5f, -0.5f),
			CVector3F(0.5f, 0.5f, -0.5f),
			CVector3F(-0.5f, 0.5f, -0.5f),
			CVector3F(-0.5f, -0.5f, 0.5f),
			CVector3F(0.5f, -0.5f, 0.5f),
			CVector3F(0.5f, 0.5f, 0.5f),
			CVector3F(-0.5f, 0.5f, 0.5f)
		};

		Array<uint16_t> lIndices =
		{
			0, 4, 5,
			5, 1, 0,
			3, 6, 7,
			3, 2, 6,
			7, 6, 4,
			6, 5, 4,
			2, 3, 1,
			3, 0, 1,
			3, 7, 0,
			7, 4, 0,
			6, 2, 5,
			2, 1, 5
		};	

		lResult = CVertexCacheManager::Get().AddVertices("VertexP", EIndexType::SixteenBit,
			lVertices.size(), lIndices.size(), lVertices.data(), lIndices.data(), &lDrawOperation);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("CVertexCacheManager::AddVertices() failed");
		}
		else
		{
			mDrawOperation = IDrawOperationPtr(lDrawOperation);
			mDrawOperation->SetPrimitiveType(EPrimitiveType::TriangleList);
		}

		return lResult;
	}
}