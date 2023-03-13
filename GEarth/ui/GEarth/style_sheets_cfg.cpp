#include "QHBoxLayout"
#include "QHBoxLayout"
#include "QPushButton"
#include "QLineEdit"
#include "QFileDialog"
#include "QComboBox"

#include <libconfig.hh>
using namespace libconfig;

#include "util/file_system.h"

#include "GEarth/style_sheets_cfg.h"

namespace ui
{

	void CStyleSheetsCfg::praseCfg(const std::string& path /*= ""*/)
	{
		try
		{
			if (!path.empty())
			{
				_pConfig->readFile(path.c_str());
				_default = path;
			}
			else
			{
				_pConfig->readFile(_default.c_str());
			}
		}
		catch (const FileIOException &fioex)
		{

		}
		catch (const ParseException &pex)
		{
		}

		//²Ë³õÊ¼»¯
		Setting &root = _pConfig->getRoot();

		if (!root.exists("qss"))
			root.add("qss", Setting::TypeGroup);

		Setting &qss = root["qss"];

		if (!qss.exists("demo"))
			qss.add("demo", Setting::TypeGroup);

		Setting &demo = qss["demo"];


		if (!demo.exists("typeGroup"))
		{
			demo.add("typeGroup", Setting::TypeGroup);
		}
		if (!demo.exists("custom"))
		{
			demo.add("custom", Setting::TypeGroup);
		}
	}

	std::string CStyleSheetsCfg::getStyle(const std::string& sType, const std::string& skey/*="default"*/)
	{
		std::string sStyle;
		Setting &root = _pConfig->getRoot();

		Setting &type = root["qss"]["demo"]["typeGroup"];
		if (!type.exists(sType))
		{
			Setting & sTypeGroup = type.add(sType, Setting::TypeGroup);
			sTypeGroup.add(skey, Setting::TypeString) = "";
		}
		else
		{
			Setting & sTypeGroup = type[sType];
			if (!sTypeGroup.exists(skey))
			{
				sTypeGroup.add(skey, Setting::TypeString) = "";
			}
			else
			{
				sTypeGroup.lookupValue(sType, sStyle);
				return sStyle;
			}
		}

		/*if (skey == "default")
		{
			Setting &default = root["qss"]["demo"]["default"];
			if (!default.exists(sType))
			{
				default.add(sType, Setting::TypeString)="";
			}
			else
			{
				default.lookupValue(sType, sStyle);
				return sStyle;
			}
		}
		else
		{
			
		}*/

		return sStyle;
	}

	std::string CStyleSheetsCfg::getUIStyle(const std::string& sType)
	{
		std::string sStyle="";
		Setting &root = _pConfig->getRoot();

		Setting &custom = root["qss"]["demo"]["custom"];


		if (!custom.exists(sType))
		{
			custom.add(sType, Setting::TypeString) = "";
		}
		else
		{
			custom.lookupValue(sType, sStyle);
			return sStyle;
		}

		return sStyle;
	}

	QPushButton* CStyleSheetsCfg::createPushButton(const std::string& skey /*= "default"*/)
	{
		QPushButton* pPushButton = new QPushButton();
		pPushButton->setObjectName(QString::fromLocal8Bit(skey.c_str()));

		pPushButton->setStyleSheet(QString::fromLocal8Bit(instance().getStyle("QPushButton", skey).c_str()));

		return pPushButton;
	}

	QHBoxLayout* CStyleSheetsCfg::createHBoxLayout(const std::string& skey /*= "default"*/)
	{
		QHBoxLayout* pLayout = new QHBoxLayout();
		/*pPushButton->setObjectName(QString::fromLocal8Bit(skey.c_str()));

		pPushButton->setStyleSheet(QString::fromLocal8Bit(instance().getStyle("QHBoxLayout", skey).c_str()));*/

		return pLayout;
	}

	QVBoxLayout* CStyleSheetsCfg::createVBoxLayout( const std::string& skey /*= "default"*/)
	{
		QVBoxLayout* pLayout = new QVBoxLayout();
		/*pPushButton->setObjectName(QString::fromLocal8Bit(skey.c_str()));

		pPushButton->setStyleSheet(QString::fromLocal8Bit(instance().getStyle("QHBoxLayout", skey).c_str()));*/

		return pLayout;
	}

	QLineEdit* CStyleSheetsCfg::createLineEdit(const std::string& skey /*= "default"*/)
	{
		QLineEdit* pQWidget = new QLineEdit();
		pQWidget->setObjectName(QString::fromLocal8Bit(skey.c_str()));

		pQWidget->setStyleSheet(QString::fromLocal8Bit(instance().getStyle("QLineEdit", skey).c_str()));

		return pQWidget;
	}

	QComboBox* CStyleSheetsCfg::createComboBox(const std::string& skey /*= "default"*/)
	{
		QComboBox* pQWidget = new QComboBox();
		pQWidget->setObjectName(QString::fromLocal8Bit(skey.c_str()));

		pQWidget->setStyleSheet(QString::fromLocal8Bit(instance().getStyle("QComboBox", skey).c_str()));

		return pQWidget;
	}

	QFileDialog * CStyleSheetsCfg::createFileDialog(const std::string& skey /*= "default"*/)
	{
		QFileDialog* pQWidget = new QFileDialog();
		pQWidget->setObjectName(QString::fromLocal8Bit(skey.c_str()));

		pQWidget->setStyleSheet(QString::fromLocal8Bit(instance().getStyle("QComboBox", skey).c_str()));
		return pQWidget;
	}

	CStyleSheetsCfg::CStyleSheetsCfg()
	{
		_default = util::FileSystem::GetModulePath() + "/../res/ui/GEarth/qss.cfg";

		_pConfig = new libconfig::Config();
		this->praseCfg(_default);
	}

	CStyleSheetsCfg::~CStyleSheetsCfg()
	{
		_pConfig->writeFile(_default.c_str());
		delete _pConfig;
		_pConfig = nullptr;
	}

}

