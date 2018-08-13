#pragma once

#include "Core/Core.h"

template <typename T> class CSingleton
{
public:

	CSingleton()
	{
		msInstance = static_cast<T*>(this);
	}

	virtual ~CSingleton()
	{
	}

	static bool IsCreated()
	{
		return msInstance != nullptr;
	}

	static T& Get() 
	{
		return *msInstance;
	}

	static T* GetPtr() 
	{
		return msInstance;
	}

	static void SetPtr(T *pInstance)
	{
		msInstance = pInstance;
	}

private:

	static T *msInstance;
};

template <typename T> T* CSingleton<T>::msInstance = nullptr;