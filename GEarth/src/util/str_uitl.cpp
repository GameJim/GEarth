#include "util/str_uitl.h"

#include <windows.h>
namespace util
{
	std::string util::StrUtil::UTF8ToAnsi(const std::string& str)
	{
		int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
		wchar_t* pwBuf = new wchar_t[nwLen + 1];    //一定要加1，不然会出现尾巴 
		memset(pwBuf, 0, nwLen * 2 + 2);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);
		int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
		char* pBuf = new char[nLen + 1];
		memset(pBuf, 0, nLen + 1);
		WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

		std::string strRet = pBuf;

		delete[]pBuf;
		delete[]pwBuf;
		pBuf = NULL;
		pwBuf = NULL;

		return strRet;
	}

	std::wstring StrUtil::UTF8ToUnicode(const std::string& str)
	{
		int len = 0;
		len = str.length();
		int unicodeLen = ::MultiByteToWideChar(CP_UTF8,0,str.c_str(),- 1,NULL,0);
		wchar_t * pUnicode;
		pUnicode = new wchar_t[unicodeLen + 1];
		memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
		::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, (LPWSTR)pUnicode,unicodeLen);
		std::wstring rt;
		rt = (wchar_t*)pUnicode;
		delete pUnicode;
		return rt;
	}

	std::string StrUtil::AnsiToUTF8(const std::string& str)
	{
		int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		wchar_t* pwBuf = new wchar_t[nwLen + 1];    //一定要加1，不然会出现尾巴 
		ZeroMemory(pwBuf, nwLen * 2 + 2);
		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
		char* pBuf = new char[nLen + 1];
		ZeroMemory(pBuf, nLen + 1);
		::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

		std::string strRet(pBuf);

		delete[]pwBuf;
		delete[]pBuf;
		pwBuf = NULL;
		pBuf = NULL;

		return strRet;
	}

	std::wstring StrUtil::AnsiToUnicode(const std::string& str)
	{
		int len = 0;
		len = str.length();
		int unicodeLen = ::MultiByteToWideChar(CP_ACP,0,str.c_str(),-1,NULL,0);
		 wchar_t * pUnicode;
		 pUnicode = new wchar_t[unicodeLen + 1];
		 memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
		 ::MultiByteToWideChar(CP_ACP,0,str.c_str(),- 1,(LPWSTR)pUnicode, unicodeLen);
		 std::wstring rt;
		 rt = (wchar_t*)pUnicode;
		 delete pUnicode;
		 return rt;
	}


	std::string StrUtil::Unicode2UTF8(const std::wstring& ws)
	{
		int len;
		int slen = (int)ws.length() + 1;
		len = WideCharToMultiByte(CP_ACP, 0, ws.c_str(), slen, NULL, 0, NULL, NULL);
		char* buf = new char[len];
		WideCharToMultiByte(CP_ACP, 0, ws.c_str(), slen, buf, len, NULL, NULL);
		std::string r(buf);
		delete[] buf;
		return r;
	}

}

	


