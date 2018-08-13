
#include "Scene/SubMeshEntity.h"

namespace FDX
{
	CSubMeshEntity::CSubMeshEntity()
	{
		mRenderableType = ERenderableType::Normal;
	}

	CSubMeshEntity::~CSubMeshEntity()
	{
	}

	void CSubMeshEntity::SetMaterial(IMaterial * pMaterial)
	{
		mMaterial = pMaterial;
	}

	void CSubMeshEntity::SetDrawOperation(IDrawOperation * pDrawOperation)
	{
		mDrawOperation = pDrawOperation;
	}
}