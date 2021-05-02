// Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "mxkit_library.h"
#include "IPython.h"

#include "base/path_utils.h"

#include "win32/win_path.h"

int main()
{
    std::cout << "Hello World!\n";

    
    mxkit::LibraryObject mxDllObj;
    mxkit::HModule mxModule = mxkit::LoadLibrary<std::string>(mxDllObj, "MXPython.dll");
    //MX_LOAD_LIBRARY_OBJECT(mxDllObj, "MXPython.dll");

    std::cout << "GetLastError: " << (int)GetLastError() << std::endl;
    if (mxDllObj.init)
        mxDllObj.init();

    mxkit::ExportInfo* all_export = nullptr;
    if (mxDllObj.queryExport)
    {
        mxDllObj.queryExport(&all_export);
    }

    mxkit::InterfaceInfo info = *all_export->interfaceInfo;

    mx::IPythonUtils* pyUtils = nullptr;
    if (mxDllObj.queryInterface)
    {
        mxkit::Result r = mxDllObj.queryInterface(&info, (void**)&pyUtils);
        if (!mxkit::ResultUtils::Success(r))
            return 0;
    }

    std::string exeDir = mxkit::Win32Path<std::string>::CurrentDirectory();
    exeDir += "..\\";

    mx::IPython* py37 = nullptr;
    pyUtils->Alloc(
        "C:\\Users\\muxin\\Desktop\\GitHub\\MXPython\\Python3.7.1\\python37.dll",
        "C:\\Users\\muxin\\Desktop\\GitHub\\MXPython\\Python3.7.1\\",
        &py37);

    mx::IPython* py27 = nullptr;
    pyUtils->Alloc(
        "C:\\Users\\muxin\\Desktop\\GitHub\\MXPython\\Python2.7.13\\python27.dll",
        "C:\\Users\\muxin\\Desktop\\GitHub\\MXPython\\Python2.7.13\\",
        &py27);

    std::string file = exeDir + "test.py";
    if (py37)
    {
        py37->ExcuteFile(file.c_str());
        //file = "C:\\Users\\muxin\\Desktop\\p.py";
        //py37->ExcuteFile(file.c_str());

        //py37->ExcuteMethod("C:\\Users\\mx\\Desktop\\GitHub\\MXMoney\\py\\danjuan_zuhe.py", "get_all_zuhe", "dsadas", nullptr);
//        py37->ExcuteMethod(file.c_str(), "Hello", nullptr, nullptr);
//
//         char* ret = nullptr;
//         py37->ExcuteMethod(file.c_str(), "GetStr", nullptr, &ret);
//         if (ret)
//         {
//             printf("%s\n",ret);            
//         }
// 
//         py37->ExcuteMethod(file.c_str(), "PrintStr", "CPP.", nullptr);

        //py37->ExcuteMethod(file.c_str(), "testAdd", (int*)nullptr, "f,i,f",5.02, 2, (float)3);
        //int ii[3] = { 1,2,3 };
        //py37->ExcuteMethod(file.c_str(), "testAdd", ii, 3, nullptr);
    }

    if (py27)
        py27->ExcuteFile(file.c_str());

    if(py37)
        py37->Release();

    if (py27)
        py27->Release();

    mxDllObj.uninit();

    mxkit::FreeLibrary<true>(mxModule);

    ::system("pause");

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
