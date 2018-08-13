#pragma once

#include "OS/Win32/Win32File.h"

namespace FDX
{
	class FDX_EXPORT CWin32MappedFile : public CWin32File
	{
	public:

		CWin32MappedFile(const String &pstrFileName);
		~CWin32MappedFile();

		EResult Create(EOpenMode pOpenMode,
			EAccessRights pAccessRights) override;

		uint32_t GetPosition() const override;
		uint32_t GetSize() const override;

		int32_t Seek(ESeekOrigin pSeekOrigin, int32_t offset) override;

		EResult Read(uint32_t puiSize, void *pBuffer) override;
		EResult Write(uint32_t puiSize, const void *pBuffer) override;

		EResult WriteString(const char *pstrFormat, ...) override;
		EResult WriteString(const char *pstrFormat, va_list pArgs) override;

		EResult Flush() override;

	private:

		HANDLE mMapHandle;
		uint32_t muiSize;
		uint32_t muiPosition;
		uint8_t *mucData;
	};
}
