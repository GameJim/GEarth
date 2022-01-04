/*!
*@File    obejct_manager.h
*@Brief   资源管理器,用于管理共享对象
*@Date    2021/12/30
*/
#pragma once
#include "Common/Factory.h"
#include "Common/Mutexable.h"
#include <memory>
#include <unordered_set>
//template<class T> 
//class CORE_PUBLIC CObjectManager : public CMutexable
//{
//public:
//    /*!
//    /* @Brief:     创建新的共享对象
//    /* @Date:      2021/12/31
//    /* @Return     std::shared_ptr<T>
//    */
//    std::shared_ptr<T> CreateShareObject();
//
//    /*!
//    /* @Brief:     添加共享对象
//    /* @Date:      2021/12/31
//    /* @Parameter: shared_ptr<T> pObkect
//    /* @Return     bool
//    */
//    bool AddShareObject(shared_ptr<T> pObkect);
//   
//    CObjectManager() = default;
//    virtual ~CObjectManager() = default;
//protected:
//    std::unordered_set<shared_ptr<T>>   m_pShaderObjects;
//};
//
//template<class T>
//bool CObjectManager<T>::AddShareObject(shared_ptr<T> pObject)
//{
//    std::unique_lock<std::mutex> lock(m_Mutex);
//    if (m_pShaderObjects.find(pObject) != m_pShaderObjects.end())
//    {
//        LOG_INFO("share object is exist")
//        return false;
//    }
//    m_pShaderObjects.insert(pObject);
//
//    return true;
//}
//
//template<class T>
//std::shared_ptr<T> CObjectManager<T>::CreateShareObject()
//{
//    std::unique_lock<std::mutex> lock(m_Mutex);
//    shared_ptr<T> pObject = make_shared<T>();
//    m_pShaderObjects.insert(pObject);
//
//    return pObject;
//}

