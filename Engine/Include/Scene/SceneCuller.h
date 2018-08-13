#pragma once

#include "Scene/SceneNode.h"
#include "Scene/Camera.h"

namespace FDX
{
	class FDX_EXPORT ISceneCuller
	{
	public:

		virtual ~ISceneCuller()
		{
		}

		virtual CCamera* GetCamera() const = 0;
		virtual void SetCamera(CCamera *pCamera) = 0;

		virtual bool CullNode(CSceneNode *pSceneNode) = 0;
	};

	using ISceneCullerPtr = SmartPtr<ISceneCuller>;
}