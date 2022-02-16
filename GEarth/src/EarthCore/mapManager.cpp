#include "EarthCore/mapManager.h"
namespace earth
{

    earth::CRefPtr<earth::CMap> CMapManager::CreateMap()
    {
        earth::CRefPtr<earth::CMap> pMap = new earth::CMap();
        m_pMaps.push_back(pMap);
      
        NotifyCallback(CMapManagerCallback::EnCallbackType::MAP_ADDED, pMap, m_pMaps.size() - 1);
        return pMap;
    }

    void CMapManager::AddCallBack(earth::CRefPtr<CMapManagerCallback> callBack)
    {
        m_pCallback.push_back(callBack);
    }

    void CMapManager::RemoveCallBack(earth::CRefPtr<CMapManagerCallback> callBack)
    {
        unsigned i = 0;
        for (auto entry: m_pCallback)
        {
            if (entry == callBack)
            {
                m_pCallback.erase(m_pCallback.begin()+i);
            }
            else
            {
                i++;
            }
            
        }
    }

    void CMapManager::AddMap(earth::CRefPtr<CMap> pMap)
    {
       m_pMaps.push_back(pMap);

        NotifyCallback(CMapManagerCallback::EnCallbackType::MAP_ADDED, pMap, m_pMaps.size() - 1);
    }

    void CMapManager::RemoveMap(earth::CRefPtr<CMap> pMap)
    {
        int index = 0;
        for (auto map : m_pMaps)
        {
            if (pMap == map)
            {
                NotifyCallback(CMapManagerCallback::EnCallbackType::MAP_REMOVE, pMap, index);
            }
            else
            {
                index++;
            }
           
        }

       
    }

    void CMapManager::RemoveMap(const unsigned& index)
    {
        if (m_pMaps.size() <= index)
            return;

        auto pMap = m_pMaps[index];
        m_pMaps.erase(m_pMaps.begin() + index);

        NotifyCallback(CMapManagerCallback::EnCallbackType::MAP_REMOVE, pMap, index);
     
    }

    unsigned CMapManager::GetMapNum() const
    {
        return m_pMaps.size();
    }

    earth::CRefPtr<earth::CMap> CMapManager::GetMap(const unsigned& index)
    {
        if (m_pMaps.size() <= index)
            return nullptr;

        return m_pMaps[index];
    }

    void CMapManager::NotifyCallback(const CMapManagerCallback::EnCallbackType& eType, earth::CRefPtr<CMap> pMap, const unsigned& index)
    {
        for (auto callback : m_pCallback)
        {
            if (callback)
                callback->Callback(eType, pMap, index);
            
        }

    }

}

