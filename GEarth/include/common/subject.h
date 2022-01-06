#pragma once
#include "common/noncopyable.h"
#include <set>

namespace COMMON_NAMESPACE
{
    class IObserver;
    class CORE_PUBLIC CSubject
    {
    public:
        /*!
        /* @Brief:     添加观察者
        /* @Date:      2021/12/21
        /* @FullName:  CSubject::Attach
        /* @Parameter: IObserver * pObserver
        /* @Return     void
        */
        virtual void Attach(IObserver* pObserver);
        /*!
        /* @Brief:     移除观察者
        /* @Date:      2021/12/21
        /* @FullName:  CSubject::Detach
        /* @Parameter: IObserver * pObserver
        /* @Return     void
        */
        virtual void Detach(IObserver* pObserver);
        /*!
        /* @Brief:     通知观察者发生变更
        /* @Date:      2021/12/21
        /* @FullName:  CSubject::Notify
        /* @Parameter: unsigned & nEvent
        /* @Parameter: void * data
        /* @Return     void
        */
        virtual void Notify(unsigned& nEvent, void* data);


        /*!
        /* @Brief:     需要拷贝观察者
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
}


