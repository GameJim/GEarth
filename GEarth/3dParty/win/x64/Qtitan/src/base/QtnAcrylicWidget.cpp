/****************************************************************************
**
** Qtitan Library by Developer Machines (Navigation UI for Qt.C++)
**
** Copyright (c) 2009-2017 Developer Machines (http://www.devmachines.com)
**           ALL RIGHTS RESERVED
**
**  The entire contents of this file is protected by copyright law and
**  international treaties. Unauthorized reproduction, reverse-engineering
**  and distribution of all or any portion of the code contained in this
**  file is strictly prohibited and may result in severe civil and
**  criminal penalties and will be prosecuted to the maximum extent
**  possible under the law.
**
**  RESTRICTIONS
**
**  THE SOURCE CODE CONTAINED WITHIN THIS FILE AND ALL RELATED
**  FILES OR ANY PORTION OF ITS CONTENTS SHALL AT NO TIME BE
**  COPIED, TRANSFERRED, SOLD, DISTRIBUTED, OR OTHERWISE MADE
**  AVAILABLE TO OTHER INDIVIDUALS WITHOUT WRITTEN CONSENT
**  AND PERMISSION FROM DEVELOPER MACHINES
**
**  CONSULT THE END USER LICENSE AGREEMENT FOR INFORMATION ON
**  ADDITIONAL RESTRICTIONS.
**
****************************************************************************/
#include <QPainter>
#include <QPaintEvent>
#include <QPaintEngine>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QBackingStore>
#endif

#include "QtnPlatform.h"
#include "QtnAcrylicWidget.h"
#include "QtnWindowTitleBarPrivate.h"

QTITAN_USE_NAMESPACE

/* AcrylicWidget */
AcrylicWidget::AcrylicWidget(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f), m_acrylicBlend(Disabled), m_tintOpacity(0.7),
    m_buffer(0, 0, QImage::Format_ARGB32_Premultiplied)
{
    qtn_initializeDrawingResources();
    AcrylicWidgetEffect* eff = new AcrylicWidgetEffect(this);
    setGraphicsEffect(eff);

    setAutoFillBackground(false);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAcrylicBlend(BackgroundAcrylic);
    m_acrylicBrush = createAcrylicBrush();
}

AcrylicWidget::~AcrylicWidget()
{
}

AcrylicWidget::AcrylicBlend AcrylicWidget::acrylicBlend() const
{
    return m_acrylicBlend;
}

void AcrylicWidget::setAcrylicBlend(AcrylicBlend acrylicBlend)
{
    if (m_acrylicBlend == acrylicBlend)
        return;
    m_acrylicBlend = acrylicBlend;

    setAttribute(Qt::WA_OpaquePaintEvent, m_acrylicBlend == BackgroundAcrylic);
    if (graphicsEffect() != Q_NULL)
        graphicsEffect()->setEnabled(m_acrylicBlend == InAppAcrylic);
    update();
}

const QColor& AcrylicWidget::tintColor() const
{
    return palette().color(backgroundRole());
}

void AcrylicWidget::setTintColor(const QColor& color)
{
    if (tintColor() == color)
        return;
    QPalette p = palette();
    p.setColor(backgroundRole(), color);
    setPalette(p);
}

double AcrylicWidget::tintOpacity() const
{
    return m_tintOpacity;
}

void AcrylicWidget::setTintOpacity(double opacity)
{
    if (m_tintOpacity == opacity)
        return;
    m_tintOpacity = opacity;
    m_acrylicBrush = createAcrylicBrush();
}

QBrush AcrylicWidget::createAcrylicBrush() const
{
    static QImage noiseTexture = QImage(":/res/acrylic_noise.png");
    QImage acrylicTexture = QImage(QSize(64, 64), QImage::Format_ARGB32_Premultiplied);
    acrylicTexture.fill(Qt::transparent);
    {
        QPainter p(&acrylicTexture);
        p.setOpacity(tintOpacity());
        p.fillRect(QRect(0, 0, acrylicTexture.width(), acrylicTexture.height()), tintColor());
        p.setOpacity(0.04);
        p.fillRect(QRect(0, 0, acrylicTexture.width(), acrylicTexture.height()), QBrush(noiseTexture));
    }
    return QBrush(acrylicTexture);
}

const QImage& AcrylicWidget::grabBackstageToBuffer(const QRegion& clip, bool& hasChanges)
{
    const QImage::Format format = QImage::Format_ARGB32_Premultiplied;
    hasChanges = false;

    if (m_buffer.size() != size())
        m_buffer = QImage(width(), height(), format);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QImage* windowBackstage = backingStore()->paintDevice()->devType() == QInternal::Image ?
        static_cast<QImage *>(backingStore()->paintDevice()) : Q_NULL;
#else
    QImage* windowBackstage = Q_NULL;
#endif
    if (windowBackstage != Q_NULL)
    {
        QPoint offset = mapTo(window(), QPoint(0, 0));
        QVector<QRect> rects = clip.rects();

        for (int i = 0; i < rects.size(); ++i)
        {
            QRect r = rects[i];
            QImage part = windowBackstage->copy(r.translated(offset));
            if (part.format() != format)
                part = part.convertToFormat(format);
            if (qtn_image_copy(m_buffer, r.topLeft(), part))
                hasChanges = true;
        }
    }
    return m_buffer;
}

