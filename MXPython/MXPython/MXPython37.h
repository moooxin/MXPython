#pragma once

#if _PY_VER_==37
#include <functional>

#include "base/singleton_object.h"
#include "base/string_convert.h"
#include "base/path_utils.h"

#include "IMXPython.h"
#include "Python.h"
#include "PythonConfig.h"

namespace mxpy
{
    class MXPython37
        : public IMXPython37
        , public PythonConfig<MXPython37>
        , public mxkit::InterfaceImp<MXPython37>
        , public mxkit::SingletonObject<MXPython37>
	{
	public:
        MXPython37();
		~MXPython37();

        virtual mxkit::Result Initialize(const char* dir) override;

        virtual mxkit::Result Uninstall() override;

        virtual mxkit::Result ExcuteFile(const char* file) override;

        virtual mxkit::Result ExcuteMethod(const char* file, const char* method, int param, int* result) override;
        virtual mxkit::Result ExcuteMethod(const char* file, const char* method, int* result, const char* paramFormat, ...) override;

        virtual mxkit::Result ExcuteMethod(const char* file, const char* method, float param, float* result) override;
        virtual mxkit::Result ExcuteMethod(const char* file, const char* method, float* result, const char* paramFormat, ...) override;

        virtual mxkit::Result ExcuteMethod(const char* file, const char* method, const char* param, char** result) override;
        virtual mxkit::Result ExcuteMethod(const char* file, const char* method, char** result,const char* paramFormat, ...) override;

    protected:
        mxkit::Result CallMethod(const char* file, const char* method, const char* paramFormat, va_list* paramList, std::function<void(PyObject*)> resultFunction);
        
        template<typename ParamType>
        mxkit::Result CallMethod(const char* file, const char* method, const char* paramFormat, ParamType param, std::function<void(PyObject*)> resultFunction)
        {
            PyGILUtil pyGILUtil;
            printf("MXPython37::ExcuteMethod file:%s, method:%s.\n", file, method);

            if (!Py_IsInitialized())
            {
                RETURN_RESULT(false);
            }

            PyErr_Clear();
            std::string dir, name;
            if (!mxkit::PathUtils::FileInfo<std::string>(file, &dir, &name, nullptr))
            {
                RETURN_RESULT(false);
            }

            PyRun_SimpleString("import os,sys");

            if (dir.empty())
            {
                PyRun_SimpleString("sys.path.append('./')");
                PyErr_Print();
            }
            else
            {
                mxkit::StringUtils::EraseLast<std::string>(dir, _MX_DIR_STRING_A);

                mxkit::StringUtils::Replace<std::string>(dir, "\\", "/");
                std::string pyCmd = "sys.path.append('" + dir;
                pyCmd += "')";
                PyRun_SimpleString(pyCmd.c_str());
                PyErr_Print();
            }

            PyObject* pyModule = PyImport_ImportModule(name.c_str());
            if (!pyModule)
            {
                PyErr_Print();
                RETURN_RESULT(false);
            }

            PyObject* pyMethod = PyObject_GetAttrString(pyModule, method);
            if (!pyMethod)
            {
                RETURN_RESULT(false);
            }
            
            //调用函数
            PyObject* retValue = nullptr;
            if (param)
                retValue = PyObject_CallFunction(pyMethod, paramFormat, param); 
            else
                retValue = PyObject_CallObject(pyMethod, nullptr);

            PyErr_Print();

            if (retValue && resultFunction)
            {
                resultFunction(retValue);
#ifdef NDEBUG
                Py_DECREF(retValue);
#endif
            }

            //清除错误
            PyErr_Clear();

#ifdef NDEBUG
            // Clean up
            Py_DECREF(pyModule);
#endif
            printf("MXPython37::ExcuteMethod completed.\n");
            RETURN_RESULT(true);
        }

	};

}
#endif