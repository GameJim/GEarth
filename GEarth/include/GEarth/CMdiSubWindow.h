#pragma once
#include "QtWidgets/qmdisubwindow"

#include <osg/ref_ptr>
#include <osgViewer/View>

#include <GEarth/earth_viewer_project.h>
namespace ui
{
	class CMdiSubWindow;
	class CCanva : public QWidget
	{
		Q_OBJECT
	public:
		CCanva(CMdiSubWindow* pMdiSubWindow, QWidget* parent = Q_NULLPTR);
		
		virtual ~CCanva() = default;

		protected slots:
		void paintEvent(QPaintEvent *paintEvent);
		QPaintEngine* paintEngine() const Q_DECL_OVERRIDE;
	public:
		CMdiSubWindow* m_pMdiSubWindow;
	};


	class CMdiSubWindow : public QMdiSubWindow {
		Q_OBJECT

	public:
		CMdiSubWindow(osg::ref_ptr<osgEarth::MapNode> pMapNode, QWidget* parent = Q_NULLPTR);
		~CMdiSubWindow();


		osg::ref_ptr<osgViewer::View> getView() const;
		osg::ref_ptr<osgEarth::MapNode> getMapNode() const;
	protected:
		
		protected slots:
		void closeEvent(QCloseEvent *closeEvent) Q_DECL_OVERRIDE;
	private:
		//数据资源
		osg::ref_ptr<osgViewer::View> m_pView;

		//当前活动的MapNode
		osg::ref_ptr<osgEarth::MapNode> m_pMapNode;
	};
}


