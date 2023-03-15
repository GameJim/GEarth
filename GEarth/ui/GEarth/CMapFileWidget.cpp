//#include <windows.h>
//#include <windowsx.h>

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
		:CBaseDialog(parent)
	{
		initContentWidget();

		//this->setWindowFlags(Qt::FramelessWindowHint);
		//加入style
		auto& pStyleSheetsCfg = CStyleSheetsCfg::instance();
		this->setObjectName("MapFileDialog");
		this->setWindowTitle("Map");

		// 设置标题栏跑马灯效果，可以不设置;
		//m_titleBar->setTitleRoll();
		m_titleBar->setTitleIcon(":/icon/res/file-eye.png");
		m_titleBar->setTitleContent(QStringLiteral("加载地图"));
		m_titleBar->setButtonType(MIN_MAX_BUTTON);
		//m_titleBar->setTitleWidth(this->width());


		this->setStyleSheet(pStyleSheetsCfg.getUIStyle("CMapFileDialog"));
		//文件位置
		
		
		//osgErath::Map::Config
		

		//地图名称

		//地图投影
		

		//缓存
		


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

	void CMapFileDialog::initContentWidget()
	{
		auto& pStyleSheetsCfg = CStyleSheetsCfg::instance();
		QVBoxLayout* pMainLayout = pStyleSheetsCfg.createVBoxLayout();
		m_contentWidget->setLayout(pMainLayout);


		//文件
		QHBoxLayout* pLayoutHeader1 = pStyleSheetsCfg.createHBoxLayout();
		
		QPushButton* browsePushButton = pStyleSheetsCfg.createPushButton("browse");
		pLayoutHeader1->addWidget(browsePushButton);
		connect(browsePushButton, &QPushButton::clicked, this, &CMapFileDialog::selectEarth);

		_filePathLineEdit = pStyleSheetsCfg.createLineEdit();
		pLayoutHeader1->addWidget(_filePathLineEdit);

		//添加文件
		pMainLayout->addLayout(pLayoutHeader1);
	}

	//bool CMapFileDialog::nativeEvent(const QByteArray &eventType, void *message, long *result)
	//{
	//	MSG* msg = (MSG*)message;
	//	switch (msg->message)
	//	{
	//	case WM_NCHITTEST:
	//		int xPos = GET_X_LPARAM(msg->lParam) - this->frameGeometry().x();
	//		int yPos = GET_Y_LPARAM(msg->lParam) - this->frameGeometry().y();
	//		if (this->childAt(xPos, yPos) == 0)
	//		{
	//			*result = HTCAPTION;
	//		}
	//		else {
	//			return false;
	//		}
	//		if (xPos > 0 && xPos < 8)
	//			*result = HTLEFT;
	//		if (xPos > (this->width() - 8) && xPos < (this->width() - 0))
	//			*result = HTRIGHT;
	//		if (yPos > 0 && yPos < 8)
	//			*result = HTTOP;
	//		if (yPos > (this->height() - 8) && yPos < (this->height() - 0))
	//			*result = HTBOTTOM;
	//		if (xPos > 18 && xPos < 22 && yPos > 18 && yPos < 22)
	//			*result = HTTOPLEFT;
	//		if (xPos > (this->width() - 22) && xPos < (this->width() - 18) && yPos > 18 && yPos < 22)
	//			*result = HTTOPRIGHT;
	//		if (xPos > 18 && xPos < 22 && yPos >(this->height() - 22) && yPos < (this->height() - 18))
	//			*result = HTBOTTOMLEFT;
	//		if (xPos > (this->width() - 22) && xPos < (this->width() - 18) && yPos >(this->height() - 22) && yPos < (this->height() - 18))
	//			*result = HTBOTTOMRIGHT;
	//		return true;
	//	}
	//	return false;
	//}

	

}
