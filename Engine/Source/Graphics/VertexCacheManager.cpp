
#include "Graphics/VertexCacheManager.h"
#include "Graphics/GraphicsDevice.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CVertexCacheManager::CVertexCacheManager(IGraphicsDevice * pGraphicsDevice)
		: mGraphicsDevice(pGraphicsDevice)
	{
	}

	CVertexCacheManager::~CVertexCacheManager()
	{
	}

	EResult CVertexCacheManager::GetVertexFormat(const String & pstrName, CVertexFormat ** pVertexFormat)
	{
		EResult lResult = EResult::Success;

		auto lIt = mVertexFormats.find(pstrName);
		if (lIt != mVertexFormats.end())
		{
			*pVertexFormat = (*lIt).second.get();
		}
		else
		{
			LOG_DEBUG("Vertex format doesn't exist", pstrName.c_str());
			lResult = EResult::NotFound;
		}

		return lResult;
	}

	EResult CVertexCacheManager::AddVertexFormat(const String & pstrName, CVertexFormatPtr pVertexFormat)
	{
		EResult lResult = EResult::Success;

		auto lIt = mVertexFormats.find(pstrName);
		if (lIt != mVertexFormats.end())
		{
			LOG_DEBUG("Vertex format %s already exist", pstrName.c_str());
			lResult = EResult::AddFailed;
		}
		else
		{
			mVertexFormats[pstrName] = std::move(pVertexFormat);
		}

		return lResult;
	}

	EResult CVertexCacheManager::RemoveVertexFormat(const String & pstrName)
	{
		EResult lResult = EResult::Success;

		auto lIt = mVertexFormats.find(pstrName);
		if (lIt != mVertexFormats.end())
		{
			mVertexFormats.erase(lIt);
		}
		else
		{
			LOG_DEBUG("Vertex format doesn't exist", pstrName.c_str());
			lResult = EResult::NotFound;
		}

		return lResult;
	}

	EResult CVertexCacheManager::AddVertexCache(const String &pstrName, EIndexType pIndexType,
		uint32_t puiMaxNumVertices, uint32_t puiMaxNumIndices)
	{
		EResult lResult = EResult::Success;
		CVertexCache *lCache = nullptr;
		CVertexFormat *lVertexFormat = nullptr;

		auto lIt = mVertexFormats.find(pstrName);
		if (lIt != mVertexFormats.end())
		{
			lVertexFormat = (*lIt).second.get();
			lCache = new CVertexCache(mGraphicsDevice, pstrName);
			
			lResult = lCache->Create(lVertexFormat, pIndexType,
				puiMaxNumVertices, puiMaxNumIndices);

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("VertexCache creation failed for %s", pstrName.c_str());
			}
			else
			{
				mVertexCaches[lVertexFormat].push_back(CVertexCachePtr(lCache));			
			}
		}
		else
		{
			LOG_DEBUG("Vertex Format %s not found", pstrName.c_str());
			lResult = EResult::NotFound;
		}		

		return lResult;
	}

	EResult CVertexCacheManager::AddVertices(const String &pstrName, uint32_t puiNumVertices, const void * pData, IDrawOperation ** pDrawOperation)
	{
		EResult lResult = EResult::Success;
		CVertexCache *lCache = nullptr;
		IDrawOperation *lDrawOperation = nullptr;

		lResult = GetCache(pstrName, EIndexType::SixteenBit,
			puiNumVertices, 0, &lCache);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("GetCache() failed");
		}
		else
		{
			lResult = mGraphicsDevice->CreateDrawOperation(lCache->GetRenderData(), 
				&lDrawOperation);

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CreateDrawOperation() failed");
			}
			else
			{
				lDrawOperation->SetActiveIndexBuffer(-1);
				lDrawOperation->SetActiveVertexBuffer(0);
				lDrawOperation->SetNumIndices(0);
				lDrawOperation->SetStartVertex(lCache->GetNumVertices());
				lDrawOperation->SetNumVertices(puiNumVertices);

				lResult = lCache->AddVertices(puiNumVertices, pData);
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("VertexCache AddVertices() failed");
					SAFE_DELETE(lDrawOperation);
				}
				else
				{
					*pDrawOperation = lDrawOperation;
				}
			}
		}

		return lResult;
	}

	EResult CVertexCacheManager::AddVertices(const String &pstrName, EIndexType pIndexType,
		uint32_t puiNumVertices, uint32_t puiNumIndices, const void * pVertexData, 
		const void * pIndexData, IDrawOperation ** pDrawOperation)
	{
		EResult lResult = EResult::Success;
		CVertexCache *lCache = nullptr;
		IDrawOperation *lDrawOperation = nullptr;

		lResult = GetCache(pstrName, pIndexType, puiNumVertices, 
			puiNumIndices, &lCache);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("GetCache() failed");
		}
		else
		{
			lResult = mGraphicsDevice->CreateDrawOperation(lCache->GetRenderData(),
				&lDrawOperation);

			if (lResult != EResult::Success)
			{
				LOG_DEBUG("CreateDrawOperation() failed");
			}
			else
			{
				lDrawOperation->SetActiveIndexBuffer(0);
				lDrawOperation->SetActiveVertexBuffer(0);
				lDrawOperation->SetNumIndices(puiNumIndices);
				lDrawOperation->SetStartVertex(lCache->GetNumVertices());
				lDrawOperation->SetStartIndex(lCache->GetNumIndices());
				lDrawOperation->SetNumVertices(puiNumVertices);
				lDrawOperation->SetNumIndices(puiNumIndices);

				lResult = lCache->AddVertices(puiNumVertices, pVertexData);
				if (lResult != EResult::Success)
				{
					LOG_DEBUG("VertexCache AddVertices() failed");
					SAFE_DELETE(lDrawOperation);
				}
				else
				{
					lResult = lCache->AddIndices(puiNumIndices, pIndexData);
					if (lResult != EResult::Success)
					{
						LOG_DEBUG("VertexCache AddIndices() failed");
						SAFE_DELETE(lDrawOperation);
					}
					else
					{
						*pDrawOperation = lDrawOperation;
					}
				}
			}
		}

		return lResult;
	}

	EResult CVertexCacheManager::AddModel(const String & pstrName, IModel * pModel, Array<IDrawOperationPtr> &pDrawOperations)
	{
		EResult lResult = EResult::Success;
		CVertexCache *lCache = nullptr;

		const Array<IModelMeshPtr> &lMeshes = pModel->GetMeshes();
		const Array<IModelMaterialPtr> &lMaterials = pModel->GetMaterials();

		Array<SModelVertex> lVertices;	
		uint32_t luiNumIndices = 0;

		for (const auto &lMesh : lMeshes)
		{
			lVertices.insert(lVertices.end(), lMesh->GetVertices().begin(),
				lMesh->GetVertices().end());

			luiNumIndices += lMesh->GetIndices().size();
		}

		lResult = GetCache(pstrName, EIndexType::SixteenBit, lVertices.size(),
			luiNumIndices, &lCache);

		if (lResult != EResult::Success)
		{
			LOG_DEBUG("GetCache() failed");
		}
		else
		{
			uint32_t luiCacheStartVertex = lCache->GetNumVertices();
			uint32_t luiCacheStartIndex = lCache->GetNumIndices();

			lResult = lCache->AddVertices(lVertices.size(), lVertices.data());
			if (lResult != EResult::Success)
			{
				LOG_DEBUG("VertexCache AddVertices() failed");
			}
			else
			{
				for (uint32_t lIdx = 0; lIdx < lMaterials.size(); lIdx++)
				{
					Array<uint16_t> lIndices;
					uint32_t luiStartVertex = 0;

					for (const auto &lMesh : lMeshes)
					{
						const Array<IModelMeshPartPtr> &lMeshParts = lMesh->GetMeshParts();
						const Array<uint16_t> &lMeshIndices = lMesh->GetIndices();

						for (const auto &lMeshPart : lMeshParts)
						{
							if (lMeshPart->GetMaterialIndex() == lIdx)
							{
								uint32_t lCounter = 0;
								while (lCounter < lMeshPart->GetNumIndices())
								{
									lIndices.push_back(lMeshIndices[lCounter + lMeshPart->GetStartIndex()] + luiStartVertex);
									lCounter++;
								}
							}
						}

						luiStartVertex += lMesh->GetVertices().size();
					}

					lResult = lCache->AddIndices(lIndices.size(), lIndices.data());
					if (lResult != EResult::Success)
					{
						LOG_DEBUG("VertexCache AddIndices() failed");
						break;
					}
					else
					{
						IDrawOperation *lDrawOperation;
						lResult = mGraphicsDevice->CreateDrawOperation(lCache->GetRenderData(),
							&lDrawOperation);

						if (lResult != EResult::Success)
						{
							LOG_DEBUG("CreateDrawOperation() failed");
							break;
						}
						else
						{
							lDrawOperation->SetActiveIndexBuffer(0);
							lDrawOperation->SetActiveVertexBuffer(0);
							lDrawOperation->SetNumIndices(lIndices.size());
							lDrawOperation->SetStartVertex(luiCacheStartVertex);
							lDrawOperation->SetStartIndex(luiCacheStartIndex);
							lDrawOperation->SetNumVertices(lVertices.size());
							lDrawOperation->SetNumIndices(lIndices.size());

							pDrawOperations.push_back(IDrawOperationPtr(lDrawOperation));
							luiCacheStartIndex += lIndices.size();
						}
					}
				}
			}
		}

		return lResult;
	}

	EResult CVertexCacheManager::GetCache(const String &pstrName, EIndexType pIndexType,
		uint32_t puiNumVertices, uint32_t puiNumIndices, CVertexCache ** pCache)
	{
		EResult lResult = EResult::Success;

		CVertexFormat *lVertexFormat = nullptr;

		auto lIt2 = mVertexFormats.find(pstrName);
		if (lIt2 != mVertexFormats.end())
		{
			lVertexFormat = (*lIt2).second.get();
			auto lIt = mVertexCaches.find(lVertexFormat);

			if (lIt != mVertexCaches.end())
			{
				auto &lCacheArray = (*lIt).second;
				bool lbCanAdd = false;

				for (uint32_t lIdx = 0; lIdx < lCacheArray.size(); lIdx++)
				{
					if (lCacheArray[lIdx]->CanAddVertices(puiNumVertices))
						lbCanAdd = true;

					if (puiNumIndices > 0)
					{
						if (!lCacheArray[lIdx]->HasIndexBuffer())
							lbCanAdd = false;
					}

					if (lbCanAdd)
					{
						*pCache = lCacheArray[lIdx].get();
						break;
					}
				}

				if (!lbCanAdd)
				{
					LOG_DEBUG("Cache not found");
					lResult = EResult::NotFound;
				}
			}
			else
			{
				LOG_DEBUG("Cache of specified VertexFormat not found");
				lResult = EResult::NotFound;
			}
		}
		else
		{
			LOG_DEBUG("Specified VertexFormat not found");
			lResult = EResult::NotFound;
		}

		return lResult;
	}
}