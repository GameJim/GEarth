#pragma once
#include <string>
#include <map>
#include <util/define_helper.h>
#include "QIcon.h"

namespace libconfig
{
	class Config;
}
namespace ui
{
	class ImageCfg
	{
		DEFINE_SINGLETON(ImageCfg)
	public:
		enum EImgSpecs
		{
			EIS_NORMAL = 0,
			EIS_SMAlLL,
			EIS_BIG
		};

		
	public:
		virtual void praseCfg(const std::string& path = "");
		//��ȡicon
		QIcon getIcon(const std::string& sKey, const EImgSpecs& eImgSpecs = EImgSpecs::EIS_NORMAL);
	protected:
		ImageCfg();
	public:
		virtual ~ImageCfg();

		std::string getImagePath(const std::string& sKey,const EImgSpecs& eImgSpecs  = EImgSpecs::EIS_NORMAL);
	public:

		std::string _default;                 //Ĭ��·��
		std::string _cfgPath;                 //Ĭ��cfg·��

		std::string _defult_res;
		libconfig::Config* _pConfig;
	};

}


