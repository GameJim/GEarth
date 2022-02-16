/*!
*@File    mapManager.h
*@Brief   用于全局管理地图数据，单例模式
*@Date    2022/2/16
*/
#pragma once
#include "common/factory.h"
#include "EarthCore/map.h"
#include "EarthCore/type.h"
namespace earth
{
    //用于记录Map的回调信息
    class CORE_PUBLIC CMapManagerCallback : public osg::Referenced
    {
    public:
        enum EnCallbackType
        {
            MAP_ADDED = 0,
            MAP_REMOVE
        };

        virtual void Callback(const CMapManagerCallback::EnCallbackType& eType, earth::CRefPtr<CMap>, const unsigned& index) {};

        CMapManagerCallback() = default;
        virtual ~CMapManagerCallback() = default;
       
        /*   CMapManagerCallback(const EnCallbackType& eCallback) :m_eCallback(eCallback){};
           EnCallbackType GetCallbackType(){ return m_eCallback; };
       protected:
           EnCallbackType m_eCallback;*/
    };


    class CORE_PUBLIC CMapManager : public common::CFactory<CMapManager>
    {
    public:
        CMapManager() = default;
       
        ~CMapManager() = default;
        earth::CRefPtr<CMap>  CreateMap();

        void AddCallBack(earth::CRefPtr<CMapManagerCallback> callBack);
        void RemoveCallBack(earth::CRefPtr<CMapManagerCallback> callBack);

        void AddMap(earth::CRefPtr<CMap> pMap);
        void RemoveMap(earth::CRefPtr<CMap> pMap);
        void RemoveMap(const unsigned& index);

        unsigned GetMapNum() const;
        earth::CRefPtr<CMap> GetMap(const unsigned& index);
    protected:
        void NotifyCallback(const CMapManagerCallback::EnCallbackType& eType, earth::CRefPtr<CMap>, const unsigned& index);
    protected:
        std::vector<earth::CRefPtr<CMap>> m_pMaps;   //地图数据

        std::vector<earth::CRefPtr<CMapManagerCallback>>  m_pCallback;
    };
}

