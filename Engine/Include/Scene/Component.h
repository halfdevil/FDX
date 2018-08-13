#pragma once

#include "Core/Core.h"

namespace FDX
{
	class CSceneEntity;

	class FDX_EXPORT IComponent
	{
	public:

		virtual ~IComponent()
		{
		}

		virtual CSceneEntity* GetEntity() = 0;
		virtual void SetEntity(CSceneEntity *pEntity) = 0;

		virtual void Start() = 0;
		virtual void Stop() = 0;

		virtual void Update() = 0;
		virtual void FixedUpdate() = 0;
	};

	using IComponentPtr = SmartPtr<IComponent>;
}