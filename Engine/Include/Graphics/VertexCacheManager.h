#pragma once

#include "Core/Singleton.h"
#include "Graphics/VertexCache.h"
#include "Core/Model.h"
#include "Graphics/DrawOperation.h"

namespace FDX
{
	class FDX_EXPORT CVertexCacheManager : public CSingleton<CVertexCacheManager>
	{
	public:

		CVertexCacheManager(IGraphicsDevice *pGraphicsDevice);
		~CVertexCacheManager();

		CVertexCacheManager(const CVertexCacheManager&) = delete;
		CVertexCacheManager& operator=(const CVertexCacheManager&) = delete;

		EResult GetVertexFormat(const String &pstrName, CVertexFormat **pVertexFormat);

		EResult AddVertexFormat(const String &pstrName, CVertexFormatPtr pVertexFormat);
		EResult RemoveVertexFormat(const String &pstrName);

		EResult AddVertexCache(const String &pstrName, EIndexType pIndexType, 
			uint32_t puiMaxNumVertices, uint32_t puiMaxNumIndices);

		EResult AddVertices(const String &pstrName, uint32_t puiNumVertices, const void *pData,
			IDrawOperation **pDrawOperation);

		EResult AddVertices(const String &pstrName, EIndexType pIndexType, uint32_t puiNumVertices, 
			uint32_t puiNumIndices,	const void *pVertexData, const void *pIndexData, IDrawOperation **pDrawOperation);
		
		EResult AddModel(const String &pstrName, IModel *pModel, Array<IDrawOperationPtr> &pDrawOperations);

	private:

		EResult GetCache(const String &pstrName, EIndexType pIndexType,
			uint32_t puiNumVertices, uint32_t puiNumIndices, CVertexCache **pCache);

	private:

		IGraphicsDevice *mGraphicsDevice;
		UnorderedMap<String, CVertexFormatPtr> mVertexFormats;
		UnorderedMap<CVertexFormat*, Array<CVertexCachePtr>> mVertexCaches;
	};

	using CVertexCacheManagerPtr = SmartPtr<CVertexCacheManager>;
}