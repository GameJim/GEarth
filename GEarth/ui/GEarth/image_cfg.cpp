#include "GEarth/image_cfg.h"
#include "util/file_system.h"

#include <libconfig.hh>

using namespace libconfig;
namespace ui
{
	const char* default_image_cfg = R"ImageConfig(
	cfg={
			dirs:[],
			images:{
				dirs:[]
				key:"add",
				images:{
					"noraml":"xx.png",
					"small":"xx.png",
					"big":"xx.png",
				}
			},
			{
				dirs:[]
				key:"add",
				images:{
					"noraml":"xx.png",
					"small":"xx.png",
					"big":"xx.png",
				}
			}
		
		}
	)ImageConfig";

	bool hasValue(const Setting &setting,const std::string& sValue)
	{
		if (setting.isArray() || setting.isList())
		{
			for (int i = 0;i < setting.getLength();i++)
			{
				std::string value = setting[i].c_str();
				if (value == sValue)
					return true;
			}
		}

		return false;
	}

	ImageCfg::ImageCfg()
	{
		_default = util::FileSystem::GetModulePath() + "/../res/ui/GEarth/Share.png";

		_cfgPath = util::FileSystem::GetModulePath() + "/../res/ui/GEarth/img.cfg";
		_defult_res = util::FileSystem::GetModulePath() + "/../res/ui/GEarth";
		_pConfig = new libconfig::Config();
		
	}

	std::string ImageCfg::getImagePath(const std::string& sKey, const EImgSpecs& eImgSpecs)
	{
		std::string sfitPath="";
		//找纹理如果找不到，则加入被需要纹理列表
		Setting &root = _pConfig->getRoot();
		Setting &icons = root["images"]["icons"];

		//获取所有可能的路径
		{
			Setting &paths = icons["paths"];

			//相对路径list
			std::vector<std::string> root_paths;
			for (int i = 0;i < paths.getLength();i++)
			{
				root_paths.push_back(paths[i].c_str());
			}
		
			//文件
			Setting &iconlist = icons["icon_list"];
			if (iconlist.exists(sKey))
			{
				Setting& icon = iconlist[sKey];
				std::string s;
				s += 'a' + eImgSpecs;
				if (icon.exists(s))
				{
					icon.lookupValue(s, sfitPath);
					if (!sfitPath.empty())
					{
						if (util::FileSystem::instance()->isAbsPath(sfitPath))
						{
							sfitPath = sfitPath;
						}
						else
						{
							//寻找相对路径
							sfitPath = util::FileSystem::instance()->searchFile(root_paths, sfitPath);
						}
					}
				}
				else
				{
					icon.add(std::to_string(eImgSpecs), Setting::TypeString) = "";
				}
			}
			else
			{
				Setting& icon = iconlist.add(sKey, Setting::TypeGroup);
				std::string s;
				s += 'a' + eImgSpecs;
				icon.add(s, Setting::TypeString) = "..";
			}
		}

		if (sfitPath.empty())
		{
			return _default;
		}
	
		return sfitPath;
	}

	void ImageCfg::praseCfg(const std::string& path)
	{
		try
		{
			if (!path.empty())
			{
				_pConfig->readFile(path.c_str());
				_cfgPath = path;
			}
			else
			{
				_pConfig->readFile(_cfgPath.c_str());
			}
		}
		catch (const FileIOException &fioex)
		{

		}
		catch (const ParseException &pex)
		{
		}

		//菜初始化
		Setting &root = _pConfig->getRoot();

		if (!root.exists("images"))
			root.add("images", Setting::TypeGroup);

		Setting &images = root["images"];

		if (!images.exists("icons"))
			images.add("icons", Setting::TypeGroup);

		Setting &icons = images["icons"];

		if (!icons.exists("paths"))
		{
			icons.add("paths", Setting::TypeArray);
			Setting &paths = icons["paths"];
			paths.add(Setting::TypeString) = _defult_res;
		}
		else
		{
			Setting &paths = icons["paths"];
			if (!hasValue(paths,_defult_res))
			{
				paths.add(Setting::TypeString) = _defult_res;
			}
		}
		
		if (!icons.exists("icon_list"))
		{
			icons.add("icon_list", Setting::TypeGroup);
		}
	}

	QIcon ImageCfg::getIcon(const std::string& sKey, const EImgSpecs& eImgSpecs /*= EImgSpecs::EIS_NORMAL*/)
	{
		std::string path = getImagePath(sKey, eImgSpecs);
		return QIcon(path.c_str());
	}


	ImageCfg::~ImageCfg()
	{
		
		_pConfig->writeFile(_cfgPath.c_str());
		delete _pConfig;
		_pConfig = nullptr;

	}
};


