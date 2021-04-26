#include "stdafx.h"
#include "PyGILUtil.h"
#include "Python.h"
#include "pystate.h"

namespace mxpy
{
    PyGILUtil::PyGILUtil()
    {
        m_state = PyGILState_Ensure(); //申请获取GIL

        m_save = PyEval_SaveThread();
        PyEval_RestoreThread(m_save);
    }

    PyGILUtil::~PyGILUtil()
    {
        if (m_state)
        {
            m_save = PyEval_SaveThread();
            if (m_save)
            {
                PyEval_RestoreThread(m_save);
                PyGILState_Release(m_state);
            }
        }
    }
}