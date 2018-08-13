#pragma once

#include "Core/Core.h"

namespace FDX
{
	template <typename T> class CObjectPool
	{
	public:

		explicit CObjectPool(uint32_t puiSize);
		~CObjectPool();

		uint32_t GetSize() const
		{
			return muiSize;
		}

		uint32_t GetNumObjects() const
		{
			return muiNumObjects;
		}

		T* GetObject();
		void ReleaseObject(T *pObject);

		void ClearUnused();
		void Empty();
		void Reset();

	private:

		uint32_t muiSize;
		uint32_t muiNumObjects;
		Array<T*> mObjects;
	};

	template <typename T>
	using CObjectPoolPtr = SmartPtr<CObjectPool<T>>;

	template<typename T>
	inline CObjectPool<T>::CObjectPool(uint32_t puiSize)
		: muiSize(puiSize),
		muiNumObjects(0)
	{
		mObjects.resize(muiSize);
		for (uint32_t lIdx = 0; lIdx < muiSize; lIdx++)
			mObjects[lIdx] = new T();
	}

	template<typename T>
	inline CObjectPool<T>::~CObjectPool()
	{
		Empty();
	}

	template<typename T>
	inline T * CObjectPool<T>::GetObject()
	{
		T *lObject = mObjects[muiNumObjects];

		if (muiNumObjects == muiSize - 1)
		{
			uint32_t luiStart = muiSize;
			
			muiSize *= 2;
			mObjects.resize(muiSize);

			for (uint32_t lIdx = luiStart; lIdx < muiSize; lIdx++)
				mObjects[lIdx] = new T();

			muiNumObjects++;
		}
		else
		{
			muiNumObjects++;
		}

		return lObject;
	}

	template<typename T>
	inline void CObjectPool<T>::ReleaseObject(T * pObject)
	{
		if (muiNumObjects == 0)
			return;

		mObjects[--muiNumObjects] = pObject;
	}

	template<typename T>
	inline void CObjectPool<T>::ClearUnused()
	{
		for (uint32_t lIdx = muiNumObjects + 1; lIdx < muiSize; lIdx++)
			SAFE_DELETE(mObjects[lIdx]);

		muiSize = muiNumObjects + 1;
		mObjects.resize(muiSize);
	}

	template<typename T>
	inline void CObjectPool<T>::Empty()
	{
		for (uint32_t lIdx = 0; lIdx < muiSize; lIdx++)
			SAFE_DELETE(mObjects[lIdx]);

		mObjects.clear();
	}

	template<typename T>
	inline void CObjectPool<T>::Reset()
	{
		muiNumObjects = 0;
	}
}