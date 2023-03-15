#include "GEarth/CBaseTitleBar.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QFile>
#include <QMouseEvent>
#include <QDebug>
#include "GEarth/style_sheets_cfg.h"

#define BUTTON_HEIGHT 30        // ��ť�߶�;
#define BUTTON_WIDTH 30         // ��ť���;
#define TITLE_HEIGHT 30         // �������߶�;
namespace ui
{
	CBaseTitleBar::CBaseTitleBar(QWidget *parent)
		: QWidget(parent)
		, m_isPressed(false)
		, m_buttonType(MIN_MAX_BUTTON)
	{
		// ��ʼ��;
		initControl();
		initConnections();

		this->setObjectName("CBaseTitleBar");

		auto& pStyleSheetsCfg = CStyleSheetsCfg::instance();
		this->setStyleSheet(pStyleSheetsCfg.getUIStyle("CBaseTitleBar"));
	}

	CBaseTitleBar::~CBaseTitleBar()
	{

	}

	// ��ʼ���ؼ�;
	void CBaseTitleBar::initControl()
	{
		m_pIcon = new QLabel;
		m_pTitleContent = new QLabel;

		m_pButtonMin = new QPushButton;
		m_pButtonRestore = new QPushButton;
		m_pButtonMax = new QPushButton;
		m_pButtonClose = new QPushButton;

		m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
		m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
		m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
		m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

		m_pTitleContent->setObjectName("TitleContent");
		m_pButtonMin->setObjectName("ButtonMin");
		m_pButtonRestore->setObjectName("ButtonRestore");
		m_pButtonMax->setObjectName("ButtonMax");
		m_pButtonClose->setObjectName("ButtonClose");

		QHBoxLayout* mylayout = new QHBoxLayout(this);
		mylayout->addWidget(m_pIcon);
		mylayout->addWidget(m_pTitleContent);

		mylayout->addWidget(m_pButtonMin);
		mylayout->addWidget(m_pButtonRestore);
		mylayout->addWidget(m_pButtonMax);
		mylayout->addWidget(m_pButtonClose);

		mylayout->setContentsMargins(5, 0, 0, 0);
		mylayout->setSpacing(0);

		m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		this->setFixedHeight(TITLE_HEIGHT);
		this->setWindowFlags(Qt::FramelessWindowHint);
	}

