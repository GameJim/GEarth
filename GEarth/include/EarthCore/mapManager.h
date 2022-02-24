/*!
*@File    mapManager.h
*@Brief   用于全局管理地图数据，单例模式
*@Date    2022/2/16
*/
#pragma once
#include "common/factory.h"
#include "EarthCore/map.h"
#include "EarthCore/type.h"
#include "EarthCore/template.h"
namespace earth
{
    
 

    class CORE_PUBLIC CMapManagerCallback : public CVecCallback<CMap>
    {
    public:
        using CMapManagerModel = CVecModelChange<CMap>;

        CMapManagerCallback() = default;
        virtual ~CMapManagerCallback() = default;
    };
    

    class CORE_PUBLIC CMapManager : public common::CFactory<CMapManager>
    {
    public:
        CMapManager() = default;
       
        ~CMapManager();
        earth::CRefPtr<CMap>  CreateMap();

        void AddCallBack(earth::CRefPtr<CMapManagerCallback> callBack);
        void RemoveCallBack(earth::CRefPtr<CMapManagerCallback> callBack);

        void AddMap(earth::CRefPtr<CMap> pMap);
        void RemoveMap(earth::CRefPtr<CMap> pMap);
        void RemoveMap(const unsigned& index);

        int findIndex(earth::CRefPtr<CMap> pMap);
        unsigned GetMapNum() const;
        earth::CRefPtr<CMap> GetMap(const unsigned& index);

    protected:
        std::vector<earth::CRefPtr<CMap>> m_pMaps;   //地图数据
        std::vector<earth::CRefPtr<CMapManagerCallback>>  m_pCallback;
    };
}

