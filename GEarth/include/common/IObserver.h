#pragma once
#include "Common/NonCopyable.h"
#include <set>
class CSubject;
class CORE_PUBLIC IObserver : public CNonCopyable
{
	friend class CSubject;
protected:
	/*!
	/* @Brief:     �۲��߼��ӵ�����
	/* @Date:      2021/12/21
	/* @Parameter: const CSubject * pSubject   ������Դ
	/* @Parameter: unsigned & nEvent           ��Ϣ����
	/* @Parameter: void * data                  ��Ϣ����
	/* @Return     void
	*/
	virtual void Update(const CSubject* pSubject,unsigned& nEvent, void* data) = 0;
protected:
	IObserver() = default;
	virtual ~IObserver();
	virtual void AddSubject(CSubject* pSubject);
	virtual void RemoveSubject(CSubject* pSubject);

	std::set<CSubject*> m_pSubjects;        //���۲������
};