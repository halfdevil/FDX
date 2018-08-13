#pragma once

#include "Core/Model.h"

namespace FDX
{
	enum class EModelLoaderType
	{
		Obj,
		Ezm,
		Fbx,
		Fdm,
		Dae,
		Assimp
	};

	class FDX_EXPORT IModelLoader
	{
	public:

		virtual ~IModelLoader()
		{
		}

		virtual EModelLoaderType GetModelLoaderType() const = 0;
		
		virtual bool CanLoadModelType(EModelType pType) = 0;
		virtual bool CanLoadModel(const String &pstrFilePath) = 0;

		virtual EResult Load(const String &pstrFilePath,
			IModel **pModel) = 0;
	};

	using IModelLoaderPtr = SmartPtr<IModelLoader>;
}