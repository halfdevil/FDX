
#include "OGL/GLOcclusionQuery.h"
#include "OGL/GLUtil.h"
#include "OS/GameLogger.h"

namespace FDX
{
	CGLOcclusionQuery::CGLOcclusionQuery(const String & pstrName)
		: mstrName(pstrName),
		muiHandle(0),
		mQueryType(0)
	{
	}

	CGLOcclusionQuery::~CGLOcclusionQuery()
	{
		if (muiHandle)
			glDeleteQueries(1, &muiHandle);
	}

	EResult CGLOcclusionQuery::Create()
	{
		EResult lResult = EResult::Success;

		glGenQueries(1, &muiHandle);
		if (!muiHandle)
		{
			LOG_DEBUG("glGenQueries() failed for %s", mstrName.c_str());
			lResult = EResult::CreateFailed;
		}

		return lResult;
	}

	bool CGLOcclusionQuery::IsResultAvailable()
	{
		GLuint luiAvailable = GL_FALSE;

		glGetQueryObjectuiv(muiHandle, GL_QUERY_RESULT_AVAILABLE,
			&luiAvailable);

		return luiAvailable == GL_TRUE;
	}

	uint32_t CGLOcclusionQuery::GetResult()
	{
		GLuint luiQueryResult = 0;

		glGetQueryObjectuiv(muiHandle, GL_QUERY_RESULT,
			&luiQueryResult);

		return luiQueryResult;
	}

	EResult CGLOcclusionQuery::BeginQuery(EQueryType pQueryType)
	{
		EResult lResult = EResult::Success;

		mQueryType = CGLUtil::GetQueryType(pQueryType);
		glBeginQuery(mQueryType, muiHandle);

		if (glGetError() != GL_NO_ERROR)
		{
			LOG_DEBUG("glBeginQuery() failed for %s", mstrName.c_str());
			lResult = EResult::Error;
		}

		return lResult;
	}

	EResult CGLOcclusionQuery::EndQuery()
	{
		EResult lResult = EResult::Success;

		glEndQuery(mQueryType);

		if (glGetError() != GL_NO_ERROR)
		{
			LOG_DEBUG("glEndQuery() failed for %s", mstrName.c_str());
			lResult = EResult::Error;
		}

		return lResult;
	}
}