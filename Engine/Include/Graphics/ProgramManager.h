#pragma once

#include "Core/Singleton.h"
#include "Graphics/Program.h"
#include "Graphics/GraphicsDevice.h"

namespace FDX
{
	using ProgramShaders = UnorderedMap<EShaderType, String>;

	class CProgramParameters
	{
	public:

		CProgramParameters(const String &pstrVertexFormat,
			const ProgramShaders &pShaders)
			: mstrVertexFormat(pstrVertexFormat),
			mShaders(pShaders)
		{
		}

		const String& GetVertexFormat() const
		{
			return mstrVertexFormat;
		}

		const ProgramShaders& GetShaders() const
		{
			return mShaders;
		}

	private:

		String mstrVertexFormat;
		ProgramShaders mShaders;
	};

	class FDX_EXPORT CProgramManager : public CSingleton<CProgramManager>
	{
	public:

		CProgramManager(IGraphicsDevice *pGraphicsDevice);
		~CProgramManager();

		CProgramManager(const CProgramManager&) = delete;
		CProgramManager& operator=(const CProgramManager&) = delete;

		IGraphicsDevice* GetGraphicsDevice() const
		{
			return mGraphicsDevice;
		}

		EResult GetProgram(const String &pstrName, IProgram **pProgram);
		EResult AddProgram(const String &pstrName, const ProgramShaders &mShaders,
			CVertexFormat *pFormat, IProgram **pProgram);

		EResult AddProgram(const String &pstrName, const CProgramParameters &pParameters,
			IProgram **pProgram);

		EResult RemoveProgram(const String &pstrName);
		void RemoveAll();

	private:

		IGraphicsDevice *mGraphicsDevice;
		UnorderedMap<String, IProgramPtr> mPrograms;
	};

	using CProgramManagerPtr = SmartPtr<CProgramManager>;
}