#pragma once

#include "mxkit.h"
#include "mxkit_library.h"

namespace mx
{
    struct PythonVersion
    {
        int     mainVersion = 0;
        int     subVersion = 0;
        int     tagNumber = 0;
        int     buildNumber = 0;
        char    version[mxkit::_MX_LITTLE_BUFFER];
    };


    //----------------------------------------------------------------------------
    // IPython,
    //----------------------------------------------------------------------------
    struct IPython
    {
        virtual mxkit::Result Version(PythonVersion* ver) = 0;

        virtual mxkit::Result Release() = 0;

        virtual mxkit::Result ExcuteFile(const char* file) = 0;

        virtual mxkit::Result ExcuteMethod(const char* file, const char* method, int param, int* result) = 0;
        virtual mxkit::Result ExcuteMethod(const char* file, const char* method, int* result, const char* paramFormat, ...) = 0;

        virtual mxkit::Result ExcuteMethod(const char* file, const char* method, float param, float* result) = 0;
        virtual mxkit::Result ExcuteMethod(const char* file, const char* method, float* result, const char* paramFormat, ...) = 0;

        virtual mxkit::Result ExcuteMethod(const char* file, const char* method, const char* param, char** result) = 0;
        virtual mxkit::Result ExcuteMethod(const char* file, const char* method, char** result, const char* paramFormat, ...) = 0;

    };

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

    struct IPythonUtils
    {
        virtual mxkit::Result Alloc(const char* pythonDll, const char* homeDir, IPython** pp) = 0;
        
        enum Type
        {
            T_CHAR,
            T_MULTI_CHAR,
        };
        virtual mxkit::Result Release(Type type ,void * buffer) = 0;
    };


}