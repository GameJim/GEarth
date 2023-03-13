
#include <osgEarth/MapNode>

#include <util/file_system.h>

#include "GEarth/earth_viewer_project.h"
using namespace osgEarth;
//using namespace osgEarth::Util;
namespace ui
{

	bool EarthViewerProject::loadEarthFile(const std::string& sPath, const osgDB::Options* options)
	{
		ReadResult r = osgEarth::URI(sPath).readNode(options);
		if (r.code() == ReadResult::RESULT_OK)
		{
			MapNode* pMapNode = r.release<MapNode>();
			osg::ref_ptr<osgEarth::Map > pMap = pMapNode->getMap();

			std::shared_ptr<EarthProject> ptrEarthProject = std::make_shared<EarthProject>(sPath, pMap, pMapNode);
			EarthProject* pEarthProject = ptrEarthProject.get();
			m_pEarthProject.push_back(std::move(ptrEarthProject));

			for (auto callback: m_pEarthCallbacks)
			{
				callback->onEarthAdded(pEarthProject, m_pEarthProject.size()-1);
			}
			return true;
		}
		

		return false;
	}

	
	bool EarthViewerProject::createEarth(const std::string& sPath, const osgDB::Options* options /*= nullptr*/, const bool& shared /*= false*/)
	{
		osg::ref_ptr<osgEarth::Map > pMap = new osgEarth::Map(options);
		osg::ref_ptr<osgEarth::MapNode > pMapNode = new MapNode(pMap.get());

		std::shared_ptr<EarthProject> ptrEarthProject = std::make_shared<EarthProject>(sPath, pMap, pMapNode);
		EarthProject* pEarthProject = ptrEarthProject.get();

		m_pEarthProject.push_back(std::move(ptrEarthProject));

		for (auto callback : m_pEarthCallbacks)
		{
			callback->onEarthAdded(pEarthProject, m_pEarthProject.size() - 1);
		}

		return true;
	}

	void EarthViewerProject::removeEarth(const uint32_t& id)
	{

	}

	void EarthViewerProject::saveProject(const std::string& sName)
	{
		
	}


	bool EarthViewerProject::addEarthCallback(std::shared_ptr<EarthCallback> pEarthCallback)
	{
		return true;
	}

	bool EarthViewerProject::removeEarthCallback(std::shared_ptr<EarthCallback> pEarthCallback)
	{
		return true;
	}

	EarthProject::EarthProject(std::string path, osg::ref_ptr<osgEarth::Map> mp, osg::ref_ptr<osgEarth::MapNode> mapNode, const bool& shared /*= false*/)
		:_path(path),_map(mp), _shared(shared),_mapNode(mapNode)
	{
		_uid = osgEarth::createUID();
		_name = util::FileSystem::instance()->fileStem(path);

	}

}


