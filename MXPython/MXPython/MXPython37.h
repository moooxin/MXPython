#pragma once

#if _PY_VER_==37
#include "MXDllExportDefine.h"
#include "MXSingleObject.h"
#include "IMXPython.h"
#include "Python.h"
#include "MXPy.h"

namespace mxpy
{
    class MXPython37
        : public IMXPython37
        , public MXPy<MXPython37>
        , public mxtoolkit::MXInterfaceImp<MXPython37>
        , public mxtoolkit::MXSingleObject<MXPython37>
	{
	public:
        MXPython37();
		~MXPython37();

        virtual mxtoolkit::Result Initialize(const CHAR* dir) override;

        virtual mxtoolkit::Result Uninstall() override;

        virtual mxtoolkit::Result ExcuteFile(const CHAR* file) override;

        virtual mxtoolkit::Result ExcuteMethod(const CHAR* file, const CHAR* method, const CHAR* param, char** result) override;

	protected:
	};

}
#endif