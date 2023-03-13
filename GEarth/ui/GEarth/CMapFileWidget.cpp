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
		//����style
		auto& pStyleSheetsCfg = CStyleSheetsCfg::instance();
		this->setObjectName("MapFileDialog");
		this->setStyleSheet("MapFileDialog");

		//�ļ�λ��

		//osgErath::Map::Config
		

		//��ͼ����

		//��ͼͶӰ
		

		//����
		QVBoxLayout* pMainLayout = pStyleSheetsCfg.createVBoxLayout();
		this->setLayout(pMainLayout);
		
		

		//�ļ�
		QHBoxLayout* pLayoutHeader1 = pStyleSheetsCfg.createHBoxLayout();
		
		QPushButton* browsePushButton = pStyleSheetsCfg.createPushButton();
		pLayoutHeader1->addWidget(browsePushButton);
		connect(browsePushButton, &QPushButton::clicked, this, &CMapFileDialog::selectEarth);

		_filePathLineEdit = pStyleSheetsCfg.createLineEdit();
		pLayoutHeader1->addWidget(_filePathLineEdit);

		//����ļ�
		pMainLayout->addLayout(pLayoutHeader1);


		//ѡ������ϵͳ

	}

	void CMapFileDialog::selectEarth()
	{
		auto& pStyleSheetsCfg = CStyleSheetsCfg::instance();
		QFileDialog* dlg = pStyleSheetsCfg.createFileDialog("QFileDialog");;
		dlg->setNameFilter("earth file(*.earth ) ;; ALL File (*.*)");
		dlg->setWindowTitle(tr("�򿪵�ͼ"));
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
