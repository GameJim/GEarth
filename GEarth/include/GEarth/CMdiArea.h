#pragma once
#include <osg/ref_ptr>
#include <atomic>
#include <thread>
#include <QMdiArea>
#include <future>

#include "QTimer"
#include <osgViewer/CompositeViewer>
#include <util/mailbox.h>
#include <util/thread_pool.h>

namespace osgEarth
{
	class Map;
	class MapNode;
}
namespace ui
{
	class CMdiSubWindow;
	class CMdiArea : public QMdiArea {
		Q_OBJECT

	public:
		CMdiArea(QWidget * parent = Q_NULLPTR);
		virtual ~CMdiArea();
	public:
		virtual void createMdiSubWindow(osg::ref_ptr<osgEarth::Map> pMap);
		virtual void createMdiSubWindow(osg::ref_ptr<osgEarth::MapNode> pMapNode);
		virtual void destoryMdiSubWindow(CMdiSubWindow* pMdiSubWindow);
		//virtual void immediatelDraw();
		public slots:

	public:
		bool waitFrameFinished(const uint32_t& microseconds = 10);  //等待渲染结束
		void pushMessage(std::unique_ptr<util::Message> message);
		bool Frame();          //渲染消息
		void NotyFrame();      //通知渲染消息
	protected:
		void receiveMessage(std::unique_ptr<util::Message> message);//对收到的消息进行加工
	protected:
		osg::ref_ptr<osgViewer::CompositeViewer> m_pCompositeViewer;
		std::mutex  m_mutex; //渲染器操作安全

		//渲染相关
		std::shared_ptr<util::Mailbox> m_pMailBox;
		std::shared_ptr<util::SequencedScheduler> m_pScheduler;

		std::atomic<bool>  m_bFramOnce;
		std::atomic<bool>  m_needFrame;
		
	};
}
