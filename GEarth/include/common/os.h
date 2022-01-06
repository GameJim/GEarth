#pragma once
/*!
*@File    OS.h
*@Brief   操作系统类
*@Date    2021/12/21
*/
#include "Common/Factory.h"
#include <string>
class CORE_PUBLIC COS : public CFactory<COS>
{
public:
	/*!
	/* @Brief:     获取当前程序占用内存
	/* @Date:      2021/12/27
	/* @Return     double
	*/
	virtual double GetMemory();

	/*!
	/* @Brief:     生成全局唯一id
	/* @Date:      2021/12/27
	/* @Return     unsigned int
	*/
	virtual unsigned int GenerateUID() const;

	/*!
	/* @Brief:     生产全局唯一id
	/* @Date:      2021/12/27
	/* @Return     std::string
	*/
	virtual std::string GenerateUSID() const; 

	virtual ~COS();
protected:
	COS();
};

