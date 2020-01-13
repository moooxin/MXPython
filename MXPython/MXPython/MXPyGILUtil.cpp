#include "stdafx.h"
#include "MXPyGILUtil.h"
#include "Python.h"
#include "pystate.h"

namespace mxpy
{
    MXPyGILUtil::MXPyGILUtil()
    {
        m_state = PyGILState_Ensure(); //申请获取GIL

        m_save = PyEval_SaveThread();
        PyEval_RestoreThread(m_save);
    }

    MXPyGILUtil::~MXPyGILUtil()
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