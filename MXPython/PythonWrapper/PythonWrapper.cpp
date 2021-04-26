// PythonWrapper.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "Python.h"
#include "base/string_convert.h"


#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#if _PY_VER_==27

#define OPEN_FILE(f,p,m) ::fopen_s(&f,p,m)

#else
#define OPEN_FILE(f,p,m) f = ::_Py_fopen(pythonFile.c_str(), "r+")

#endif

int main(int argc, char* argv[])
{
    int i = 0;

    std::string s = "dasdas";
    std::wstring ss;
    //mxkit::WAConvert<std::string,std::wstring>(s.c_str(), &ss);
    mxkit::Win32StringConvert::AnsiiToUnicode(s.c_str(), ss);
    std::wcout << ss << std::endl;
    std::string sss;
    //mxkit::WAConvert<std::wstring, std::string>(ss.c_str(), &sss);
    mxkit::Win32StringConvert::UnicodeToAnsii(ss.c_str(), sss);
    std::cout << sss << std::endl;


    printf("app run path:%s.\n", argv[0]);

    // 开始PythonWrapper
#if _PY_VER_==27
    CHAR filePath[MAX_PATH];
    ::GetModuleFileNameA(NULL, filePath, MAX_PATH);
    ::PathRemoveFileSpecA(filePath);
    std::string python_home(filePath);
    python_home += "\\ph27\\";
    Py_SetPythonHome((char*)python_home.c_str());

#else
    WCHAR filePath[MAX_PATH];
    ::GetModuleFileNameW(NULL, filePath, MAX_PATH);
    ::PathRemoveFileSpecW(filePath);
    std::wstring python_home(filePath);
    python_home += L"\\ph37\\";
    Py_SetPythonHome(python_home.c_str());

#endif

    Py_Initialize();

    //检查初始化是否完成
    if (!Py_IsInitialized())
    {
        printf("python init fail:\n");
        PyErr_Print();
        printf("\n");

        ::system("pause");
        return -1;
    }

    const char* ver = Py_GetVersion();
    printf("python version:%s.\n", ver);

    //
StartRunPW:

    std::string pythonFile;
    if (argc <= 1)
    {
    InputPythonFile:
        printf("please input a python file or exit:\n");
        std::cin >> pythonFile;
        if (pythonFile.empty())
        {
            printf("input none !\n");
            goto InputPythonFile;
        }

        if (pythonFile == "exit")
        {
            //关闭Python解释器
            Py_Finalize();
            return 0;
        }

        int pos = pythonFile.rfind(".py");
        if (pos == std::string::npos || pos != (pythonFile.length() - 3))
        {
            printf("not a python file!\n");
            goto InputPythonFile;
        }

        if (!::PathFileExistsA(pythonFile.c_str()))
        {
            printf("file not exist!\n");
            goto InputPythonFile;
        }
    }
    else
    {
        pythonFile = argv[1];//第一个传入的python文件
    }

    FILE *ff = nullptr;
    OPEN_FILE(ff, pythonFile.c_str(), "r+");
   
    PyCompilerFlags cf;
    cf.cf_flags = 0;
  
#if _PY_VER_==27
    std::string name = pythonFile.substr(pythonFile.rfind('\\') + 1);

    int res = PyRun_SimpleFileEx(ff, name.c_str(), false);
    if (ff)
    {
        ::fclose(ff);
        ff = nullptr;
    }

#else
    int res = PyRun_SimpleFileEx(ff, pythonFile.c_str(), true);

#endif

    PyErr_Print();
    
    argc = 0;
    goto StartRunPW;

    return 0;
}