void AcrylicWidget::setBackstageToWindow(const QImage& newBackstage, const QRegion& clip)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QImage* windowBackstage = backingStore()->paintDevice()->devType() == QInternal::Image ?
        static_cast<QImage *>(backingStore()->paintDevice()) : Q_NULL;
#else
    QImage* windowBackstage = Q_NULL;
#endif
    if (windowBackstage != Q_NULL)
    {
        const QImage::Format format = windowBackstage->format();
        QPoint offset = mapTo(window(), QPoint(0, 0));
        if (clip.boundingRect() == rect())
        {
            if (newBackstage.format() != format)
                qtn_image_copy(*windowBackstage, offset, newBackstage.convertToFormat(format));
            else
                qtn_image_copy(*windowBackstage, offset, newBackstage);
        }
        else
        {
            //Optimiztion....
            QVector<QRect> rects = clip.rects();
            for (int i = 0; i < rects.size(); ++i)
            {
                QRect r = rects[i];
                QImage part = newBackstage.copy(r);
                if (part.format() != format)
                    part = part.convertToFormat(format);
                qtn_image_copy(*windowBackstage, offset + r.topLeft(), part);
            }
        }
    }
}

void AcrylicWidget::paintBackground(QPainter* painter, const QRegion& clip)
{
    const QRect rect = QRect(0, 0, width(), height());
    QRegion region = clip.isEmpty() ? rect : clip;

    painter->setClipRegion(region);

    if (m_acrylicBlend == AcrylicWidget::BackgroundAcrylic)
    {
        QPainter::CompositionMode mode = painter->compositionMode();
        painter->setCompositionMode(QPainter::CompositionMode_Clear);
        painter->fillRect(rect, Qt::white);
        painter->setCompositionMode(mode);
    }
    else if (m_acrylicBlend == AcrylicWidget::InAppAcrylic)
    {
        //Do nothing
    }
    else
    {
        Q_ASSERT(m_acrylicBlend == AcrylicWidget::Disabled);
    }
#if 1
    painter->save();

    painter->setCompositionMode(QPainter::CompositionMode_SoftLight);
    painter->setOpacity(0.3);
    painter->fillRect(QRect(0, 0, rect.width(), rect.height()), Qt::white);

    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter->setOpacity(1);
    painter->fillRect(rect, m_acrylicBrush);

    painter->restore();
#endif
}

/*! \reimp */
void AcrylicWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    paintBackground(&painter, event->region());
#if 0
    QRect rect = QRect(0, 0, width(), height());
    QPen pen(Qt::darkRed);
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawRect(rect.adjusted(0, 0, -1, -1));
#endif
}

/*! \reimp */
void AcrylicWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
}

/*! \reimp */
void AcrylicWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
}

/*! \reimp */
void AcrylicWidget::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
}

/*! \reimp */
void AcrylicWidget::changeEvent(QEvent * event)
{
    if (event->type() == QEvent::PaletteChange)
        m_acrylicBrush = createAcrylicBrush();

    QWidget::changeEvent(event);
}


/* AcrylicWidgetEffect */
AcrylicWidgetEffect::AcrylicWidgetEffect(AcrylicWidget* widget)
    : QGraphicsEffect(), m_widget(widget)
{
}

AcrylicWidgetEffect::~AcrylicWidgetEffect()
{
    m_widget = Q_NULL;
}

QRectF AcrylicWidgetEffect::boundingRectFor(const QRectF &sourceRect) const
{
    return sourceRect;
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
static QRect qtn_marginsRemoved(const QRect& rc, const QMargins& margins)
{
    return QRect(QPoint(rc.left() + margins.left(), rc.top() + margins.top()),
        QPoint(rc.right() - margins.right(), rc.bottom() - margins.bottom()));
}
#endif

void AcrylicWidgetEffect::draw(QPainter *painter)
{
    Q_UNUSED(painter);
    QRegion clip = painter->paintEngine()->systemClip();
    QPoint offset = m_widget->mapTo(m_widget->window(), QPoint());
    clip = clip.translated(-offset).intersected(m_widget->rect());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QRect windowRect = m_widget->window()->rect().marginsRemoved(m_widget->window()->contentsMargins());
#else
    QRect windowRect = qtn_marginsRemoved(m_widget->window()->rect(), m_widget->window()->contentsMargins());
#endif
    QRect widgetRect = windowRect.translated(-offset).intersected(m_widget->rect());

    bool fullyUpdate = false;
    QImage buffer = m_widget->grabBackstageToBuffer(clip, fullyUpdate);
    if (fullyUpdate)
    {
        qt_blurImage(buffer, 128, false, 0);
        m_blurredBuffer = buffer;
    }
    else
    {
        buffer = m_blurredBuffer;
    }

    QRegion dirty = fullyUpdate ? widgetRect : clip;
    {
        QPainter p(&buffer);
        m_widget->render(&p, dirty.boundingRect().topLeft(), dirty.boundingRect());
    }

    m_widget->setBackstageToWindow(buffer, dirty);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    if (fullyUpdate && clip != m_widget->rect())
    {
        QRegion forcedUpdate = QRegion(widgetRect) - clip;
        m_widget->backingStore()->flush(forcedUpdate.translated(offset), m_widget->windowHandle(), QPoint(0, 0));
    }
#endif
}

void AcrylicWidgetEffect::sourceChanged(ChangeFlags flags)
{
    QGraphicsEffect::sourceChanged(flags);
}
