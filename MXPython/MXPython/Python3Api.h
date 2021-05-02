#pragma once

#include <memory>
#include "Python3.7.1/include/python.h"


#include "PythonApi.h"

class Python3Api : public PythonApi
{
public:
    typedef std::shared_ptr<Python3Api> Ptr;

    struct Api
    {
        //PyAPI_FUNC(const char*) Py_GetVersion(void);
        _MX_DECLARE_CALL_FUNCTION_(const char*, _cdecl, Py_GetVersion)(void);
        Py_GetVersion GetVersion;

        //27
        //PyAPI_FUNC(void) Py_SetPythonHome(char*);
        //_MX_DECLARE_CALL_FUNCTION_(void, _cdecl, Py_SetPythonHome_27)(const char*);
        //static Py_SetPythonHome_27 SetPythonHome_27;

        //3
        //PyAPI_FUNC(void) Py_SetPythonHome(const wchar_t*);
        _MX_DECLARE_CALL_FUNCTION_(void, _cdecl, Py_SetPythonHome)(const wchar_t*);
        Py_SetPythonHome SetPythonHome;

        //PyAPI_FUNC(void) Py_Initialize(void);
        _MX_DECLARE_CALL_FUNCTION_(void, _cdecl, Py_Initialize)(void);
        Py_Initialize Initialize;

        //PyAPI_FUNC(int) Py_IsInitialized(void);
        _MX_DECLARE_CALL_FUNCTION_(int, _cdecl, Py_IsInitialized)(void);
        Py_IsInitialized IsInitialized;

        //PyAPI_FUNC(void) PyErr_Print(void);
        _MX_DECLARE_CALL_FUNCTION_(void, _cdecl, PyErr_Print)(void);
        PyErr_Print Err_Print;

        //PyAPI_FUNC(void) Py_Finalize(void);
        _MX_DECLARE_CALL_FUNCTION_(void, _cdecl, Py_Finalize)(void);
        Py_Finalize Finalize;

        //PyAPI_FUNC(FILE*) _Py_fopen(const char* pathname, const char* mode);
        _MX_DECLARE_CALL_FUNCTION_(FILE*, _cdecl, _Py_fopen)(const char* pathname, const char* mode);
        _Py_fopen fopen;

        //PyAPI_FUNC(int) PyRun_SimpleFileExFlags(FILE *, const char *, int, PyCompilerFlags *);
        _MX_DECLARE_CALL_FUNCTION_(int, _cdecl, PyRun_SimpleFileExFlags)(FILE*, const char*, int, PyCompilerFlags*);
        PyRun_SimpleFileExFlags Run_SimpleFileExFlags;

        //PyAPI_FUNC(int) PyRun_SimpleStringFlags(const char*, PyCompilerFlags*);
        _MX_DECLARE_CALL_FUNCTION_(int, _cdecl, PyRun_SimpleStringFlags)(const char*, PyCompilerFlags*);
        PyRun_SimpleStringFlags Run_SimpleStringFlags;

        //PyAPI_FUNC(PyObject*) PyImport_ImportModule(const char* name);
        _MX_DECLARE_CALL_FUNCTION_(PyObject*, _cdecl, PyImport_ImportModule)(const char* name);
        PyImport_ImportModule Import_ImportModule;

        //PyAPI_FUNC(PyObject*) PyObject_GetAttrString(PyObject*, const char*);
        _MX_DECLARE_CALL_FUNCTION_(PyObject*, _cdecl, PyObject_GetAttrString)(PyObject*, const char*);
        PyObject_GetAttrString Object_GetAttrString;

        //PyAPI_FUNC(void) PyErr_Clear(void);
        _MX_DECLARE_CALL_FUNCTION_(void, _cdecl, PyErr_Clear)(void);
        PyErr_Clear Err_Clear;

        //PyAPI_FUNC(int) PyArg_Parse(PyObject*, const char*, ...);
        _MX_DECLARE_CALL_FUNCTION_(int, _cdecl, PyArg_Parse)(PyObject*, const char*, ...);
        PyArg_Parse Arg_Parse;

        //PyAPI_FUNC(PyObject*) PyObject_CallFunction(PyObject* callable_object, char* format, ...);
        _MX_DECLARE_CALL_FUNCTION_(PyObject*, _cdecl, PyObject_CallFunction)(PyObject* callable_object, const char* format, ...);
        PyObject_CallFunction Object_CallFunction;

        //PyAPI_FUNC(PyObject*) PyObject_CallObject(PyObject* callable, PyObject* args);
        _MX_DECLARE_CALL_FUNCTION_(PyObject*, _cdecl, PyObject_CallObject)(PyObject* callable, PyObject* args);
        PyObject_CallObject Object_CallObject;

        //PyAPI_FUNC(void) PyEval_InitThreads(void);
        _MX_DECLARE_CALL_FUNCTION_(void, _cdecl, PyEval_InitThreads)();
        PyEval_InitThreads Eval_InitThreads;

        //PyAPI_FUNC(PyGILState_STATE) PyGILState_Ensure(void);
        _MX_DECLARE_CALL_FUNCTION_(PyGILState_STATE, _cdecl, PyGILState_Ensure)();
        PyGILState_Ensure GILState_Ensure;

        //PyAPI_FUNC(PyThreadState*) PyEval_SaveThread(void);
        _MX_DECLARE_CALL_FUNCTION_(PyThreadState*, _cdecl, PyEval_SaveThread)();
        PyEval_SaveThread Eval_SaveThread;

        //PyAPI_FUNC(void) PyEval_RestoreThread(PyThreadState*);
        _MX_DECLARE_CALL_FUNCTION_(void, _cdecl, PyEval_RestoreThread)(PyThreadState*);
        PyEval_RestoreThread Eval_RestoreThread;

        //PyAPI_FUNC(void) PyGILState_Release(PyGILState_STATE);
        _MX_DECLARE_CALL_FUNCTION_(void, _cdecl, PyGILState_Release)(PyGILState_STATE);
        PyGILState_Release GILState_Release;
    };

    class GILUtils
    {
    public:
        GILUtils(Api& api);
        ~GILUtils();

    private:
        Api& m_api;
        PyGILState_STATE m_gilState = PyGILState_UNLOCKED;
        PyThreadState* m_threadState = nullptr;
    };



public:
    ~Python3Api();

    bool Init(const PythonApi::Info& info);

    Api api;
};

