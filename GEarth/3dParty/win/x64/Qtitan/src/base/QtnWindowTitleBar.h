/****************************************************************************
**
** Qtitan Library by Developer Machines (Components for Qt.C++)
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


#ifndef QTN_WINDOWTITLEBAR_H
#define QTN_WINDOWTITLEBAR_H

#include <QWidget>
#include "QtitanDef.h"

QTITAN_BEGIN_NAMESPACE

class WindowTitleBarPrivate;

class QTITAN_EXPORT WindowTitleBar : public QObject
{
    Q_OBJECT
protected:
    WindowTitleBar(QWidget* window);
public:
    static WindowTitleBar* get(QWidget* window);
    virtual ~WindowTitleBar();
public:
    enum TitleBarAlign
    {
        TitleBarAlignLeft,
        TitleBarAlignRight,
        TitleBarAlignClient,
    };
public:
    void show();
    void hide();
    void setWidget(QWidget* widget, TitleBarAlign align = TitleBarAlignLeft);
    QWidget* widget() const;
    bool styledFrame() const;
    void setStyledFrame(bool styled);
    bool blurWindowEnabled() const;
    void setBlurWindowEnabled(bool enabled);
    bool extendViewIntoTitleBar() const;
    void setExtendViewIntoTitleBar(bool extend);
    void setTitleHeight(int height);
    int titleHeight() const;
    void setBorderThickness(int thickness);
    int borderThickness() const;
    bool isSysMenuButtonVisible() const;
    void setSysMenuButtonVisible(bool visible);
    const QColor& backgroundColor() const;
    void setBackgroundColor(const QColor& color);
    void setDestroyed();
protected slots:
    void appAboutToQuit();
private:
    WindowTitleBarPrivate* m_p;
};

QTITAN_END_NAMESPACE

#endif //QTN_WINDOWTITLEBAR_H


