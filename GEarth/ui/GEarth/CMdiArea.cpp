#include "GEarth/CMdiArea.h"
#include "GEarth/CMdiSubWindow.h"
#include <osgViewer/ViewerEventHandlers>
#include <osgEarth/MapNode>


namespace ui
{
	CMdiArea::CMdiArea(QWidget * parent)
		: QMdiArea(parent)
		, m_pMailBox(new util::Mailbox())
		, m_pScheduler(new util::SequencedScheduler())
		, m_bFramOnce(false)
		, m_needFrame(true)
	{
		m_pMailBox->open(*m_pScheduler);

		m_pCompositeViewer = new osgViewer::CompositeViewer();
		m_pCompositeViewer->setThreadingModel(osgViewer::ViewerBase::CullThreadPerCameraDrawThreadPerContext);

		//������һ����Ⱦ
		this->pushMessage(util::actor::makeMessage(*this, &CMdiArea::NotyFrame));
	
	}

	CMdiArea::~CMdiArea() {
		//��ֹ��Ⱦ�߳�
		m_pMailBox->close();
		m_pMailBox = nullptr;
		m_pScheduler = nullptr;
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
				std::unique_lock<std::mutex> mutex(m_mutex);
				m_pCompositeViewer->addView(pNewWindow->getView());
				this->addSubWindow(pNewWindow);
				return;
			}
		}

		
		auto pNewWindow = new CMdiSubWindow(new osgEarth::MapNode(pMap.get()));
		std::unique_lock<std::mutex> mutex(m_mutex);
		m_pCompositeViewer->addView(pNewWindow->getView());
		this->addSubWindow(pNewWindow);

		m_pCompositeViewer->realize();
	}


	
	void CMdiArea::destoryMdiSubWindow(CMdiSubWindow* pMdiSubWindow)
	{
		std::unique_lock<std::mutex> mutex(m_mutex);
		m_pCompositeViewer->removeView(pMdiSubWindow->getView());
	}

	bool CMdiArea::Frame()
	{
		std::unique_lock<std::mutex> mutex(m_mutex);
		if (m_pCompositeViewer->getNumViews() > 0)
		{
			m_pCompositeViewer->frame();

			return true;
		}

		return false;
	}

	
	void CMdiArea::pushMessage(std::unique_ptr<util::Message> message)
	{
		m_pMailBox->push(util::actor::makeMessage(*this, &CMdiArea::receiveMessage,std::move(message)));
	}

	void CMdiArea::NotyFrame()
	{
		m_needFrame = true;
	}

	bool CMdiArea::waitFrameFinished(const uint32_t& microseconds)
	{
		std::promise<bool> frame;
		std::future<bool> result = frame.get_future();
		this->pushMessage(util::actor::makeMessage(std::move(frame),*this, &CMdiArea::Frame));
		if (microseconds == 0)
		{
			return result.get();
		}

		std::future_status status = result.wait_for(std::chrono::microseconds(microseconds));
		if (status == std::future_status::timeout)//���̻߳�ûִ����
		{
			return false;
		}
		else if (status == std::future_status::ready)//���߳���ִ����
		{
			return true;
		}

		return true;
	}

	void CMdiArea::receiveMessage(std::unique_ptr<util::Message> message)
	{
		//�������Ϣ
		(*message)();

		//����Ƿ�Ϊ���һ����Ϣ
		if (m_pMailBox->hasMessage()) return;

		if (!m_bFramOnce || m_needFrame)
		{
			this->pushMessage(util::actor::makeMessage(std::move(std::promise<bool>()),*this, &CMdiArea::Frame));
			m_needFrame.store(false);
		}
	}

}

