
#include "OS/OSX/OSXLibrary.h"
#include "OS/System.h"
#include "OS/GameLogger.h"

namespace FDX
{
    COSXLibrary::COSXLibrary(const FDX::String &pstrFileName)
        : mstrFileName(pstrFileName),
          mHandle(nullptr)
    {
    }

    COSXLibrary::~COSXLibrary()
    {
        Unload();
    }

    void* COSXLibrary::GetFunctionAddress(const char *pstrFuncName)
    {
        FDX_ASSERT(mHandle != nullptr, _T("DLL %s not loaded"), mstrFileName.c_str());
        return dlsym(mHandle, pstrFuncName);
    }

    EResult COSXLibrary::Load()
    {
        EResult lResult = EResult::Success;

        mHandle = dlopen(mstrFileName.c_str(), RTLD_LAZY);
        if (mHandle == nullptr)
        {
            LOG_DEBUG(_T("dlopen() failed for: %s"), mstrFileName.c_str());
            lResult = EResult::LoadFailed;
        }

        return lResult;
    }

    EResult COSXLibrary::Unload()
    {
        if (mHandle)
            dlclose(mHandle);
    }
}
