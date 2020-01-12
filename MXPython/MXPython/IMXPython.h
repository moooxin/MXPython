#pragma once

#include "MXCommon.h"
#include "MXBaseNotify.h"


namespace mxpy
{

    //----------------------------------------------------------------------------
    // IWebRequest,
    //----------------------------------------------------------------------------
    struct IMXPython37
    {
        //初始化
        virtual mxtoolkit::Result Initialize(const CHAR* homeDir) = 0;

        virtual mxtoolkit::Result Uninstall() = 0;

        virtual mxtoolkit::Result ExcuteFile(const CHAR* file) = 0;

        virtual mxtoolkit::Result ExcuteMethod(const CHAR* file, const CHAR* method, const CHAR* param, char** result) = 0;

    };

    struct IMXPythonUtil
    {
        virtual mxtoolkit::Result ReleaseString(char* buffer) = 0;
    };


}