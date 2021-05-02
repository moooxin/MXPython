#pragma once

#include <memory>

#include "base/singleton_object.h"
#include "base/string_utils.h"

#include "IPython.h"

#define _MX_DECLARE_CALL_FUNCTION_(ret, call, name) typedef ret(call* name)

#define _MX_GET_PROC_ADDRESS(md, type, name) name = (type)GetProcAddress(md, ##type)
#define _MX_GET_PROC_ADDRESS_2(md, ns, type, name) name = (ns::type)GetProcAddress(md, #type)
#define _MX_GET_PROC_ADDRESS_3(md, type, name, func) name = (type)GetProcAddress(md, func)

#define _GET_PROCESS_ADDRESS_(hm,type,value) PythonApi::GetProcessAddress(hm,value,#type)


class PythonApi 
{
public:

    template<typename T>
    static inline void GetProcessAddress(HMODULE& hm, T& value, const char* name)
    {
        if (!name)
            return;

        value = (T)::GetProcAddress(hm, name);
        if (!value)
            printf("can't find function address:%s.\n", name);
    }


public:
    typedef std::shared_ptr<PythonApi> Ptr;

    struct Info
    {
        HMODULE             dllModule;
        std::string         fullVersion;
        mx::PythonVersion   version;
    };

    static bool Load(const char* dll, Info* info)
    {
        if (!info)
            return false;

        info->dllModule = 0;
        info->fullVersion.clear();
        info->version.mainVersion = 0;
        info->version.subVersion = 0;
        info->version.tagNumber = 0;
        info->version.buildNumber = 0;

        HMODULE pyModule = ::LoadLibraryA((LPCSTR)dll);
        if (!pyModule)
        {
            DWORD le = GetLastError();
            return false;
        }

        _MX_DECLARE_CALL_FUNCTION_(const char*, _cdecl, Py_GetVersion)(void);
        Py_GetVersion getVersion;
        _GET_PROCESS_ADDRESS_(pyModule, Py_GetVersion, getVersion);
        if (!getVersion)
        {
            ::FreeLibrary(pyModule);
            return false;
        }

        info->dllModule = pyModule;
        const char* ver = getVersion();
        info->fullVersion = ver ? ver : "";
        std::vector<std::string> vers;
        if(mxkit::StringUtils::Split<std::string>(info->fullVersion, ".", &vers) < 3)
        {
            ::FreeLibrary(pyModule);
            return false;
        }

        info->version.mainVersion = atoi(vers[0].c_str());
        info->version.subVersion = atoi(vers[1].c_str());
        info->version.tagNumber = atoi(vers[2].c_str());

        std::string str = mxkit::StringUtils::ToString<int, std::string>(info->version.mainVersion);
        str += ".";
        str += mxkit::StringUtils::ToString<int, std::string>(info->version.subVersion);
        str += ".";
        str += mxkit::StringUtils::ToString<int, std::string>(info->version.tagNumber);

        strcpy_s(info->version.version, sizeof(info->version.version), str.c_str());
        
        return true;
    }

    static void Free(Info* info)
    {
        if (!info)
            return ;

        if (info->dllModule)
            ::FreeLibrary(info->dllModule);

        info->dllModule = 0;
        info->fullVersion.clear();
        info->version.mainVersion = 0;
        info->version.subVersion = 0;
        info->version.tagNumber = 0;
        info->version.buildNumber = 0;
    }

public:
    virtual ~PythonApi() {}

    const Info& info() const { return m_info; }

protected:
    Info m_info;
};