	// �źŲ۵İ�;
	void CBaseTitleBar::initConnections()
	{
		connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
		connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
		connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
		connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));
	}

	

	// ���ñ�����ͼ��;
	void CBaseTitleBar::setTitleIcon(QString filePath)
	{
		QPixmap titleIcon(filePath);
		m_pIcon->setPixmap(titleIcon.scaled(25, 25));
	}

	// ���ñ�������;
	void CBaseTitleBar::setTitleContent(QString titleContent)
	{
		m_pTitleContent->setText(titleContent);
		m_titleContent = titleContent;
	}

	// ���ñ���������;
	//void CBaseTitleBar::setTitleWidth(int width)
	//{
	//	this->setFixedWidth(width);
	//}

	// ���ñ������ϰ�ť����;
	// ���ڲ�ͬ���ڱ������ϵİ�ť����һ�������Կ����Զ���������еİ�ť;
	// �����ṩ���ĸ���ť���ֱ�Ϊ��С������ԭ����󻯡��رհ�ť�������Ҫ������ť�������������;
	void CBaseTitleBar::setButtonType(ButtonType buttonType)
	{
		m_buttonType = buttonType;

		switch (buttonType)
		{
		case MIN_BUTTON:
		{
			m_pButtonRestore->setVisible(false);
			m_pButtonMax->setVisible(false);
		}
		break;
		case MIN_MAX_BUTTON:
		{
			m_pButtonRestore->setVisible(false);
		}
		break;
		case ONLY_CLOSE_BUTTON:
		{
			m_pButtonMin->setVisible(false);
			m_pButtonRestore->setVisible(false);
			m_pButtonMax->setVisible(false);
		}
		break;
		default:
			break;
		}
	}

	// ���ñ������еı����Ƿ���Զ�����������Ƶ�Ч��;
	// һ������±������еı��������ǲ������ģ����Ǽ�Ȼ�Զ���Ϳ��Լ���Ҫ�����ô��ƾ���ô��O(��_��)O��
	void CBaseTitleBar::setTitleRoll()
	{
		connect(&m_titleRollTimer, SIGNAL(timeout()), this, SLOT(onRollTitle()));
		m_titleRollTimer.start(200);
	}

	// ���洰�����ǰ���ڵ�λ���Լ���С;
	void CBaseTitleBar::saveRestoreInfo(const QPoint point, const QSize size)
	{
		m_restorePos = point;
		m_restoreSize = size;
	}

	// ��ȡ�������ǰ���ڵ�λ���Լ���С;
	void CBaseTitleBar::getRestoreInfo(QPoint& point, QSize& size)
	{
		point = m_restorePos;
		size = m_restoreSize;
	}

	// ���Ʊ���������ɫ;
	void CBaseTitleBar::paintEvent(QPaintEvent *event)
	{
		//���ñ���ɫ;
		QPainter painter(this);
		QPainterPath pathBack;
		pathBack.setFillRule(Qt::WindingFill);
		pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
		painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
		painter.fillPath(pathBack, QBrush(painter.background().color()));

		// ��������󻯻��߻�ԭ�󣬴��ڳ��ȱ��ˣ��������ĳ���Ӧ��һ��ı�;
		/*if (this->width() != this->parentWidget()->width())
		{
			this->setFixedWidth(this->parentWidget()->width());
		}*/
		QWidget::paintEvent(event);
	}

	// ˫����Ӧ�¼�����Ҫ��ʵ��˫��������������󻯺���С������;
	void CBaseTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
	{
		// ֻ�д�����󻯡���ԭ��ťʱ˫������Ч;
		if (m_buttonType == MIN_MAX_BUTTON)
		{
			// ͨ����󻯰�ť��״̬�жϵ�ǰ�����Ǵ�����󻯻���ԭʼ��С״̬;
			// ����ͨ���������ñ�������ʾ��ǰ����״̬;
			if (m_pButtonMax->isVisible())
			{
				onButtonMaxClicked();
			}
			else
			{
				onButtonRestoreClicked();
			}
		}

		return QWidget::mouseDoubleClickEvent(event);
	}

	// ����ͨ��mousePressEvent��mouseMoveEvent��mouseReleaseEvent�����¼�ʵ��������϶��������ƶ����ڵ�Ч��;
	void CBaseTitleBar::mousePressEvent(QMouseEvent *event)
	{
		if (m_buttonType == MIN_MAX_BUTTON)
		{
			// �ڴ������ʱ��ֹ�϶�����;
			if (m_pButtonMax->isVisible())
			{
				m_isPressed = true;
				m_startMovePos = event->globalPos();
			}
		}
		else
		{
			m_isPressed = true;
			m_startMovePos = event->globalPos();
		}

		return QWidget::mousePressEvent(event);
	}

	void CBaseTitleBar::mouseMoveEvent(QMouseEvent *event)
	{
		if (m_isPressed)
		{
			QPoint movePoint = event->globalPos() - m_startMovePos;
			QPoint widgetPos = this->parentWidget()->pos();
			m_startMovePos = event->globalPos();
			this->parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
		}
		return QWidget::mouseMoveEvent(event);
	}

	void CBaseTitleBar::mouseReleaseEvent(QMouseEvent *event)
	{
		m_isPressed = false;
		return QWidget::mouseReleaseEvent(event);
	}

	
	// ����Ϊ��ť������Ӧ�Ĳ�;
	void CBaseTitleBar::onButtonMinClicked()
	{
		emit signalButtonMinClicked();
	}

	void CBaseTitleBar::onButtonRestoreClicked()
	{
		m_pButtonRestore->setVisible(false);
		m_pButtonMax->setVisible(true);
		emit signalButtonRestoreClicked();
	}

	void CBaseTitleBar::onButtonMaxClicked()
	{
		m_pButtonMax->setVisible(false);
		m_pButtonRestore->setVisible(true);
		emit signalButtonMaxClicked();
	}

	void CBaseTitleBar::onButtonCloseClicked()
	{
		emit signalButtonCloseClicked();
	}

	// �÷�����Ҫ���ñ������еı�����ʾΪ������Ч��;
	void CBaseTitleBar::onRollTitle()
	{
		static int nPos = 0;
		QString titleContent = m_titleContent;
		// ����ȡ��λ�ñ��ַ�����ʱ����ͷ��ʼ;
		if (nPos > titleContent.length())
			nPos = 0;

		m_pTitleContent->setText(titleContent.mid(nPos));
		nPos++;
	}
}


