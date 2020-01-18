#pragma once

#if _PY_VER_==37
#include <functional>

#include "MXDllExportDefine.h"
#include "MXSingleObject.h"
#include "IMXPython.h"
#include "Python.h"
#include "MXPy.h"

namespace mxpy
{
    class MXPython37
        : public IMXPython37
        , public MXPy<MXPython37>
        , public mxtoolkit::MXInterfaceImp<MXPython37>
        , public mxtoolkit::MXSingleObject<MXPython37>
	{
	public:
        MXPython37();
		~MXPython37();

        virtual mxtoolkit::Result Initialize(const char* dir) override;

        virtual mxtoolkit::Result Uninstall() override;

        virtual mxtoolkit::Result ExcuteFile(const char* file) override;

        virtual mxtoolkit::Result ExcuteMethod(const char* file, const char* method, int param, int* result) override;
        virtual mxtoolkit::Result ExcuteMethod(const char* file, const char* method, int* result, const char* paramFormat, ...) override;

        virtual mxtoolkit::Result ExcuteMethod(const char* file, const char* method, float param, float* result) override;
        virtual mxtoolkit::Result ExcuteMethod(const char* file, const char* method, float* result, const char* paramFormat, ...) override;

        virtual mxtoolkit::Result ExcuteMethod(const char* file, const char* method, const char* param, char** result) override;
        virtual mxtoolkit::Result ExcuteMethod(const char* file, const char* method, char** result,const char* paramFormat, ...) override;

    protected:
        mxtoolkit::Result CallMethod(const char* file, const char* method, const char* paramFormat, va_list* paramList, std::function<void(PyObject*)> resultFunction);
        
        template<typename ParamType>
        mxtoolkit::Result CallMethod(const char* file, const char* method, const char* paramFormat, ParamType param, std::function<void(PyObject*)> resultFunction)
        {
            MXPyGILUtil pyGILUtil;
            printf("MXPython37::ExcuteMethod file:%s, method:%s.\n", file, method);

            if (!Py_IsInitialized())
            {
                RETURN_RESULT(false);
            }

            PyErr_Clear();
            std::string dir, name;
            if (!mxtoolkit::Path::GetFilePathInfo(file, &dir, &name, nullptr))
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
                int pos = dir.rfind(L'\\');
                int len = dir.length();
                while (pos != std::wstring::npos && pos == (len - 1))
                {
                    dir.resize(dir.size() - 1);
                    pos = dir.rfind(L'\\');
                    len = dir.length();
                }

                mxtoolkit::ReplaceString<std::string>(dir, "\\", "/");
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