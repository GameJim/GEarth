#pragma once
/*!
*@File    Byte.h
*@Brief   字节操作类,不支持多线程
*@Date    2021/12/22
*/
#include <memory>
#include <string>
#include "Common/NonCopyable.h"
class CORE_PUBLIC CByte : public CNonCopyable
{
public:
	CByte(unsigned char* src, unsigned nSize = 0);
	CByte(unsigned nSize);
	CByte() = delete;
	virtual ~CByte();

	const unsigned char* Data();

	//跳转到指定位置
	void SeekgPos(const unsigned& nsize = 0);
	//跳过多少
	void Seekg(const unsigned& nsize);
	//数据转换
	CByte& operator >> (uint8_t& ui8);
	CByte& operator >> (bool& b);
	CByte& operator >> (char& c);
	CByte& operator >> (short& s);
	CByte& operator >> (int& i);
	CByte& operator >> (unsigned& ui);
    //CByte& operator >> (unsigned int& ui);
	CByte& operator >> (float& i);
	CByte& operator >> (double& ui);
	CByte& operator >> (std::string& str);


	
	CByte& operator << (const uint8_t& ui8);
	CByte& operator << (const bool& b);
	CByte& operator << (const char& c);
	CByte& operator << (const short& s);
	CByte& operator << (const int& i);
	CByte& operator << (const unsigned& ui);
    //CByte& operator << (const unsigned int& ui);
	CByte& operator << (const float& f);
	CByte& operator << (const double& d);
	CByte& operator << (const std::string& str);

    //处理数组
    void ReadCharArry(unsigned char* data, unsigned nSize);
    void WriteCharArry(unsigned char* data, unsigned nSize);
protected:
	unsigned char* itr;     //数据的浏览器

	unsigned char* m_pData;
	unsigned m_nSize;   //字节大小，如果为0则不代表有意义
	bool m_IsSwap;  //判断数据大小端
};