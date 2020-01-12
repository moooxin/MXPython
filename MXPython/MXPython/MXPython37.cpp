#include "stdafx.h"
#if _PY_VER_==37
#include "MXPython37.h"
#include "MXStringKit.h"
#include "MXPythonUtil.h"

namespace mxpy
{

	MXPython37::MXPython37()
	{
	}


	MXPython37::~MXPython37()
	{
		int ret = Py_IsInitialized();
		if (ret == 0)
			Py_Finalize();
	}

    mxtoolkit::Result MXPython37::Initialize(const CHAR* dir)
    {
		//WCHAR filePath[MAX_PATH];
		//::GetModuleFileNameW(NULL, filePath, MAX_PATH);
		//::PathRemoveFileSpecW(filePath);

        std::wstring path;
        mxtoolkit::WAConvert<std::string,std::wstring>(dir, &path);

		std::wstring python_home(path.c_str());
		python_home += L"\\ph37\\";
		Py_SetPythonHome(python_home.c_str());

		Py_Initialize();
		int ret = Py_IsInitialized();
		if (ret != 0)
		{
			printf("python init fail:\n");
			PyErr_Print();
			printf("\n");

			RETURN_RESULT(false);
		}

		m_version = Py_GetVersion();
		printf("python version:%s.\n", m_version.c_str());

		RETURN_RESULT(true);
	}

    mxtoolkit::Result MXPython37::Uninstall()
    {
        RETURN_RESULT(true);
    }

    mxtoolkit::Result MXPython37::ExcuteFile(const CHAR* file)
	{
        if (!Py_IsInitialized())
            RETURN_RESULT(false);

		FILE *ff = ::_Py_fopen(file, "r+");
		int res = PyRun_SimpleFileEx(ff, file, true);

		PyErr_Print();

        RETURN_RESULT(true);
	}

    mxtoolkit::Result MXPython37::ExcuteMethod(const CHAR* file, const CHAR* method, const CHAR* param, char** result)
    {
        if (!Py_IsInitialized())
        {
            RETURN_RESULT(false);
        }

        PyObject* pyModule = PyImport_ImportModule(file);
        if (!pyModule)
        {
            RETURN_RESULT(false);
        }

        PyObject* pyMethod = PyObject_GetAttrString(pyModule, method);
        if (!pyMethod)
        {
            RETURN_RESULT(false);
        }

        //调用函数
        PyObject* retValue = PyObject_CallFunction(pyMethod, param);

        PyErr_Print();

        if (retValue)
        {
            char* retStr = nullptr;
            PyArg_Parse(retValue, "s", &retStr);

            if (retStr)
            {
                *result = MXPythonUtil::GetInstance()->AllocString(retStr);
            }

#ifdef NDEBUG
            Py_DECREF(retValue);
#endif
        }

#ifdef NDEBUG
        // Clean up
        Py_DECREF(pyModule);
#endif
        RETURN_RESULT(true);
    }

}

#endif