#include "Common/Subject.h"
#include "Common/IObserver.h"


CSubject::~CSubject()
{
	for (std::set<IObserver*>::iterator itr = m_pObserver.begin(); itr != m_pObserver.end(); itr++)
	{
		//�Ƴ���
		(*itr)->RemoveSubject(this);
	}
	m_pObserver.clear();
}
void CSubject::Attach(IObserver* pObserver)
{
	if (!pObserver)
		return;

	for (std::set<IObserver*>::iterator itr = m_pObserver.begin(); itr != m_pObserver.end(); itr++)
	{
		(*itr)->AddSubject(this);
	}
	
	m_pObserver.insert(pObserver);
}

void CSubject::Detach(IObserver* pObserver)
{
	std::set<IObserver*>::iterator itr = m_pObserver.find(pObserver);
	if (itr != m_pObserver.end())
	{
		//�Ƴ���
		(*itr)->RemoveSubject(this);
		//�Ƴ��۲���
		m_pObserver.erase(itr);
	}
}

void CSubject::Notify(unsigned& nEvent, void* data)
{
	for (std::set<IObserver*>::iterator itr = m_pObserver.begin();itr !=  m_pObserver.end(); itr++)
	{
		(*itr)->Update(this, nEvent, data);
	}
		
}

CSubject& CSubject::operator=(const CSubject& other)
{
	m_pObserver = other.m_pObserver;
	for (std::set<IObserver*>::iterator itr = m_pObserver.begin(); itr != m_pObserver.end(); itr++)
	{
		//���۲�������µİ�
		(*itr)->AddSubject(this);
	}

	return *this;
}
