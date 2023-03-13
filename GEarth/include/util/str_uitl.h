#pragma once
#include <string>
#include "util/util_export.h"

namespace util
{
	class UTIL_API  StrUtil
	{

	public:
		static std::string UTF8ToAnsi(const std::string& str);

		static std::wstring UTF8ToUnicode(const std::string& str);
		
		// std:stringתUTF8
		static std::string AnsiToUTF8(const std::string& str);
		static std::wstring AnsiToUnicode(const std::string& str);


		static std::string Unicode2UTF8(const std::wstring& ws);
		
	};
}