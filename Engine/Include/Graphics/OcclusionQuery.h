#pragma once

#include "Core/Core.h"
#include "Core/Result.h"

namespace FDX
{
	enum class EQueryType
	{
		SamplesPassed
	};

	class FDX_EXPORT IOcclusionQuery
	{
	public:

		virtual ~IOcclusionQuery()
		{
		}

		virtual const String& GetName() const = 0;

		virtual bool IsResultAvailable() = 0;
		virtual uint32_t GetResult() = 0;

		virtual EResult BeginQuery(EQueryType pQueryType) = 0;
		virtual EResult EndQuery() = 0;
	};

	using IOcclusionQueryPtr = SmartPtr<IOcclusionQuery>;
}