#include "stdafx.h"

#include "Python3X.h"
#include "base/string_convert.h"
#include "base/path_utils.h"
#include "PythonUtils.h"
#include "Python3Api.h"

#include "win32/file_utils.h"


#include "Python3Api.h"


namespace py3
{

    template<typename ParamType>
    mxkit::Result CallMethod_t(Python3Api::Api& api, const char* file, const char* method, const char* paramFormat, ParamType param, std::function<void(PyObject*)> resultFunction)
    {
        Python3Api::GILUtils pyGILUtil(api);
        printf("Python3X::ExcuteMethod file:%s, method:%s.\n", file, method);

        if (!api.IsInitialized())//Py_IsInitialized())
        {
            RETURN_RESULT(false);
        }

        api.Err_Clear(); //PyErr_Clear();
        std::string dir, name;
        if (!mxkit::PathUtils::FileInfo<std::string>(file, &dir, &name, nullptr))
        {
            RETURN_RESULT(false);
        }

        api.Run_SimpleStringFlags("import os,sys", NULL); //PyRun_SimpleString("import os,sys");

        if (dir.empty())
        {
            api.Run_SimpleStringFlags("sys.path.append('./')", NULL); //PyRun_SimpleString("sys.path.append('./')");
            api.Err_Print(); //PyErr_Print();
        }
        else
        {
            mxkit::StringUtils::EraseLast<std::string>(dir, _MX_DIR_STRING_A);

            mxkit::StringUtils::Replace<std::string>(dir, "\\", "/");
            std::string pyCmd = "sys.path.append('" + dir;
            pyCmd += "')";
            api.Run_SimpleStringFlags(pyCmd.c_str(), nullptr); //PyRun_SimpleString(pyCmd.c_str());
            api.Err_Print(); //PyErr_Print();
        }

        PyObject* pyModule = api.Import_ImportModule(name.c_str()); //PyImport_ImportModule(name.c_str());
        if (!pyModule)
        {
            api.Err_Print(); //PyErr_Print();
            RETURN_RESULT(false);
        }

        PyObject* pyMethod = api.Object_GetAttrString(pyModule, method); //PyObject_GetAttrString(pyModule, method);
        if (!pyMethod)
        {
            RETURN_RESULT(false);
        }

        //调用函数
        PyObject* retValue = nullptr;
        if (param)
            retValue = api.Object_CallFunction(pyMethod, paramFormat, param);
        else
            retValue = api.Object_CallObject(pyMethod, nullptr);

        api.Err_Print(); //PyErr_Print();

        if (retValue && resultFunction)
        {
            resultFunction(retValue);
#ifdef NDEBUG
            Py_DECREF(retValue);
#endif
        }

        //清除错误
        api.Err_Clear(); //PyErr_Clear();

#ifdef NDEBUG
        // Clean up
        Py_DECREF(pyModule);
#endif
        printf("Python3X::ExcuteMethod completed.\n");
        RETURN_RESULT(true);
    }

