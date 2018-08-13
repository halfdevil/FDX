
#include "Scene/TerrainTraverser.h"

namespace FDX
{
	CTerrainTraverser::CTerrainTraverser(CTerrainEntity * pTerrain)
		: mTerrain(pTerrain)
	{
	}

	CTerrainTraverser::~CTerrainTraverser()
	{
	}

	void CTerrainTraverser::GetVisible(const CFrustum & pFrustum, CRenderQueue * pRenderQueue)
	{
		if (mTerrain)
		{
			if (mTerrain->HasCameraMoved(pRenderQueue->GetCamera()))
			{
				mTerrain->GetTerrainQuadTree()->UpdatePatches(pRenderQueue->GetCamera());
				mTerrain->UpdatePatchLevels();
			}

			AppendPatches(mTerrain->GetTerrainQuadTree(), pRenderQueue);
		}
	}

	void CTerrainTraverser::AppendPatches(CTerrainNode * pTerrainNode, CRenderQueue * pRenderQueue)
	{
		if (pTerrainNode->IsVisible())
		{
			if (pTerrainNode->IsLeafNode())
			{
				pRenderQueue->AddRenderable(pTerrainNode->GetTerrainPatch());
			}
			else
			{
				AppendPatches(pTerrainNode->GetChildUL(), pRenderQueue);
				AppendPatches(pTerrainNode->GetChildUR(), pRenderQueue);
				AppendPatches(pTerrainNode->GetChildLL(), pRenderQueue);
				AppendPatches(pTerrainNode->GetChildLR(), pRenderQueue);
			}
		}
	}
}