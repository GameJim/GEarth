#pragma once

#pragma once
#include <string>
#include <map>
#include <util/define_helper.h>
#include "GEarth/api_description.h"

class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QLineEdit;
class QComboBox;
class QFileDialog;
namespace libconfig
{
	class Config;
}
namespace ui
{
	class EARTH_API CStyleSheetsCfg
	{
		DEFINE_SINGLETON(CStyleSheetsCfg)
	public:
		virtual void praseCfg(const std::string& path = "");

		//获取style
		std::string getStyle(const std::string& sType, const std::string& skey="default");

		//创建对应控件
		std::string getUIStyle(const std::string& sType);
	public:
		QPushButton* createPushButton(const std::string& skey = "default");
		QHBoxLayout* createHBoxLayout(const std::string& skey = "default");
		QVBoxLayout* createVBoxLayout(const std::string& skey = "default");
		QLineEdit* createLineEdit(const std::string& skey = "default");
		QComboBox * createComboBox(const std::string& skey = "default");
		QFileDialog * createFileDialog(const std::string& skey = "default");
	protected:
		CStyleSheetsCfg();
	public:
		virtual ~CStyleSheetsCfg();
	public:
		std::string _default;                 //默认路径

		libconfig::Config* _pConfig;
	};

}




