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

    //ExcuteMethod 中 paramFormat 以 , 分割。格式化的类型翻译如下----------------------------------------
    //():tuple
    //[]:list
    //{}:dict
    //b:
    //B:
    //h:
    //i:int
    //H:unsigned in
    //I:long from unsigned int
    //n:ling from size_t
    //l:long ftom long
    //k:long from unsigned long
    //L:long from long long
    //K:long from unsigned long long
    //u:unicode fromt wide char
    //f:
    //d:float from double
    //D:complex from c complex
    //c:bytes from string[0]
    //C:unicode
    //s:
    //z:
    //U:string
    //y:bytes
    //N:
    //S:
    //O:object
    //----------------------------------------------------------------------------------------------------

    struct IMXPythonUtil
    {
        virtual mxtoolkit::Result ReleaseString(char* buffer) = 0;
    };


}