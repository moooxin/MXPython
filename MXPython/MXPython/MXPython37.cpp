#include "stdafx.h"
#include "MXPython37.h"
#include "MXStringKit.h"

#if _PY_VER_==37
namespace mxpython
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

	bool MXPython37::Init(const CHAR* dir)
	{
		//WCHAR filePath[MAX_PATH];
		//::GetModuleFileNameW(NULL, filePath, MAX_PATH);
		//::PathRemoveFileSpecW(filePath);

        std::wstring path;
        mxtoolkit::AWConvert(dir, &path);

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

			return false;
		}

		m_version = Py_GetVersion();
		printf("python version:%s.\n", m_version.c_str());

		return true;;
	}

	bool MXPython37::RunFile(const CHAR* file)
	{
        if (!Py_IsInitialized())
            return false;

		FILE *ff = ::_Py_fopen(file, "r+");
		int res = PyRun_SimpleFileEx(ff, file, true);

		PyErr_Print();

		return true;
	}
}

#endif