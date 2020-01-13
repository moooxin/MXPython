#pragma once
#include "python.h"

namespace mxpy
{
    class MXPyGILUtil
    {
    public:
        MXPyGILUtil();
        ~MXPyGILUtil();

    private:
        PyGILState_STATE    m_state;
        PyThreadState*      m_save;
    };

}