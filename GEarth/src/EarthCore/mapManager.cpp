#include "EarthCore/mapManager.h"
#include <assert.h>
namespace earth
{
    CMapManager::~CMapManager()
    {

    }

   CRefPtr<earth::CMap> CMapManager::CreateMap()
    {
        earth::CRefPtr<earth::CMap> pMap = new earth::CMap();
        m_pMaps.push_back(pMap);

        for (auto& callback : m_pCallback)
        {
            callback->onModelChanged(CMapManagerCallback::CMapManagerModel(CMapManagerCallback::CMapManagerModel::ADD_CHILD, pMap, -1, m_pMaps.size()));
        }
       
        return pMap;
    }

    void CMapManager::AddCallBack(earth::CRefPtr<CMapManagerCallback> callBack)
    {
        assert(callBack != nullptr);
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

        for (auto& callback: m_pCallback)
        {
            callback->onModelChanged(CMapManagerCallback::CMapManagerModel(CMapManagerCallback::CMapManagerModel::ADD_CHILD, pMap, -1, m_pMaps.size()));
        }
    }

    void CMapManager::RemoveMap(earth::CRefPtr<CMap> pMap)
    {
        int index = findIndex(pMap);
        RemoveMap(index);
    }

    void CMapManager::RemoveMap(const unsigned& index)
    {
        if (m_pMaps.size() <= index)
            return;

        auto pMap = m_pMaps[index];
        m_pMaps.erase(m_pMaps.begin() + index);

        for (auto& callback : m_pCallback)
        {
            callback->onModelChanged(CMapManagerCallback::CMapManagerModel(CMapManagerCallback::CMapManagerModel::REMOVE_CHILD, pMap, index, -1));
        }
     
    }

    int CMapManager::findIndex(earth::CRefPtr<CMap> pMap)
    {
        for (int i = 0;i < m_pMaps.size();++i)
        {
            if (m_pMaps[i] == pMap)
            {
                return i;
            }
        }
        return -1;
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


}

