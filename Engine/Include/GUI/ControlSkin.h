#pragma once

#include "GUI/ControlSkin.h"
#include "GUI/ControlMaterial.h"

namespace FDX
{
	class FDX_EXPORT CControlSkin
	{
	public:

		CControlSkin(const String &pstrName);
		virtual ~CControlSkin();

		CControlSkin(const CControlSkin&) = delete;
		CControlSkin& operator = (const CControlSkin&) = delete;

		const String& GetName() const
		{
			return mstrName;
		}

		CControlMaterial* GetActiveMaterial() const
		{
			return mActiveMaterial;
		}

		CControlMaterial* GetMaterial(EControlMaterialType pMaterialType) const
		{
			auto lIt = mMaterials.find(pMaterialType);

			if (lIt != mMaterials.end())
				return (*lIt).second.get();

			return nullptr;
		}

		virtual void AddMaterial(CControlMaterialPtr pMaterial);
		virtual void SetActiveMaterial(EControlMaterialType pMaterialType);

		virtual void Update(float pfDelta);

	protected:

		String mstrName;
		CControlMaterial *mActiveMaterial;
		UnorderedMap<EControlMaterialType, CControlMaterialPtr> mMaterials;
	};

	using CControlSkinPtr = SmartPtr<CControlSkin>;
}