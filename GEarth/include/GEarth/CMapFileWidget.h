#pragma once
#include "qdialog.h"
#include "GEarth/api_description.h"
namespace ui
{
	namespace osgDB
	{
		class Options;
	}

	class EARTH_API CMapFileDialog : public QDialog
	{
		Q_OBJECT
	public:
		CMapFileDialog(QWidget* parent = Q_NULLPTR);
		virtual ~CMapFileDialog() = default;

		private slots:
		void selectEarth();
	public:
		//��ȡ������Ϣ��
	protected:
		QLineEdit* _filePathLineEdit;
	};

}

