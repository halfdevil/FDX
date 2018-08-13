
#include "GUI/ControlSkin.h"

namespace FDX
{
	CControlSkin::CControlSkin(const String & pstrName)
		: mstrName(pstrName),
		mActiveMaterial(nullptr)
	{
	}

	CControlSkin::~CControlSkin()
	{
	}

	void CControlSkin::AddMaterial(CControlMaterialPtr pMaterial)
	{
		EControlMaterialType lType = pMaterial->GetMaterialType();
		mMaterials[lType] = std::move(pMaterial);
	}

	void CControlSkin::SetActiveMaterial(EControlMaterialType pMaterialType)
	{
		auto lIt = mMaterials.find(pMaterialType);

		if (lIt != mMaterials.end())
			mActiveMaterial = (*lIt).second.get();
	}

	void CControlSkin::Update(float pfDelta)
	{
		if (mActiveMaterial != nullptr)
			mActiveMaterial->Update(pfDelta);
	}
}