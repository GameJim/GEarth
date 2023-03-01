#include "GEarth/CMdiArea.h"
#include "GEarth/CMdiSubWindow.h"
#include <osgViewer/ViewerEventHandlers>

#include <osgEarth/MapNode>
namespace ui
{
	CMdiArea::CMdiArea(QWidget * parent)
		: QMdiArea(parent)
	{
		m_bClosed = false;
		m_needNotiyFrameFinished = false;
		m_pCompositeViewer = new osgViewer::CompositeViewer();
		m_pCompositeViewer->setThreadingModel(osgViewer::ViewerBase::CullThreadPerCameraDrawThreadPerContext);


		m_renderThread = std::thread([&]() {
			while (!m_bClosed)
			{
				this->Frame();
			}
		});

		m_renderThread.detach();
	}

	CMdiArea::~CMdiArea() {
		m_bClosed.store(true);
		m_renderThread.join();
	}

	void CMdiArea::createMdiSubWindow(osg::ref_ptr<osgEarth::Map> pMap)
	{
		//����Ѵ��ڴ����Ƿ�������pMap�ڵ㣬������ڸ�����MapNode
		auto subWindowList = this->subWindowList();
		for (auto subWindow : subWindowList)
		{
			auto pWindow = static_cast<CMdiSubWindow*>(subWindow);
			if (pWindow->getMapNode()->getMap() == pMap)
			{
				auto pNewWindow = new CMdiSubWindow(pWindow->getMapNode());
				m_pCompositeViewer->addView(pNewWindow->getView());
				this->addSubWindow(pNewWindow);
				return;
			}
		}

		
		auto pNewWindow = new CMdiSubWindow(new osgEarth::MapNode(pMap.get()));
		m_pCompositeViewer->addView(pNewWindow->getView());
		this->addSubWindow(pNewWindow);

		
		m_pCompositeViewer->realize();
	}

	void CMdiArea::immediatelDraw()
	{
		m_framed = std::promise<bool>();
		std::future<bool> frame_future = m_framed.get_future();
		//������Ϣ������Ⱦ��,����������Ⱦһ��
		m_needNotiyFrameFinished.store(true);
		//�ȴ���Ⱦ���
		frame_future.wait_for(std::chrono::microseconds(10));
	}

	
	void CMdiArea::Frame()
	{
		bool needNotiyFrameFinished = m_needNotiyFrameFinished.load();
		

		if (m_pCompositeViewer->getNumViews() > 0)
		{
			m_pCompositeViewer->frame();
		}

		if (needNotiyFrameFinished == true && m_needNotiyFrameFinished.load() == true)
		{
			m_framed.set_value(true);
			m_needNotiyFrameFinished.store(false);
		}
		
	}

}

