#pragma once

#include "Scene/SceneTraverser.h"
#include "Scene/TerrainEntity.h"

namespace FDX
{
	class FDX_EXPORT CTerrainTraverser : public ISceneTraverser
	{
	public:

		CTerrainTraverser(CTerrainEntity *pTerrain);
		~CTerrainTraverser();

		void GetVisible(const CFrustum &pFrustum,
			CRenderQueue *pRenderQueue) override;

	private:

		void AppendPatches(CTerrainNode *pTerrainNode,
			CRenderQueue *pRenderQueue);

	private:

		CTerrainEntity *mTerrain;
	};
}