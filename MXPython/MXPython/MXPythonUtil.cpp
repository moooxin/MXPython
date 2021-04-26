#include "stdafx.h"
#include "MXPythonUtil.h"

namespace mxpy
{


    mxkit::Result MXPythonUtil::ReleaseString(char* buffer)
    {
        if(buffer)
            delete[] buffer;
    
        RETURN_RESULT(true);
    }

    char* MXPythonUtil::AllocString(const char* str)
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

}
