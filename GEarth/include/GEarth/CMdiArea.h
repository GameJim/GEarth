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
		bool waitFrameFinished(const uint32_t& microseconds = 10);  //�ȴ���Ⱦ����
		void pushMessage(std::unique_ptr<util::Message> message);
		bool Frame();          //��Ⱦ��Ϣ
		void NotyFrame();      //֪ͨ��Ⱦ��Ϣ
	protected:
		void receiveMessage(std::unique_ptr<util::Message> message);//���յ�����Ϣ���мӹ�
	protected:
		osg::ref_ptr<osgViewer::CompositeViewer> m_pCompositeViewer;
		std::mutex  m_mutex; //��Ⱦ��������ȫ

		//��Ⱦ���
		std::shared_ptr<util::Mailbox> m_pMailBox;
		std::shared_ptr<util::SequencedScheduler> m_pScheduler;

		std::atomic<bool>  m_bFramOnce;
		std::atomic<bool>  m_needFrame;
		
	};
}
