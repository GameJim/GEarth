#pragma once
#include "Common/NonCopyable.h"
#include <set>

class IObserver;
class CORE_PUBLIC CSubject 
{
public:
	/*!
	/* @Brief:     ��ӹ۲���
	/* @Date:      2021/12/21
	/* @FullName:  CSubject::Attach
	/* @Parameter: IObserver * pObserver
	/* @Return     void
	*/
	virtual void Attach(IObserver* pObserver);
	/*!
	/* @Brief:     �Ƴ��۲���
	/* @Date:      2021/12/21
	/* @FullName:  CSubject::Detach
	/* @Parameter: IObserver * pObserver
	/* @Return     void
	*/
	virtual void Detach(IObserver* pObserver);
	/*!
	/* @Brief:     ֪ͨ�۲��߷������
	/* @Date:      2021/12/21
	/* @FullName:  CSubject::Notify
	/* @Parameter: unsigned & nEvent
	/* @Parameter: void * data
	/* @Return     void
	*/
	virtual void Notify(unsigned& nEvent,void* data);

	
	/*!
	/* @Brief:     ��Ҫ�����۲���
	/* @Date:      2021/12/22
	/* @Parameter: const CSubject & other
	/* @Return     CSubject&
	*/
	virtual CSubject& operator=(const CSubject& other);
protected:
	CSubject() = default;
	virtual ~CSubject();
	std::set<IObserver*> m_pObserver;
};

