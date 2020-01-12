// MXPython.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "MXDllExportDefine.h"



namespace mxpython {

	MX_C_EXPORT mxtoolkit::mx_call_result mx_dll_init()
	{
		return mxtoolkit::mx_call_successed;
	}

	MX_C_EXPORT void mx_dll_uninit()
	{

	}

	MX_C_EXPORT unsigned int mx_dll_all_export(mxtoolkit::mx_dll_export_info **)
	{
		return mxtoolkit::mx_call_fail;
	}
	
	MX_C_EXPORT unsigned int mx_dll_get_interface(const mxtoolkit::mx_export_interface_info*, void**)
	{
		return mxtoolkit::mx_call_fail;
	}
}