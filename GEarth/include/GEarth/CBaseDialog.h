#pragma once
#include <QDialog>
#include <QMainWindow>
#include "GEarth/CBaseTitleBar.h"
//https://www.jianshu.com/p/1b91fd2beb85

#include <GEarth/api_description.h>
namespace ui
{
	class EARTH_API CBaseDialog : public QDialog
	{
		Q_OBJECT

	public:
		CBaseDialog(QWidget *parent = 0);
		~CBaseDialog();

	protected:
		virtual void initContentWidget() = 0;
	private:
		void initTitleBar();
		void paintEvent(QPaintEvent *event);

		//void loadStyleSheet(const QString &sheetName);
		private slots:
		void onButtonMinClicked();
		void onButtonRestoreClicked();
		void onButtonMaxClicked();
		void onButtonCloseClicked();

	protected:
		bool nativeEvent(const QByteArray &eventType, void *message, long *result);
	protected:
		CBaseTitleBar* m_titleBar;
		QWidget* m_contentWidget;
	};


}



