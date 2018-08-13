
#include "GUI/XmlControlAnimationLoader.h"
#include "OS/GameLogger.h"
#include "OS/System.h"

namespace FDX
{
	CXmlControlAnimationLoader::CXmlControlAnimationLoader()
	{
	}

	CXmlControlAnimationLoader::~CXmlControlAnimationLoader()
	{
	}

	EResult CXmlControlAnimationLoader::LoadAnimation(const tinyxml2::XMLElement * pNode, 
		IControlAnimation ** pAnimation)
	{
		EResult lResult = EResult::Success;

		IControlAnimation *lAnimation = nullptr;
		EControlAnimationType lType = (EControlAnimationType)pNode->IntAttribute("type");		

		switch (lType)
		{
		case EControlAnimationType::Loop:
			lResult = LoadLoopAnimation(pNode, pAnimation);
			break;

		case EControlAnimationType::StopAtEnd:
			lResult = LoadStopAndWaitAnimation(pNode, pAnimation);
			break;

		case EControlAnimationType::Oscillate:
			lResult = LoadOscillateAnimation(pNode, pAnimation);
			break;

		case EControlAnimationType::Random:
			lResult = LoadRandomAnimation(pNode, pAnimation);
			break;
		}

		return lResult;
	}

	EResult CXmlControlAnimationLoader::LoadLoopAnimation(const tinyxml2::XMLElement * pNode, 
		IControlAnimation ** pAnimation)
	{
		EResult lResult = EResult::Success;

		String lstrName = (pNode->Name() != nullptr) ?
			pNode->Name() : "";

		CLoopControlAnimation *lAnimation = new CLoopControlAnimation(lstrName);

		lResult = LoadBaseAnimation(pNode, lAnimation);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("LoadBaseAnimation() failed");
			SAFE_DELETE(lAnimation);
		}
		else
		{
			*pAnimation = lAnimation;
		}

		return lResult;
	}

	EResult CXmlControlAnimationLoader::LoadStopAndWaitAnimation(const tinyxml2::XMLElement * pNode, 
		IControlAnimation ** pAnimation)
	{
		EResult lResult = EResult::Success;

		String lstrName = (pNode->Name() != nullptr) ?
			pNode->Name() : "";

		CStopAtEndControlAnimation *lAnimation = new CStopAtEndControlAnimation(lstrName);

		lResult = LoadBaseAnimation(pNode, lAnimation);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("LoadBaseAnimation() failed");
			SAFE_DELETE(lAnimation);
		}
		else
		{
			*pAnimation = lAnimation;
		}

		return lResult;
	}

	EResult CXmlControlAnimationLoader::LoadOscillateAnimation(const tinyxml2::XMLElement * pNode, 
		IControlAnimation ** pAnimation)
	{
		EResult lResult = EResult::Success;

		String lstrName = (pNode->Name() != nullptr) ?
			pNode->Name() : "";

		COscillateControlAnimation *lAnimation = new COscillateControlAnimation(lstrName);

		lResult = LoadBaseAnimation(pNode, lAnimation);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("LoadBaseAnimation() failed");
			SAFE_DELETE(lAnimation);
		}
		else
		{
			*pAnimation = lAnimation;
		}

		return lResult;
	}

	EResult CXmlControlAnimationLoader::LoadRandomAnimation(const tinyxml2::XMLElement * pNode, 
		IControlAnimation ** pAnimation)
	{
		EResult lResult = EResult::Success;

		String lstrName = (pNode->Name() != nullptr) ?
			pNode->Name() : "";

		CRandomControlAnimation *lAnimation = new CRandomControlAnimation(lstrName);

		lResult = LoadBaseAnimation(pNode, lAnimation);
		if (lResult != EResult::Success)
		{
			LOG_DEBUG("LoadBaseAnimation() failed");
			SAFE_DELETE(lAnimation);
		}
		else
		{
			*pAnimation = lAnimation;
		}

		return lResult;
	}
	
	EResult CXmlControlAnimationLoader::LoadBaseAnimation(const tinyxml2::XMLElement * pNode,
		IControlAnimation * pAnimation)
	{
		EResult lResult = EResult::Success;

		if (pAnimation)
			pAnimation->SetFrameLength(pNode->FloatAttribute("frame_length"));

		return lResult;
	}
}