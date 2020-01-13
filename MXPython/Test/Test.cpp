// Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "MXDllExportDefine.h"
#include "IMXPython.h"

#include "Win32PathUtil.h"



int main()
{
    std::cout << "Hello World!\n";

    
    mxtoolkit::MXDllObject mxDllObj;
    HMODULE mxDll = MXInitDll(mxDllObj, "MXPython.dll");

    std::cout << "GetLastError: " << (int)GetLastError << std::endl;
    if (mxDllObj.dllInit)
        mxDllObj.dllInit();

    mxtoolkit::MXDllExportInfo* all_export = nullptr;
    if (mxDllObj.getExportInfo)
    {
        mxDllObj.getExportInfo(&all_export);
    }

    mxtoolkit::MXInterfaceInfo info = *all_export->interfaceInfo;

    mxpy::IMXPython37* py37 = nullptr;
    if (mxDllObj.getInterfaceInfo)
    {
        if (!mxDllObj.getInterfaceInfo(&info, (void**)&py37))
            return 0;
    }

    std::string exeDir = mxtoolkit::Win32App<std::string>::CurrentDirectory();
    if (py37->Initialize(exeDir.c_str()))
    {
        std::string file = exeDir + "testMain.py";
        py37->ExcuteFile(file.c_str());
        file = exeDir + "test123.py";
        py37->ExcuteMethod(file.c_str(), "Hello", nullptr, nullptr);

        char* ret = nullptr;
        py37->ExcuteMethod(file.c_str(), "GetStr", nullptr, &ret);
        if (ret)
        {
            printf("%s\n",ret);            
        }

        py37->ExcuteMethod(file.c_str(), "PrintStr", "CPP.", nullptr);
    }

    py37->Uninstall();

    mxDllObj.dllUninit();

    FreeLibrary(mxDll);

    int i;
    std::cin >> i;
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
