#pragma once
#include "qdialog.h"
#include "GEarth/api_description.h"
#include "GEarth/CBaseDialog.h"
namespace ui
{
	namespace osgDB
	{
		class Options;
	}

	class EARTH_API CMapFileDialog : public CBaseDialog
	{
		Q_OBJECT
	public:
		CMapFileDialog(QWidget* parent = Q_NULLPTR);
		virtual ~CMapFileDialog() = default;

	
		private slots:
		void selectEarth();
	public:
		//获取配置信息的
	
	protected:
		//bool nativeEvent(const QByteArray &eventType, void *message, long *result);
		virtual void initContentWidget() override;
	protected:
		QLineEdit* _filePathLineEdit;
	};

}

