#pragma once

#if _PY_VER_==37
#include "Python.h"
#include "MXPy.h"

namespace mxpython
{
	class MXPython37 : public MXPy<MXPython37>
	{
	public:
		MXPython37();
		~MXPython37();

		bool Init(const CHAR* dir);

		bool RunFile(const CHAR* file);

	protected:
	};

}
#endif