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
		auto hasValue = [](const Setting &setting, const std::string& sValue)
		{
			if (setting.isArray() || setting.isList())
			{
				for (int i = 0; i < setting.getLength(); i++)
				{
					std::string value = setting[i].c_str();
					if (value == sValue)
						return true;
				}
			}

			return false;
		};

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

		//菜初始化
		Setting &root = _pConfig->getRoot();

		if (!root.exists("qss"))
			root.add("qss", Setting::TypeGroup);

		Setting &qss = root["qss"];

		if (!qss.exists("demo"))
			qss.add("demo", Setting::TypeGroup);

		Setting &demo = qss["demo"];

		
		if (!demo.exists("paths"))
		{
			demo.add("paths", Setting::TypeArray);
			Setting &paths = demo["paths"];
			paths.add(Setting::TypeString) = _defult_css_path;
		}
		else
		{
			Setting &paths = demo["paths"];
			if (!hasValue(paths, _defult_css_path))
			{
				paths.add(Setting::TypeString) = _defult_css_path;
			}
		}


		if (!demo.exists("typeGroup"))
		{
			demo.add("typeGroup", Setting::TypeGroup);
		}
		if (!demo.exists("custom"))
		{
			demo.add("custom", Setting::TypeGroup);
		}
	}

	QString CStyleSheetsCfg::getStyle(const std::string& sType, const std::string& skey/*="default"*/)
	{
		std::string sfitPath;
		Setting &root = _pConfig->getRoot();

		//相对路径list
		Setting &paths = root["qss"]["demo"]["paths"];
		std::vector<std::string> root_paths;
		for (int i = 0; i < paths.getLength(); i++)
		{
			root_paths.push_back(paths[i].c_str());
		}

		Setting &type = root["qss"]["demo"]["typeGroup"];
		if (!type.exists(sType))
		{
			Setting & sTypeGroup = type.add(sType, Setting::TypeGroup);
			sTypeGroup.add(skey, Setting::TypeString) = sType + "/" + skey + ".css";
		}
		else
		{
			Setting & sTypeGroup = type[sType];
			if (!sTypeGroup.exists(skey))
			{
				sTypeGroup.add(skey, Setting::TypeString) = sType+"/" + skey + ".css";
			}
			else
			{
				sTypeGroup.lookupValue(skey, sfitPath);

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
		}

		
		//如果存在该文件加载文件
		if (!sfitPath.empty() && util::FileSystem::instance()->fileExists(sfitPath))
		{
			QFile file(sfitPath.c_str());
			file.open(QFile::ReadOnly);
			if (file.isOpen())
			{
				return  QLatin1String(file.readAll());
			}
		}
		return "";
	}

	QString CStyleSheetsCfg::getUIStyle(const std::string& sType)
	{
		std::string sfitPath ="";
		Setting &root = _pConfig->getRoot();

		Setting &custom = root["qss"]["demo"]["custom"];

		Setting &paths = root["qss"]["demo"]["paths"];

		//相对路径list
		std::vector<std::string> root_paths;
		for (int i = 0; i < paths.getLength(); i++)
		{
			root_paths.push_back(paths[i].c_str());
		}

		if (!custom.exists(sType))
		{
			custom.add(sType, Setting::TypeString) = sType + ".css";
		}
		else
		{
			custom.lookupValue(sType, sfitPath);
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

		//如果存在该文件加载文件
		if (!sfitPath.empty() && util::FileSystem::instance()->fileExists(sfitPath))
		{
			QFile file(sfitPath.c_str());
			file.open(QFile::ReadOnly);
			if (file.isOpen())
			{
				return QLatin1String(file.readAll());
			}
		}
		return "";
	}

	QPushButton* CStyleSheetsCfg::createPushButton(const QString& skey /*= "default"*/)
	{
		QPushButton* pPushButton = new QPushButton();
		pPushButton->setText(skey);
		pPushButton->setObjectName(skey);

		pPushButton->setStyleSheet(instance().getStyle("QPushButton", skey.toStdString()));

		return pPushButton;
	}

	QHBoxLayout* CStyleSheetsCfg::createHBoxLayout(const QString& skey /*= "default"*/)
	{
		QHBoxLayout* pLayout = new QHBoxLayout();
		/*pPushButton->setObjectName(QString::fromLocal8Bit(skey.c_str()));

		pPushButton->setStyleSheet(QString::fromLocal8Bit(instance().getStyle("QHBoxLayout", skey).c_str()));*/

		return pLayout;
	}

	QVBoxLayout* CStyleSheetsCfg::createVBoxLayout( const QString& skey /*= "default"*/)
	{
		QVBoxLayout* pLayout = new QVBoxLayout();
		/*pPushButton->setObjectName(QString::fromLocal8Bit(skey.c_str()));

		pPushButton->setStyleSheet(QString::fromLocal8Bit(instance().getStyle("QHBoxLayout", skey).c_str()));*/

		return pLayout;
	}

	QLineEdit* CStyleSheetsCfg::createLineEdit(const QString& skey /*= "default"*/)
	{
		QLineEdit* pQWidget = new QLineEdit();
		pQWidget->setObjectName(skey);

		pQWidget->setStyleSheet(instance().getStyle("QLineEdit", skey.toStdString()));

		return pQWidget;
	}

	QComboBox* CStyleSheetsCfg::createComboBox(const QString& skey /*= "default"*/)
	{
		QComboBox* pQWidget = new QComboBox();
		pQWidget->setObjectName(skey);
		pQWidget->setStyleSheet(instance().getStyle("QComboBox", skey.toStdString()));
		

		return pQWidget;
	}

	QFileDialog * CStyleSheetsCfg::createFileDialog(const QString& skey /*= "default"*/)
	{
		QFileDialog* pQWidget = new QFileDialog();
		pQWidget->setObjectName(skey);
		pQWidget->setStyleSheet(instance().getStyle("QFileDialog", skey.toStdString()));
		return pQWidget;
	}

	CStyleSheetsCfg::CStyleSheetsCfg()
	{
		_default = util::FileSystem::GetModulePath() + "/../data/cfg/earth_qss.cfg";
		_defult_css_path =  util::FileSystem::GetModulePath() + "/../res/ui/GEarth/qss";
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

