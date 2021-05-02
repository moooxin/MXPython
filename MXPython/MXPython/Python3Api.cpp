#include "stdafx.h"
#include "Python3Api.h"



Python3Api::~Python3Api()
{
    PythonApi::Free(&m_info);
}

bool Python3Api::Init(const PythonApi::Info& info)
{
    if (!info.dllModule)
        return false;

    m_info = info;
    HMODULE& mxModule = m_info.dllModule;

    _GET_PROCESS_ADDRESS_(mxModule, Py_GetVersion, api.GetVersion);

    _GET_PROCESS_ADDRESS_(mxModule, Py_SetPythonHome, api.SetPythonHome);
    _GET_PROCESS_ADDRESS_(mxModule, Py_Initialize, api.Initialize);
    _GET_PROCESS_ADDRESS_(mxModule, Py_IsInitialized, api.IsInitialized);
    _GET_PROCESS_ADDRESS_(mxModule, PyErr_Print, api.Err_Print);
    _GET_PROCESS_ADDRESS_(mxModule, Py_Finalize, api.Finalize);
    _GET_PROCESS_ADDRESS_(mxModule, _Py_fopen, api.fopen);
    _GET_PROCESS_ADDRESS_(mxModule, PyRun_SimpleFileExFlags, api.Run_SimpleFileExFlags);
    _GET_PROCESS_ADDRESS_(mxModule, PyRun_SimpleStringFlags, api.Run_SimpleStringFlags);
    _GET_PROCESS_ADDRESS_(mxModule, PyImport_ImportModule, api.Import_ImportModule);
    _GET_PROCESS_ADDRESS_(mxModule, PyObject_GetAttrString, api.Object_GetAttrString);
    _GET_PROCESS_ADDRESS_(mxModule, PyErr_Clear, api.Err_Clear);
    _GET_PROCESS_ADDRESS_(mxModule, PyArg_Parse, api.Arg_Parse);
    _GET_PROCESS_ADDRESS_(mxModule, PyObject_CallFunction, api.Object_CallFunction);
    _GET_PROCESS_ADDRESS_(mxModule, PyObject_CallObject, api.Object_CallObject);

    _GET_PROCESS_ADDRESS_(mxModule, PyGILState_Ensure, api.GILState_Ensure);

    _GET_PROCESS_ADDRESS_(mxModule, PyEval_InitThreads, api.Eval_InitThreads);
    _GET_PROCESS_ADDRESS_(mxModule, PyEval_SaveThread, api.Eval_SaveThread);
    _GET_PROCESS_ADDRESS_(mxModule, PyEval_RestoreThread, api.Eval_RestoreThread);
    _GET_PROCESS_ADDRESS_(mxModule, PyGILState_Release, api.GILState_Release);

    return true;
}

Python3Api::GILUtils::GILUtils(Api& api) : m_api(api)
{
    m_gilState = m_api.GILState_Ensure(); //申请获取GIL

    m_threadState = m_api.Eval_SaveThread();
    m_api.Eval_RestoreThread(m_threadState);
}

Python3Api::GILUtils::~GILUtils()
{
    if (m_gilState)
    {
        m_threadState = m_api.Eval_SaveThread();
        if (m_threadState)
        {
            m_api.Eval_RestoreThread(m_threadState);
            m_api.GILState_Release(m_gilState);
        }
    }
}
