#include "GEarth/CBaseDialog.h"

#include <windows.h>
#include <windowsx.h>
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <QFile>
#include <QHBoxLayout>

#include "GEarth/style_sheets_cfg.h"
namespace ui
{
	CBaseDialog::CBaseDialog(QWidget *parent)
		: QDialog(parent)
	{
		// FramelessWindowHint属性设置窗口去除边框;
		// WindowMinimizeButtonHint 属性设置在窗口最小化时，点击任务栏窗口可以显示出原窗口;
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
		// 设置窗口背景透明;
		setAttribute(Qt::WA_TranslucentBackground);
		// 初始化标题栏;
		initTitleBar();

		//initContentWidget();

		auto& pStyleSheetsCfg = CStyleSheetsCfg::instance();
		this->setStyleSheet(pStyleSheetsCfg.getUIStyle("CBaseDialogWindow"));
	}

	CBaseDialog::~CBaseDialog()
	{

	}

	void CBaseDialog::initTitleBar()
	{
		//添加一个水平布局
		
		QVBoxLayout* pLayoutHeader1 =  new QVBoxLayout();
		this->setLayout(pLayoutHeader1);

		pLayoutHeader1->setContentsMargins(1, 0, 0, 1);
		pLayoutHeader1->setSpacing(0);

		m_titleBar = new CBaseTitleBar();
		m_titleBar->move(0, 0);

		connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
		connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
		connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
		connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));

		pLayoutHeader1->addWidget(m_titleBar);

		//内容窗口
		m_contentWidget = new QWidget();
		pLayoutHeader1->addWidget(m_contentWidget);
	}

	void CBaseDialog::paintEvent(QPaintEvent* event)
	{
		QPainter painter(this);
		QPainterPath pathBack;
		pathBack.setFillRule(Qt::WindingFill);
		pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
		painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
		painter.fillPath(pathBack, painter.background().color());

		return QWidget::paintEvent(event);
	}

	bool CBaseDialog::nativeEvent(const QByteArray &eventType, void *message, long *result)
	{
		MSG* msg = (MSG*)message;
		switch (msg->message)
		{
		case WM_NCHITTEST:
			int xPos = GET_X_LPARAM(msg->lParam) - this->frameGeometry().x();
			int yPos = GET_Y_LPARAM(msg->lParam) - this->frameGeometry().y();
			if (this->childAt(xPos, yPos) == 0)
			{
				*result = HTCAPTION;
			}
			else {
				return false;
			}
			if (xPos > 0 && xPos < 8)
				*result = HTLEFT;
			if (xPos > (this->width() - 8) && xPos < (this->width() - 0))
				*result = HTRIGHT;
			if (yPos > 0 && yPos < 8)
				*result = HTTOP;
			if (yPos > (this->height() - 8) && yPos < (this->height() - 0))
				*result = HTBOTTOM;
			if (xPos > 18 && xPos < 22 && yPos > 18 && yPos < 22)
				*result = HTTOPLEFT;
			if (xPos > (this->width() - 22) && xPos < (this->width() - 18) && yPos > 18 && yPos < 22)
				*result = HTTOPRIGHT;
			if (xPos > 18 && xPos < 22 && yPos >(this->height() - 22) && yPos < (this->height() - 18))
				*result = HTBOTTOMLEFT;
			if (xPos > (this->width() - 22) && xPos < (this->width() - 18) && yPos >(this->height() - 22) && yPos < (this->height() - 18))
				*result = HTBOTTOMRIGHT;
			return true;
		}
		return false;
	}

	void CBaseDialog::onButtonMinClicked()
	{
		if (Qt::Tool == (windowFlags() & Qt::Tool))
		{
			hide();    //设置了Qt::Tool 如果调用showMinimized()则窗口就销毁了？？？
		}
		else
		{
			showMinimized();
		}
	}

	void CBaseDialog::onButtonRestoreClicked()
	{
		QPoint windowPos;
		QSize windowSize;
		m_titleBar->getRestoreInfo(windowPos, windowSize);
		this->setGeometry(QRect(windowPos, windowSize));
	}

	void CBaseDialog::onButtonMaxClicked()
	{
		m_titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
		QRect desktopRect = QApplication::desktop()->availableGeometry();
		QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
		setGeometry(FactRect);
	}

	void CBaseDialog::onButtonCloseClicked()
	{
		close();
	}

}


