#pragma once
#include "Common/NonCopyable.h"
#include <set>

namespace COMMON_NAMESPACE
{
    class CSubject;
    class CORE_PUBLIC IObserver
    {
        friend class CSubject;
    protected:
        /*!
        /* @Brief:     观察者监视到更新
        /* @Date:      2021/12/21
        /* @Parameter: const CSubject * pSubject   数据来源
        /* @Parameter: unsigned & nEvent           信息类型
        /* @Parameter: void * data                  信息数据
        /* @Return     void
        */
        virtual void Update(const CSubject* pSubject, unsigned& nEvent, void* data) = 0;
    protected:
        IObserver() = default;
        virtual ~IObserver();
        virtual void AddSubject(CSubject* pSubject);
        virtual void RemoveSubject(CSubject* pSubject);

        std::set<CSubject*> m_pSubjects;        //被观察的数据
    };
}
