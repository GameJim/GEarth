#include "GEarth/earth_cfg.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <libconfig.hh>
#include <util/log_system.h>
#include <util/file_system.h>
using namespace libconfig;
using namespace std;



namespace ui
{
	const char* default_cfg = R"EarthConfig(
		cfg={
			threadingModel=3;
			view={
				sky={ 
					active=false;
					light_active=false;
				};
				vsync=true; //´¹Ö±Í¬²½¿ªÆô·ñ
				windowDecoration=true;
				doubleBuffer=true;
				alpha=0;
				stencil=0;
			};
			display={
				multiSample=0; //¿¹¾â³Ýmess
				numMultiSample=0; //¿¹¾â³Ýmess
				minAlpha=0;
				minStencil=0;
			};
			plugins=(
				{
					name="logsystem";
					type="dll";
					path="/plugins/util_LogSystem";
					path_root="modulePath";
					active=true;	
					suiffixes={debug="d";RelWithDebInfo="rd";release="";};	
				},
				{
					name="filesystem";
					type="dll";
					path="/plugins/util_FileSystem";
					path_root="modulePath";
					active=true;	
					suiffixes={debug="d";RelWithDebInfo="rd";release="";};	
				},
			);
		}

)EarthConfig";



	//class EarthCfg::Setting
	//{
	//public:
	//	Setting()
	//	{
	//		
	//		try
	//		{
	//			_cfg.readString(default_cfg);
	//		}
	//		catch (const FileIOException &fioex)
	//		{
	//			int i = 0;
	//			i++;
	//		}
	//		catch (const ParseException &pex)
	//		{
	//			/*std::cout << "Parse error at " << pex.getLine() << ":" << pex.getLine()
	//				<< " - " << pex.getError() << std::endl;*/
	//		}
	//	};


	EarthCfg::EarthCfg()
	{
		m_pConfig = new libconfig::Config();
		_path = util::FileSystem::GetModulePath() + "/../cfg/earth.cfg";
		m_pConfig->readString(default_cfg);
	}

	EarthCfg::~EarthCfg()
	{
		m_pConfig->writeFile(_path.c_str());
		delete m_pConfig;
	}

	bool EarthCfg::loadCfg(const std::string& sPath)
	{
		libconfig::Config cfg;
		try
		{
			cfg.readFile(sPath.c_str());

			//¶ÁÈ¡ÊôÐÔ²¢¸²¸Ç
		}
		catch (const std::exception&)
		{

		}

		return true;
	}


	int EarthCfg::threadingModel() const
	{
		int nValue;
		auto& root = m_pConfig->getRoot()["cfg"];
		root.lookupValue("threadingModel", nValue);
		return nValue;
	}
	void EarthCfg::threadingModel(const int& data)
	{
		auto& root = m_pConfig->getRoot()["cfg"];
		root["threadingModel"] = data;
	}


	bool EarthCfg::view_sky_active() const
	{
		bool nValue;
		auto& root = m_pConfig->getRoot()["cfg"]["view"]["sky"];
		root.lookupValue("active", nValue);
		return nValue;
	}
	void EarthCfg::view_sky_active(const bool& data)
	{
		m_pConfig->getRoot()["cfg"]["view"]["sky"]["active"] = data;
	}


	bool EarthCfg::view_vsync() const
	{
		bool nValue;
		auto& root = m_pConfig->getRoot()["cfg"]["view"];
		root.lookupValue("vsync", nValue);
		return nValue;
	}

	void EarthCfg::view_vsync(const bool& data)
	{
		m_pConfig->getRoot()["cfg"]["view"]["vsync"] = data;
	}


	bool EarthCfg::view_windowDecoration() const
	{
		bool nValue;
		auto& root = m_pConfig->getRoot()["cfg"]["view"];
		root.lookupValue("windowDecoration", nValue);
		return nValue;
	}

	void EarthCfg::view_windowDecoration(const bool& data)
	{
		m_pConfig->getRoot()["cfg"]["view"]["windowDecoration"] = data;
	}


	bool EarthCfg::view_doubleBuffer() const
	{
		bool nValue;
		auto& root = m_pConfig->getRoot()["cfg"]["view"];
		root.lookupValue("doubleBuffer", nValue);
		return nValue;
	}

	void EarthCfg::view_doubleBuffer(const bool& data)
	{
		m_pConfig->getRoot()["cfg"]["view"]["doubleBuffer"] = data;
	}
	

	int EarthCfg::view_alpha() const
	{
		int nValue;
		auto& root = m_pConfig->getRoot()["cfg"]["view"];
		root.lookupValue("alpha", nValue);
		return nValue;
	}

	void EarthCfg::view_alpha(const int& data)
	{
		m_pConfig->getRoot()["cfg"]["view"]["alpha"] = data;
	}



	int EarthCfg::view_stencil() const
	{
		int nValue;
		auto& root = m_pConfig->getRoot()["cfg"]["view"];
		root.lookupValue("stencil", nValue);
		return nValue;
	}

	void EarthCfg::view_stencil(const int& data)
	{
		m_pConfig->getRoot()["cfg"]["view"]["stencil"] = data;
	}


	int EarthCfg::display_multiSample() const
	{
		int nValue;
		auto& root = m_pConfig->getRoot()["cfg"]["display"];
		root.lookupValue("multiSample", nValue);
		return nValue;
	}

	void EarthCfg::display_multiSample(const int& data)
	{
		m_pConfig->getRoot()["cfg"]["display"]["multiSample"] = data;
	}


	int EarthCfg::display_numMultiSample() const
	{
		int nValue;
		auto& root = m_pConfig->getRoot()["cfg"]["display"];
		root.lookupValue("numMultiSample", nValue);
		return nValue;
	}

	void EarthCfg::display_numMultiSample(const int& data)
	{
		m_pConfig->getRoot()["cfg"]["display"]["numMultiSample"] = data;
	}


	int EarthCfg::display_minAlpha() const
	{
		int nValue;
		auto& root = m_pConfig->getRoot()["cfg"]["display"];
		root.lookupValue("minAlpha", nValue);
		return nValue;
	}

	void EarthCfg::display_minAlpha(const int& data)
	{
		m_pConfig->getRoot()["cfg"]["display"]["minAlpha"] = data;
	}



	int EarthCfg::display_minStencil() const
	{
		int nValue;
		auto& root = m_pConfig->getRoot()["cfg"]["display"];
		root.lookupValue("minStencil", nValue);
		return nValue;
	}

	void EarthCfg::display_minStencil(const int& data)
	{
		m_pConfig->getRoot()["cfg"]["display"]["minStencil"] = data;
	}


	std::vector<EarthCfg::PluginsSetting> EarthCfg::getPluginsSetting() const
	{
		std::vector<EarthCfg::PluginsSetting> pluginsSetting;
		const Setting &plugins = m_pConfig->getRoot()["cfg"]["plugins"];
		int count = plugins.getLength();
		for (int i = 0;i < count;i++)
		{
			pluginsSetting.push_back(EarthCfg::PluginsSetting(plugins[i]));
		}
		
		return pluginsSetting;
	}

	EarthCfg::PluginsSetting::PluginsSetting(const libconfig::Setting& setting)
		: _setting(setting)
	{

	}

	std::string EarthCfg::PluginsSetting::name() const
	{
		std::string str;
		_setting.lookupValue("name", str);
		return str;
	}

	std::string EarthCfg::PluginsSetting::type() const
	{
		std::string str;
		_setting.lookupValue("type", str);
		return str;
	}

	std::string EarthCfg::PluginsSetting::path() const
	{
		std::string str;
		_setting.lookupValue("path", str);
		return str;
	}

	std::string EarthCfg::PluginsSetting::path_root() const
	{
		std::string str;
		_setting.lookupValue("path_root", str);
		return str;
	}

	bool EarthCfg::PluginsSetting::active() const
	{
		bool active;
		_setting.lookupValue("active", active);
		return active;
	}

	std::string EarthCfg::PluginsSetting::suffix() const
	{
		std::string str;
		_setting.lookupValue("suffix", str);


		std::string suffix = "";
		if (CMAKE_INTDIR == "RelWithDebInfo")
		{
			suffix = "rd";
		}
		else if (CMAKE_INTDIR == "Release")
		{

		}
		else
		{
			suffix = "d";
		}

		return suffix;
	}

}


