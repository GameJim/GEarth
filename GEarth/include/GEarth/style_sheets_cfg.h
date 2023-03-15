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
class QString;
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

		

		
	public:
		//������Ӧ�ؼ�
		QString getUIStyle(const std::string& sType);
		//��ȡstyle
		QString getStyle(const std::string& sType, const std::string& skey = "default");

		QPushButton* createPushButton(const QString& skey = "default");
		QHBoxLayout* createHBoxLayout(const QString& skey = "default");
		QVBoxLayout* createVBoxLayout(const QString& skey = "default");
		QLineEdit* createLineEdit(const QString& skey = "default");
		QComboBox * createComboBox(const QString& skey = "default");
		QFileDialog * createFileDialog(const QString& skey = "default");
	protected:
		CStyleSheetsCfg();
	public:
		virtual ~CStyleSheetsCfg();
	public:
		std::string _default;                 //Ĭ��·�� cfg·��
		std::string _defult_css_path;
		libconfig::Config* _pConfig;
	};

}




