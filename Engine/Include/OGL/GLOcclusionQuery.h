#pragma once

#include "Graphics/OcclusionQuery.h"
#include <GL/gl3w.h>

namespace FDX
{
	class CGLOcclusionQuery : public IOcclusionQuery
	{
	public:

		CGLOcclusionQuery(const String &pstrName);
		~CGLOcclusionQuery();

		const String& GetName() const
		{
			return mstrName;
		}

		EResult Create();

		bool IsResultAvailable();
		uint32_t GetResult();

		EResult BeginQuery(EQueryType pQueryType);
		EResult EndQuery();

	private:

		String mstrName;
		GLuint muiHandle;
		GLenum mQueryType;
	};
}