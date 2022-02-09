/*!
*@File    type.h
*@Brief   文件描述:下次一定
*@Date    2022/1/10
*/
#pragma once
#include "osg/ref_ptr"
#include "osg/Referenced"
#include "osg/DisplaySettings"
#include "osgEarth/EarthManipulator"
#include "osgEarth/MapNode"
#include "osgEarth/ExampleResources"
#include "common/apiDescription.h"

#include "osgViewer/CompositeViewer"
#include "osgViewer/ViewerEventHandlers"
namespace earth
{
   //using 
    template <typename T> using CRefPtr = osg::ref_ptr<T>;
  
    using CReferenced = osg::Referenced;
    using CDisplaySettings = osg::DisplaySettings;


    using CMapNode = osgEarth::Util::MapNode;
    using CMapNodeHelper = osgEarth::Util::MapNodeHelper;

    using  CCompositeViewer = osgViewer::CompositeViewer;

    using CStatsHandler = osgViewer::StatsHandler;
}


