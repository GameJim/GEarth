#include "Common/Subject.h"
#include "Common/IObserver.h"

namespace COMMON_NAMESPACE
{
    CSubject::~CSubject()
    {
        for (std::set<IObserver*>::iterator itr = m_pObserver.begin(); itr != m_pObserver.end(); itr++)
        {
            //移除绑定
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
            //移除绑定
            (*itr)->RemoveSubject(this);
            //移除观察者
            m_pObserver.erase(itr);
        }
    }

    void CSubject::Notify(unsigned& nEvent, void* data)
    {
        for (std::set<IObserver*>::iterator itr = m_pObserver.begin(); itr != m_pObserver.end(); itr++)
        {
            (*itr)->Update(this, nEvent, data);
        }

    }

    CSubject& CSubject::operator=(const CSubject& other)
    {
        m_pObserver = other.m_pObserver;
        for (std::set<IObserver*>::iterator itr = m_pObserver.begin(); itr != m_pObserver.end(); itr++)
        {
            //给观察者添加新的绑定
            (*itr)->AddSubject(this);
        }

        return *this;
    }

}