    mxkit::Result CallMethod(Python3Api::Api& api, const char* file, const char* method, const char* paramFormat, va_list* paramList, std::function<void(PyObject*)> resultFunction)
    {
        Python3Api::GILUtils pyGILUtil(api);
        printf("Python3X::ExcuteMethod file:%s, method:%s.\n", file, method);

        if (!paramFormat)
        {
            RETURN_RESULT(false);
        }

        if (!api.IsInitialized())//Py_IsInitialized())
        {
            RETURN_RESULT(false);
        }

        api.Err_Clear(); //PyErr_Clear();
        std::string dir, name;
        if (!mxkit::PathUtils::FileInfo<std::string>(file, &dir, &name, nullptr))
        {
            RETURN_RESULT(false);
        }

        api.Run_SimpleStringFlags("import os,sys", NULL);// PyRun_SimpleString("import os,sys");

        if (dir.empty())
        {
            api.Run_SimpleStringFlags("sys.path.append('./')", NULL);// PyRun_SimpleString("sys.path.append('./')");
            api.Err_Print(); //PyErr_Print();
        }
        else
        {
            mxkit::StringUtils::EraseLast<std::string>(dir, _MX_DIR_STRING_A);

            mxkit::StringUtils::Replace<std::string>(dir, "\\", "/");
            std::string pyCmd = "sys.path.append('" + dir;
            pyCmd += "')";
            api.Run_SimpleStringFlags(pyCmd.c_str(), NULL);
            //PyRun_SimpleString(pyCmd.c_str());
            api.Err_Print(); //PyErr_Print();
        }

        PyObject* pyModule = api.Import_ImportModule(name.c_str()); //PyImport_ImportModule(name.c_str());
        if (!pyModule)
        {
            api.Err_Print(); //PyErr_Print();
            RETURN_RESULT(false);
        }

        PyObject* pyMethod = api.Object_GetAttrString(pyModule, method);// PyObject_GetAttrString(pyModule, method);
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
            retValue = api.Object_CallFunction(pyMethod, paramFormat, params[0]); break;
        case 2:
            retValue = api.Object_CallFunction(pyMethod, paramFormat, params[0], params[1]); break;
        case 3:
            retValue = api.Object_CallFunction(pyMethod, paramFormat, params[0], params[1], params[2]); break;
        case 4:
            retValue = api.Object_CallFunction(pyMethod, paramFormat, params[0], params[1], params[2], params[3]); break;
        case 5:
            retValue = api.Object_CallFunction(pyMethod, paramFormat, params[0], params[1], params[2], params[3], params[4]); break;
        case 6:
            retValue = api.Object_CallFunction(pyMethod, paramFormat, params[0], params[1], params[2], params[3], params[4], params[5]); break;
        case 7:
            retValue = api.Object_CallFunction(pyMethod, paramFormat, params[0], params[1], params[2], params[3], params[4], params[5], params[6]); break;
        case 8:
            retValue = api.Object_CallFunction(pyMethod, paramFormat, params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]); break;
        case 9:
            retValue = api.Object_CallFunction(pyMethod, paramFormat, params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8]); break;
        case 10:
            retValue = api.Object_CallFunction(pyMethod, paramFormat, params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9]); break;
            break;
        default:break;
        }

        api.Err_Print();// PyErr_Print();

        if (retValue && resultFunction)
        {
            resultFunction(retValue);

#ifdef NDEBUG
            Py_DECREF(retValue);
#endif
        }

        //清除错误
        api.Err_Clear(); //PyErr_Clear();

#ifdef NDEBUG
    // Clean up
        Py_DECREF(pyModule);
#endif
        printf("Python3X::CallMethod completed.\n");
        RETURN_RESULT(true);
    }

}

Python3X::Python3X()
{
}


Python3X::~Python3X()
{
    if (m_api)
    {
        int ret = m_api->api.IsInitialized(); //Py_IsInitialized();
        if (ret == 0)
            m_api->api.Finalize(); //Py_Finalize();
    }
}

mxkit::Result Python3X::Initialize(const PythonApi::Info& info, const char* homeDir)
{
    if (!homeDir || 
        mxkit::FileSystem::FileType<>(homeDir) != mxkit::FileSystem::FOLDER)
    {
        RETURN_RESULT(false);
    }

    m_api = std::make_shared<Python3Api>();
    m_api->Init(info);

    const char* ver = m_api->api.GetVersion();
    std::wstring path;
    //mxkit::WAConvert<std::string,std::wstring>(dir, &path);
    mxkit::Win32StringConvert::Utf8ToUnicode(homeDir, path);
    
    m_api->api.SetPythonHome(path.c_str());// Py_SetPythonHome(path.c_str());

    try
    {
        m_api->api.Initialize(); //Py_Initialize();
    }
    catch (...)
    {
    }

    int ret = m_api->api.IsInitialized(); //Py_IsInitialized();
    if (ret == 0)
    {
        printf("Python3X::Initialize fail:\n");
        m_api->api.Err_Print(); //PyErr_Print();
        printf("\n");

        RETURN_RESULT(false);
    }

    //mxkit::WAConvert<std::wstring, std::string>(path.c_str(), &m_homePath);
    mxkit::Win32StringConvert::UnicodeToAnsii(path.c_str(), m_homePath);
    InitConfig(m_api->api.GetVersion(), m_homePath.c_str());
    //InitConfig(Py_GetVersion(), m_homePath.c_str());

    printf("Python3X::Initialize Completed !\n");
    printf("Python Version : %s\n", m_version.c_str());
    printf("Python Home: %s.\n", m_homePath.c_str());

    RETURN_RESULT(true);
}

mxkit::Result Python3X::Version(mx::PythonVersion* ver)
{
    if(!ver)
    RETURN_RESULT(false);

    *ver = m_api->info().version;
    RETURN_RESULT(true);   
}

