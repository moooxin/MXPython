#pragma once
#include "IPython.h"
#include "base/singleton_object.h"


class PythonUtils
    : public mx::IPythonUtils
    , public mxkit::InterfaceImp<PythonUtils>
    , public mxkit::SingletonObject<PythonUtils>
{
public:

    virtual mxkit::Result Alloc(const char* pythonDll, const char* homeDir, mx::IPython** pp) override;

    virtual mxkit::Result Release(Type type, void* buffer) override;

public:
    char* AllocString(const char* str);
};

