#pragma once

#include "GUI/ControlAnimation.h"
#include "GUI/ControlMaterial.h"
#include "tinyxml2.h"

namespace FDX
{
	class FDX_EXPORT CXmlControlAnimationLoader
	{
	public:

		CXmlControlAnimationLoader();
		~CXmlControlAnimationLoader();

		EResult LoadAnimation(const tinyxml2::XMLElement *pNode, 
			IControlAnimation **pAnimation);

	private:

		EResult LoadLoopAnimation(const tinyxml2::XMLElement *pNode,
			IControlAnimation **pAnimation);

		EResult LoadStopAndWaitAnimation(const tinyxml2::XMLElement *pNode,
			IControlAnimation **pAnimation);

		EResult LoadOscillateAnimation(const tinyxml2::XMLElement *pNode,
			IControlAnimation **pAnimation);

		EResult LoadRandomAnimation(const tinyxml2::XMLElement *pNode,
			IControlAnimation **pAnimation);

		EResult LoadBaseAnimation(const tinyxml2::XMLElement *pNode,
			IControlAnimation *pAnimation);
	};

	using CXmlControlAnimationLoaderPtr = SmartPtr<CXmlControlAnimationLoader>;
}