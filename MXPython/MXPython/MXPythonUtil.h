#pragma once
#include "IMXPython.h"
#include "MXDllExportDefine.h"
#include "MXSingleObject.h"

namespace mxpy
{
    class MXPythonUtil
        : public IMXPythonUtil
        , public mxtoolkit::MXInterfaceImp<MXPythonUtil>
        , public mxtoolkit::MXSingleObject<MXPythonUtil>
    {
    public:
        virtual mxtoolkit::Result ReleaseString(char* buffer) override;

    public:
        char* AllocString(const char* str);
    };

}