#pragma once

#include <functional>
#include <memory>

#include "base/singleton_object.h"
#include "base/string_convert.h"
#include "base/path_utils.h"

#include "IPython.h"
#include "PythonConfig.h"

#include "PythonApi.h"

class Python2Api;

class Python27
    : public mx::IPython
    , public PythonConfig<Python27>
{
public:
    Python27();
    ~Python27();

    mxkit::Result Initialize(const PythonApi::Info& info, const char* homeDir);

public:

    virtual mxkit::Result Version(mx::PythonVersion* ver) override;

    virtual mxkit::Result Release() override;

    virtual mxkit::Result ExcuteFile(const char* file) override;

    virtual mxkit::Result ExcuteMethod(const char* file, const char* method, int param, int* result) override;
    virtual mxkit::Result ExcuteMethod(const char* file, const char* method, int* result, const char* paramFormat, ...) override;

    virtual mxkit::Result ExcuteMethod(const char* file, const char* method, float param, float* result) override;
    virtual mxkit::Result ExcuteMethod(const char* file, const char* method, float* result, const char* paramFormat, ...) override;

    virtual mxkit::Result ExcuteMethod(const char* file, const char* method, const char* param, char** result) override;
    virtual mxkit::Result ExcuteMethod(const char* file, const char* method, char** result, const char* paramFormat, ...) override;

protected:
    std::shared_ptr<Python2Api> m_api;
};

