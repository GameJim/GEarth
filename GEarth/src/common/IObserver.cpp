#include "Common/IObserver.h"
#include "Common/Subject.h"


IObserver::~IObserver()
{
	//ÒÆ³ý
	for (auto& pSubject :m_pSubjects)
	{
		pSubject->Detach(this);
	}

	m_pSubjects.clear();
}

void IObserver::AddSubject(CSubject* pSubject)
{
	m_pSubjects.insert(pSubject);
}

void IObserver::RemoveSubject(CSubject* pSubject)
{
	auto itr = m_pSubjects.find(pSubject);
	if (itr != m_pSubjects.end())
	{
		m_pSubjects.erase(itr);
	}
}