mxkit::Result Python3X::Release()
{
    if (m_api->api.IsInitialized())//Py_IsInitialized())
    {
        Python3Api::GILUtils pyGILUtil(m_api->api);
        m_api->api.Finalize(); //Py_Finalize();
    }

    RETURN_RESULT(true);
}

mxkit::Result Python3X::ExcuteFile(const char* file)
{
    Python3Api::GILUtils pyGILUtil(m_api->api);
    printf("Python3X::ExcuteFile:%s.\n", file);

    if (!m_api->api.IsInitialized())//Py_IsInitialized())
        RETURN_RESULT(false);

    FILE* pyFile = m_api->api.fopen(file, "r+");// ::_Py_fopen(file, "r+");
    if (!pyFile)
        RETURN_RESULT(false);

    int res = m_api->api.Run_SimpleFileExFlags(pyFile, file, true, NULL);// PyRun_SimpleFileEx(pyFile, file, true);

    m_api->api.Err_Print(); //PyErr_Print();
    m_api->api.Err_Clear(); //PyErr_Clear();

    printf("Python3X::ExcuteFile completed: %d.\n", res);
    RETURN_RESULT(true);
}

mxkit::Result Python3X::ExcuteMethod(const char* file, const char* method, int param, int* result)
{
    return py3::CallMethod_t<int>(m_api->api, file, method, "i", param, [&](PyObject* retValue)
        {
            int retVal = 0;
            if (retValue && result)
                m_api->api.Arg_Parse(retValue, "i", &retVal);//PyArg_Parse(retValue, "i", &retVal);

            if (retValue && result)
                *result = retVal;
        });
}

mxkit::Result Python3X::ExcuteMethod(const char* file, const char* method, int* result, const char* paramFormat, ...)
{
    va_list paramList = nullptr;
    va_start(paramList, paramFormat);
    mxkit::Result rt = py3::CallMethod(m_api->api, file, method, paramFormat, &paramList, [&](PyObject* retValue)
        {
            int retVal = 0;
            if (retValue && result)
                m_api->api.Arg_Parse(retValue, "i", &retVal);//PyArg_Parse(retValue, "i", &retVal);

            if (retValue && result)
                *result = retVal;
        });

    va_end(paramList);
    return rt;
}

mxkit::Result Python3X::ExcuteMethod(const char* file, const char* method, float param, float* result)
{
    return py3::CallMethod_t<float>(m_api->api, file, method, "f", param, [&](PyObject* retValue)
        {
            float retVal = 0.0;
            if (retValue && result)
                m_api->api.Arg_Parse(retValue, "f", &retVal);//PyArg_Parse(retValue, "f", &retVal);

            if (retValue && result)
                *result = retVal;
        });
}

mxkit::Result Python3X::ExcuteMethod(const char* file, const char* method, float* result, const char* paramFormat, ...)
{
    va_list paramList = nullptr;
    va_start(paramList, paramFormat);

    mxkit::Result rt = py3::CallMethod(m_api->api, file, method, paramFormat, &paramList, [&](PyObject* retValue)
        {
            float retVal = 0.0;
            if (retValue && result)
                m_api->api.Arg_Parse(retValue, "f", &retVal);//PyArg_Parse(retValue, "f", &retVal);

            if (retValue && result)
                *result = retVal;
        });

    va_end(paramList);
    return rt;
}

mxkit::Result Python3X::ExcuteMethod(const char* file, const char* method, const char* param, char** result)
{
    return py3::CallMethod_t<const char*>(m_api->api, file, method, "s", param, [&](PyObject* retValue)
        {
            char* retVal = nullptr;
            if (retValue && result)
                m_api->api.Arg_Parse(retValue, "s", &retVal);//PyArg_Parse(retValue, "s", &retVal);

            if (retVal && result)
                *result = PythonUtils::Instance()->AllocString(retVal);
        });
}

mxkit::Result Python3X::ExcuteMethod(const char* file, const char* method, char** result, const char* paramFormat, ...)
{
    va_list paramList = nullptr;
    va_start(paramList, paramFormat);

    mxkit::Result rt = py3::CallMethod(m_api->api, file, method, paramFormat, &paramList, [&](PyObject* retValue)
        {
            char* retVal = nullptr;
            if (retValue && result)
                m_api->api.Arg_Parse(retValue, "s", &retVal);//PyArg_Parse(retValue, "s", &retVal);

            if (retVal && result)
                *result = PythonUtils::Instance()->AllocString(retVal);
        });

    va_end(paramList);
    return rt;
}

