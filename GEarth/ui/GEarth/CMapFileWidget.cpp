#include "QHBoxLayout"
#include "QHBoxLayout"
#include "QPushButton"
#include "QLineEdit"
#include <QFileDialog>

#include "osgEarth/URI"

#include "util/file_system.h"


#include "GEarth/CMapFileWidget.h"
#include "GEarth/image_cfg.h"
#include "GEarth/style_sheets_cfg.h"

namespace ui
{

	CMapFileDialog::CMapFileDialog(QWidget* parent /*= Q_NULLPTR*/)
	{
		//加入style
		auto& pStyleSheetsCfg = CStyleSheetsCfg::instance();
		this->setObjectName("MapFileDialog");
		this->setStyleSheet("MapFileDialog");

		//文件位置

		//osgErath::Map::Config
		

		//地图名称

		//地图投影
		

		//缓存
		QVBoxLayout* pMainLayout = pStyleSheetsCfg.createVBoxLayout();
		this->setLayout(pMainLayout);
		
		

		//文件
		QHBoxLayout* pLayoutHeader1 = pStyleSheetsCfg.createHBoxLayout();
		
		QPushButton* browsePushButton = pStyleSheetsCfg.createPushButton();
		pLayoutHeader1->addWidget(browsePushButton);
		connect(browsePushButton, &QPushButton::clicked, this, &CMapFileDialog::selectEarth);

		_filePathLineEdit = pStyleSheetsCfg.createLineEdit();
		pLayoutHeader1->addWidget(_filePathLineEdit);

		//添加文件
		pMainLayout->addLayout(pLayoutHeader1);


		//选择坐标系统

	}

	void CMapFileDialog::selectEarth()
	{
		auto& pStyleSheetsCfg = CStyleSheetsCfg::instance();
		QFileDialog* dlg = pStyleSheetsCfg.createFileDialog("QFileDialog");;
		dlg->setNameFilter("earth file(*.earth ) ;; ALL File (*.*)");
		dlg->setWindowTitle(tr("打开地图"));
		if (dlg->exec() == QFileDialog::Accepted)
		{
			QStringList fileList = dlg->selectedFiles();

			for (auto file : fileList)
			{
				_filePathLineEdit->setText(file);
			}
		}
	}

}
