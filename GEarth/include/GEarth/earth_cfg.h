#pragma once

#include <util/define_helper.h>
#include <string>
#include <map>
#include <vector>
#define DEFINE_ATTRIBUTE_FUNC(TYPE, NAME)  \
	TYPE NAME() const ;                \
    void NAME(const TYPE& data);

namespace libconfig
{
	class Config;
	class Setting;
};
namespace ui
{
	
	class EarthCfg
	{
		DEFINE_SINGLETON(EarthCfg)
		DEFINE_PROPERTY(std::string, path)
	public:
		// Ù–‘
		DEFINE_ATTRIBUTE_FUNC(int, threadingModel)

		DEFINE_ATTRIBUTE_FUNC(bool, view_sky_active)
		DEFINE_ATTRIBUTE_FUNC(bool, view_vsync)
		DEFINE_ATTRIBUTE_FUNC(bool, view_windowDecoration)
		DEFINE_ATTRIBUTE_FUNC(bool, view_doubleBuffer)
		DEFINE_ATTRIBUTE_FUNC(int, view_alpha)
		DEFINE_ATTRIBUTE_FUNC(int, view_stencil)

		DEFINE_ATTRIBUTE_FUNC(int, display_multiSample)
		DEFINE_ATTRIBUTE_FUNC(int, display_numMultiSample)
		DEFINE_ATTRIBUTE_FUNC(int, display_minAlpha)
		DEFINE_ATTRIBUTE_FUNC(int, display_minStencil)

	public:
		class PluginsSetting
		{
		public:
			PluginsSetting(const libconfig::Setting&);
			~PluginsSetting() = default;
			std::string name() const;
			std::string type() const;
			std::string path() const;
			std::string path_root() const;
			bool active() const;
			std::string suffix() const;
		protected:
			const libconfig::Setting& _setting;
		};
		//

		std::vector<PluginsSetting> getPluginsSetting() const;
	protected:
		EarthCfg();

		
	public:
		 virtual ~EarthCfg();
		 bool loadCfg(const std::string& sPath);
	private:
		libconfig::Config* m_pConfig;
	};
}
