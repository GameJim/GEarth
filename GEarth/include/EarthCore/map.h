/*!
*@File    type.h
*@Brief   文件描述:下次一定
*@Date    2022/1/10
*/
#pragma once
#include "common/apiDescription.h"
#include "osgEarth/Map"
#include "type.h"
namespace earth
{

    class CORE_PUBLIC CMapCallback :/* public CVecCallback<CLayer> :*/ public osgEarth::MapCallback
    {
    public:
       // using CMapModel = CVecModelChange<CLayer>;

        CMapCallback() = default;
        virtual ~CMapCallback() = default;


        //! Invoked when a layer is added to a Map.
        virtual void onLayerAdded(osgEarth::Layer* layer, unsigned index) { this->onChildAdded(layer, index); }

        //! Invoked when a layer is removed from a Map.
        virtual void onLayerRemoved(osgEarth::Layer* layer, unsigned index) { this->onChildRemoved(layer, index); }

        //! Invoked when a layer is re-ordered within a Map.
        virtual void onLayerMoved(osgEarth::Layer* layer, unsigned oldIndex, unsigned newIndex) { this->onChildMoved(layer, oldIndex, newIndex); }

    protected:
        //! Invoked when a layer is added to a Map.
        virtual void onChildAdded(earth::CLayer* pLayer, unsigned index) {
            
        }

        //! Invoked when a layer is removed from a Map.
        virtual void onChildRemoved(earth::CLayer* pLayer, unsigned index) {
         

        }

        //! Invoked when a layer is re-ordered within a Map.
        virtual void onChildMoved(earth::CLayer* pLayer, unsigned oldIndex, unsigned newIndex) {}
    };

   
   class CORE_PUBLIC CMap : public osgEarth::Map
   {
   public:
       CMap();
       CMap(int argc, char **);
       /*!
       /* @Brief:     添加函数描述
       /* @Date:      2022/1/21
       /* @Return     
       */
       virtual ~CMap();
   public:
       bool IsEnable();
       bool m_IsEnable;
   };

}


