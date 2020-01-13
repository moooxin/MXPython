#include "stdafx.h"
#if _PY_VER_==37
#include "MXPython37.h"
#include "MXStringKit.h"
#include "MXPythonUtil.h"
#include "MXPath.h"
#include "MXPyGILUtil.h"

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
		if (ret == 0)
		{
			printf("MXPython37::Initialize fail:\n");
			PyErr_Print();
			printf("\n");

			RETURN_RESULT(false);
		}

		m_version = Py_GetVersion();
		printf("MXPython37::Initialize Python Version:%s.\n", m_version.c_str());

		RETURN_RESULT(true);
	}

    mxtoolkit::Result MXPython37::Uninstall()
    {
        if (Py_IsInitialized())
        {
            MXPyGILUtil pyGILUtil;
            Py_Finalize();
        }

        RETURN_RESULT(true);
    }

    mxtoolkit::Result MXPython37::ExcuteFile(const CHAR* file)
    {
        MXPyGILUtil pyGILUtil;
        printf("MXPython37::ExcuteFile:%s.\n", file);

        if (!Py_IsInitialized())
            RETURN_RESULT(false);

		FILE *pyFile = ::_Py_fopen(file, "r+");
        if (!pyFile)
            RETURN_RESULT(false);

		int res = PyRun_SimpleFileEx(pyFile, file, true);

		PyErr_Print();

        printf("MXPython37::ExcuteFile completed: %d.\n", res);
        RETURN_RESULT(true);
	}

    mxtoolkit::Result MXPython37::ExcuteMethod(const CHAR* file, const CHAR* method, const CHAR* param, char** result)
    {
        MXPyGILUtil pyGILUtil;
        printf("MXPython37::ExcuteMethod file:%s, method:%s.\n", file, method);

        if (!Py_IsInitialized())
        {
            RETURN_RESULT(false);
        }

        std::string dir, name;
        if (!mxtoolkit::Path::GetFilePathInfo(file, &dir, &name, nullptr))
        {
            RETURN_RESULT(false);
        }

        if (dir.empty())
        {
            PyRun_SimpleString("sys.path.append('./')");
        }
        else
        {
            int len = dir.length();
            if(dir.at(len-1) == '\\')
                dir.resize(dir.length()-1);

            std::string pyCmd = "sys.path.append('" + dir;
            pyCmd += "')";
            PyRun_SimpleString(pyCmd.c_str());
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
            retValue = PyObject_CallFunction(pyMethod, "s", param);
        else
            retValue = PyObject_CallObject(pyMethod, nullptr);

        PyErr_Print();
        
        if (retValue)
        {
            char* retStr = nullptr;
            PyArg_Parse(retValue, "s", &retStr);

            if (retStr)
            {
                *result = MXPythonUtil::GetInstance()->AllocString(retStr);
            }

            //这里不一定是返回的字符串，所以清除这个格式化的错误
            PyErr_Clear();
#ifdef NDEBUG
            Py_DECREF(retValue);
#endif
        }

#ifdef NDEBUG
        // Clean up
        Py_DECREF(pyModule);
#endif
        printf("MXPython37::ExcuteMethod completed.\n");
        RETURN_RESULT(true);
    }

}

#endif