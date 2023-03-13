#pragma once
#include <util/define_helper.h>
#include <GEarth/api_description.h>
#include <string>
#include <osg/ref_ptr>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include <osgEarth/MapNode>
#include <osgEarth/Map>
namespace ui
{
	/*一个Map 只能有一个MapNode
	如果存在两个MapNode则会造成双份渲染数据在内存，内存浪费
	同时一个Layer如果在两个投影不同MapNode的Map中，生成的渲染数据是不一样的

	因此相同投影的Map可以共享一个Layer,但是必须完全共享。比如默认的3维模式下地形数据
	 一个MapNode可以有多个窗口,存在问题一张Map如何同时实现球和平面显示（插jian）

	*/
	class EARTH_API EarthProject
	{
		DEFINE_PROPERTY(std::string, name)
		DEFINE_PROPERTY(std::string, path)
		DEFINE_PROPERTY(uint32_t, uid)
		DEFINE_PROPERTY(bool, shared)
		DEFINE_PROPERTY(osg::ref_ptr<osgEarth::Map>, map)
		DEFINE_PROPERTY(osg::ref_ptr<osgEarth::MapNode>, mapNode)
	public:
		EarthProject(std::string path, osg::ref_ptr<osgEarth::Map> mp, osg::ref_ptr<osgEarth::MapNode> mapNode, const  bool& shared = false);
		virtual ~EarthProject() = default;
	};


	struct EARTH_API EarthCallback
	{
		virtual void onEarthAdded(EarthProject* earth,unsigned index) { }
		virtual void onEarthRemoved(EarthProject* earth,unsigned index) { }
		virtual void onEarthMoved(EarthProject* Earth, unsigned oldIndex, unsigned newIndex) { }
	};

	//有自定义有工程
	class EARTH_API EarthViewerProject
	{
		DEFINE_OPTION(std::string, path)
	public:
		EarthViewerProject() = default;
		virtual ~EarthViewerProject() = default;

		bool loadEarthFile(const std::string& sPath, const osgDB::Options* options = nullptr);
		bool createEarth(const std::string& sPath, const osgDB::Options* options = nullptr, const bool& shared = false);
		void removeEarth(const uint32_t& uid);

		void saveProject(const std::string& sName);

		bool addEarthCallback(std::shared_ptr<EarthCallback> pEarthCallback);
		bool removeEarthCallback(std::shared_ptr<EarthCallback> pEarthCallback);
	private:
		std::vector<std::shared_ptr<EarthProject>>  m_pEarthProject;    //Earth 工程
		std::vector<std::shared_ptr<EarthCallback>> m_pEarthCallbacks;   //回调函数
	};
}
