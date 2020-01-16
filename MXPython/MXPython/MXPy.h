#pragma once
#include <string>

namespace mxpy
{
	template<typename mx_py>
	class MXPy
	{
	public:
		static const char* GetVersion() { return m_version.c_str(); }
		static const char* GetHomePath() { return m_homePath.c_str(); }

	protected:
        static void InitMXPy(const char* ver, const char* home)
        {
            m_version = ver;
            m_homePath = home;
        }

	protected:
		static std::string m_homePath;
		static std::string m_version;
	};

	template<typename mx_py>
	std::string MXPy<mx_py>::m_homePath;

	template<typename mx_py>
	std::string MXPy<mx_py>::m_version;

}