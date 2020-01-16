#pragma once

#include "MXCommon.h"
#include "MXBaseNotify.h"

namespace mxpy
{

#if _PY_VER_==37
    //----------------------------------------------------------------------------
    // IWebRequest,
    //----------------------------------------------------------------------------
    struct IMXPython37
    {
        //初始化
        virtual mxtoolkit::Result Initialize(const char* homeDir) = 0;

        virtual mxtoolkit::Result Uninstall() = 0;

        virtual mxtoolkit::Result ExcuteFile(const char* file) = 0;

        virtual mxtoolkit::Result ExcuteMethod(const char* file, const char* method, int param, int* result) = 0;
        virtual mxtoolkit::Result ExcuteMethod(const char* file, const char* method, int* result, const char* paramFormat, ...) = 0;

        virtual mxtoolkit::Result ExcuteMethod(const char* file, const char* method, float param, float* result) = 0;
        virtual mxtoolkit::Result ExcuteMethod(const char* file, const char* method, float* result, const char* paramFormat, ...) = 0;

        virtual mxtoolkit::Result ExcuteMethod(const char* file, const char* method, const char* param, char** result) = 0;
        virtual mxtoolkit::Result ExcuteMethod(const char* file, const char* method, char** result, const char* paramFormat, ...) = 0;

    };
#endif

    struct IMXPythonUtil
    {
        virtual mxtoolkit::Result ReleaseString(char* buffer) = 0;
    };


}