#include "stdafx.h"
#include "PythonUtils.h"

#include "win32/file_utils.h"

#include "Python3X.h"
#include "Python27.h"


mxkit::Result PythonUtils::Alloc(const char* pythonDll, const char* homeDir, mx::IPython** pp)
{
    if (!pp ||
        mxkit::FileSystem::FileType<>(pythonDll) != mxkit::FileSystem::FILE ||
        mxkit::FileSystem::FileType<>(homeDir) != mxkit::FileSystem::FOLDER)
    {
        RETURN_RESULT(false);
    }

    PythonApi::Info info;
    if (!PythonApi::Load(pythonDll, &info) && info.version.mainVersion != 2 && info.version.mainVersion != 3)
    {
        PythonApi::Free(&info);

        RETURN_RESULT(false);
    }

    if (2 == info.version.mainVersion)
    {
        Python27* py = new Python27;

        if (!mxkit::ResultUtils::Success(py->Initialize(info, homeDir)))
        {
            PythonApi::Free(&info);

            RETURN_RESULT(false);
        }

        *pp = dynamic_cast<mx::IPython*>(py);
    }
    else if (3 == info.version.mainVersion)
    {
        Python3X* py = new Python3X;

        if (!mxkit::ResultUtils::Success(py->Initialize(info, homeDir)))
        {
            PythonApi::Free(&info);

            RETURN_RESULT(false);
        }

        *pp = dynamic_cast<mx::IPython*>(py);
    }

    RETURN_RESULT(true);
}


mxkit::Result PythonUtils::Release(Type type, void* buffer)
{
    if (!buffer)
        RETURN_RESULT(true);

    switch (type)
    {
    case mx::IPythonUtils::T_CHAR:
        delete (char*)buffer;
        break;
    case mx::IPythonUtils::T_MULTI_CHAR:
        delete[](char*)buffer;
        break;
    default:
        break;
    }

    RETURN_RESULT(true);
}

char* PythonUtils::AllocString(const char* str)
{
    if (!str)
        return nullptr;

    size_t size = strlen(str) + 1;
    if (size <= 1)
        return nullptr;

    char* ret = new char[size];
    if (!ret)
        return nullptr;

    memcpy(ret, str, size);
    return ret;
}

