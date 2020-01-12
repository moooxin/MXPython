#pragma once
#include <string>

namespace mxpython
{
	template<typename mx_py>
	class MXPy
	{
	public:
		static const char* GetVersion() { return m_version.c_str(); }

	protected:
		static std::string m_homePath;
		static std::string m_version;
	};

	template<typename mx_py>
	std::string MXPy<mx_py>::m_homePath;

	template<typename mx_py>
	std::string MXPy<mx_py>::m_version;

}