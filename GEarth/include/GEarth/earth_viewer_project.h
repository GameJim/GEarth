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
	/*һ��Map ֻ����һ��MapNode
	�����������MapNode������˫����Ⱦ�������ڴ棬�ڴ��˷�
	ͬʱһ��Layer���������ͶӰ��ͬMapNode��Map�У����ɵ���Ⱦ�����ǲ�һ����

	�����ͬͶӰ��Map���Թ���һ��Layer,���Ǳ�����ȫ��������Ĭ�ϵ�3άģʽ�µ�������
	 һ��MapNode�����ж������,��������һ��Map���ͬʱʵ�����ƽ����ʾ����jian��

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

	//���Զ����й���
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
		std::vector<std::shared_ptr<EarthProject>>  m_pEarthProject;    //Earth ����
		std::vector<std::shared_ptr<EarthCallback>> m_pEarthCallbacks;   //�ص�����
	};
}
