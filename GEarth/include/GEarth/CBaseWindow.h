#pragma once

#include <QWidget>
#include <QMainWindow>
#include "GEarth/CBaseTitleBar.h"
//https://www.jianshu.com/p/1b91fd2beb85
namespace ui
{
	class BaseWindow : public QWidget
	{
		Q_OBJECT

	public:
		BaseWindow(QWidget *parent = 0);
		~BaseWindow();

	private:
		void initTitleBar();
		void paintEvent(QPaintEvent *event);

		void loadStyleSheet(const QString &sheetName);
		private slots:
		void onButtonMinClicked();
		void onButtonRestoreClicked();
		void onButtonMaxClicked();
		void onButtonCloseClicked();

	protected:
		CBaseTitleBar* m_titleBar;
	};



}

