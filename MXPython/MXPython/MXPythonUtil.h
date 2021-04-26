#pragma once
#include "IMXPython.h"
#include "base/singleton_object.h"

namespace mxpy
{
    class MXPythonUtil
        : public IMXPythonUtil
        , public mxkit::InterfaceImp<MXPythonUtil>
        , public mxkit::SingletonObject<MXPythonUtil>
    {
    public:
        virtual mxkit::Result ReleaseString(char* buffer) override;

    public:
        char* AllocString(const char* str);
    };

}