#pragma once

#include "Core/Result.h"

namespace FDX
{
	template <typename Type, typename Builder> class CFactory
	{
	public:

		CFactory(Builder *pBuilder)
			: mBuilder(pBuilder)
		{
		}

		virtual ~CFactory()
		{
		}

		Builder* GetBuilder() const
		{
			return mBuilder;
		}

		EResult Build(Type **pObject)
		{
			return mBuilder->Build(pObject);
		}

	protected:

		Builder *mBuilder;
	};
}
