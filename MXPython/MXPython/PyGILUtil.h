#pragma once
#include "python.h"

namespace mxpy
{
    class PyGILUtil
    {
    public:
        PyGILUtil();
        ~PyGILUtil();

    private:
        PyGILState_STATE    m_state;
        PyThreadState*      m_save;
    };

}