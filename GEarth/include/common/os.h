#pragma once
/*!
*@File    OS.h
*@Brief   ����ϵͳ��
*@Date    2021/12/21
*/
#include "Common/Factory.h"
#include <string>
class CORE_PUBLIC COS : public CFactory<COS>
{
public:
	/*!
	/* @Brief:     ��ȡ��ǰ����ռ���ڴ�
	/* @Date:      2021/12/27
	/* @Return     double
	*/
	virtual double GetMemory();

	/*!
	/* @Brief:     ����ȫ��Ψһid
	/* @Date:      2021/12/27
	/* @Return     unsigned int
	*/
	virtual unsigned int GenerateUID() const;

	/*!
	/* @Brief:     ����ȫ��Ψһid
	/* @Date:      2021/12/27
	/* @Return     std::string
	*/
	virtual std::string GenerateUSID() const; 

	virtual ~COS();
protected:
	COS();
};

