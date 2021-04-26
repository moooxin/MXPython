#include "stdafx.h"
#if _PY_VER_==37
#include "MXPython37.h"
#include "base/string_convert.h"
#include "base/path_utils.h"
#include "MXPythonUtil.h"
#include "PyGILUtil.h"

#include "win32/file_utils.h"

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

    mxkit::Result MXPython37::Initialize(const char* dir)
    {
        if (mxkit::FileSystem::FileType<>(dir) != mxkit::FileSystem::FOLDER)
        {
            RETURN_RESULT(false);
        }
                
        std::wstring path;
        //mxkit::WAConvert<std::string,std::wstring>(dir, &path);
        mxkit::Win32StringConvert::Utf8ToUnicode(dir, path);

        Py_SetPythonHome(path.c_str());

        try
        {
            Py_Initialize();
        }
        catch (...)
        {
        }

		int ret = Py_IsInitialized();
		if (ret == 0)
		{
			printf("MXPython37::Initialize fail:\n");
			PyErr_Print();
			printf("\n");

			RETURN_RESULT(false);
		}

        //mxkit::WAConvert<std::wstring, std::string>(path.c_str(), &m_homePath);
        mxkit::Win32StringConvert::UnicodeToAnsii(path.c_str(), m_homePath);
        InitConfig(Py_GetVersion(), m_homePath.c_str());

        printf("MXPython37::Initialize Completed !\n");
        printf("Python Version : %s\n", m_version.c_str());
        printf("Python Home: %s.\n", m_homePath.c_str());

		RETURN_RESULT(true);
	}

    mxkit::Result MXPython37::Uninstall()
    {
        if (Py_IsInitialized())
        {
            PyGILUtil pyGILUtil;
            Py_Finalize();
        }

        RETURN_RESULT(true);
    }

    mxkit::Result MXPython37::ExcuteFile(const char* file)
    {
        PyGILUtil pyGILUtil;
        printf("MXPython37::ExcuteFile:%s.\n", file);

        if (!Py_IsInitialized())
            RETURN_RESULT(false);

		FILE *pyFile = ::_Py_fopen(file, "r+");
        if (!pyFile)
            RETURN_RESULT(false);

		int res = PyRun_SimpleFileEx(pyFile, file, true);

		PyErr_Print();
        PyErr_Clear();

        printf("MXPython37::ExcuteFile completed: %d.\n", res);
        RETURN_RESULT(true);
	}

    mxkit::Result MXPython37::ExcuteMethod(const char* file, const char* method, int param, int* result)
    {
        return CallMethod<int>(file, method, "i", param, [&](PyObject* retValue)
        {
            int retVal = 0;
            if (retValue && result)
                PyArg_Parse(retValue, "i", &retVal);

            if (retValue && result)
                *result = retVal;
        });
    }

    mxkit::Result MXPython37::ExcuteMethod(const char* file, const char* method, int* result, const char* paramFormat, ...)
    {
        va_list paramList = nullptr;
        va_start(paramList, paramFormat);
        mxkit::Result rt = CallMethod(file, method, paramFormat, &paramList, [&](PyObject* retValue)
        {
            int retVal = 0;
            if (retValue && result)
                PyArg_Parse(retValue, "i", &retVal);

            if (retValue && result)
                *result = retVal;
        });

        va_end(paramList);
        return rt;
    }

    mxkit::Result MXPython37::ExcuteMethod(const char* file, const char* method, float param, float* result)
    {
        return CallMethod<float>(file, method, "f", param, [&](PyObject* retValue)
        {
            float retVal = 0.0;
            if (retValue && result)
                PyArg_Parse(retValue, "f", &retVal);

            if (retValue && result)
                *result = retVal;
        });
    }

    mxkit::Result MXPython37::ExcuteMethod(const char* file, const char* method, float* result, const char* paramFormat, ...)
    {
        va_list paramList = nullptr;
        va_start(paramList, paramFormat);

        mxkit::Result rt = CallMethod(file, method, paramFormat, &paramList, [&](PyObject* retValue)
        {
            float retVal = 0.0;
            if (retValue && result)
                PyArg_Parse(retValue, "f", &retVal);

            if (retValue && result)
                *result = retVal;
        });

        va_end(paramList);
        return rt;
    }

    mxkit::Result MXPython37::ExcuteMethod(const char* file, const char* method, const char* param, char** result)
    {
        return CallMethod<const char*>(file, method, "s", param, [&](PyObject* retValue)
        {
            char* retVal = nullptr;
            if (retValue && result)
                PyArg_Parse(retValue, "s", &retVal);

            if (retVal && result)
                *result = MXPythonUtil::Instance()->AllocString(retVal);
        });
    }

    mxkit::Result MXPython37::ExcuteMethod(const char* file, const char* method, char** result, const char* paramFormat, ...)
    {
        va_list paramList = nullptr;
        va_start(paramList, paramFormat);

        mxkit::Result rt = CallMethod(file, method, paramFormat, &paramList, [&](PyObject* retValue)
        {
            char* retVal = nullptr;
            if (retValue && result)
                PyArg_Parse(retValue, "s", &retVal);

            if (retVal && result)
                *result = MXPythonUtil::Instance()->AllocString(retVal);
        });

        va_end(paramList);
        return rt;
    }

    mxkit::Result MXPython37::CallMethod(const char* file, const char* method, const char* paramFormat, va_list* paramList, std::function<void(PyObject*)> resultFunction)
    {
        PyGILUtil pyGILUtil;
        printf("MXPython37::ExcuteMethod file:%s, method:%s.\n", file, method);

        if (!paramFormat)
        {
            RETURN_RESULT(false);
        }

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

        //判断参数个数 ------
        int paramCount = 1;
        const char* formatStr = paramFormat;
        while (formatStr = strstr(formatStr, ","))
        {
            paramCount++;
            ++formatStr;
        }

        //调用函数 --------
        std::vector<mxkit::uint64> params;
        int loopCnt = paramCount;
        while (loopCnt--)
            params.push_back(va_arg(*paramList, mxkit::uint64));

        PyObject* retValue = nullptr;

        switch (paramCount)
        {
        case 1:
            retValue = PyObject_CallFunction(pyMethod, paramFormat, params[0]); break;
        case 2:
            retValue = PyObject_CallFunction(pyMethod, paramFormat, params[0], params[1]); break;
        case 3:
            retValue = PyObject_CallFunction(pyMethod, paramFormat, params[0], params[1], params[2]); break;
        case 4:
            retValue = PyObject_CallFunction(pyMethod, paramFormat, params[0], params[1], params[2], params[3]); break;
        case 5:
            retValue = PyObject_CallFunction(pyMethod, paramFormat, params[0], params[1], params[2], params[3], params[4]); break;
        case 6:
            retValue = PyObject_CallFunction(pyMethod, paramFormat, params[0], params[1], params[2], params[3], params[4], params[5]); break;
        case 7:
            retValue = PyObject_CallFunction(pyMethod, paramFormat, params[0], params[1], params[2], params[3], params[4], params[5], params[6]); break;
        case 8:
            retValue = PyObject_CallFunction(pyMethod, paramFormat, params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]); break;
        case 9:
            retValue = PyObject_CallFunction(pyMethod, paramFormat, params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8]); break;
        case 10:
            retValue = PyObject_CallFunction(pyMethod, paramFormat, params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9]); break;
            break;
        default:break;
        }

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
        printf("MXPython37::CallMethod completed.\n");
        RETURN_RESULT(true);
    }

}

#endif