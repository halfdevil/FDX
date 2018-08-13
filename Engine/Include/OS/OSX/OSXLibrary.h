#pragma once

#include "OS/Library.h"
#include "Core/Result.h"
#include <dlfcn.h>

namespace FDX
{
    class FDX_EXPORT COSXLibrary : public ILibrary
    {
    public:

        explicit COSXLibrary(const String &pstrFileName);
        ~COSXLibrary() override;

        const String& GetFileName() const override
        {
            return mstrFileName;
        }

        void* GetFunctionAddress(const char *pstrFuncName) override;

        EResult Load();
        EResult Unload();

    private:

        void *mHandle;
        String mstrFileName;
    };
}