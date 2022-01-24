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
#ifndef QTN_WINDOWTITLEBAR_WIN_H
#define QTN_WINDOWTITLEBAR_WIN_H


#include "QtnPlatform.h"
#include "QtnWindowTitleBarPrivate.h"

#ifdef QTN_PRIVATE_HEADERS
#include <qpa/qplatformwindow.h>
#include <qpa/qplatformnativeinterface.h>
#endif

QTITAN_BEGIN_NAMESPACE

class WindowsStyleMouseHandlerWin;

/* WindowTitleBarPrivateWin */
class WindowTitleBarPrivateWin : public WindowTitleBarPrivate,
                             public WindowsHook
{
public:
    WindowTitleBarPrivateWin();
    virtual ~WindowTitleBarPrivateWin();
public:
    virtual void setWindow(QWidget* window);
    virtual void update(const QRect& rect);
    virtual void update(const QRegion& region);
    virtual bool isActive() const;
    virtual void showSystemMenu(const QPoint& pos);
public:
    bool isAeroGlassEnabled() const;
    void setAeroGlassEnabled(bool enabled);
    void setTransitionsEffectEnabled(bool enabled);
protected:
    bool isCompositionAvailable() const;
    virtual LRESULT winProc(HWND hWnd, uint nMessage, WPARAM wParam, LPARAM lParam);
    virtual void winEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
    void initializeWindowUI();
    void finalizeWindowUI();
    void forceNonClientSizeUpdate();
    bool updateDwmSettings(bool activated);
    void updateWindowSize();
    virtual void styleChanged();
    virtual void updateSysButtonsRect();
    virtual QStyle::SubControl getHoverNativeSubControl(const QPoint& pos);

    bool handleMessage(MSG* message, LRESULT& result);
    bool handleNonClientCreate();
    bool handleNonClientDestroy();
    bool handleNonClientPaint(HRGN updateRegion);
    bool handleNonClientCalcSize(NCCALCSIZE_PARAMS* sizeParams, bool calcRects);
    bool handlePaint(); 
    bool handleNonClientMouseEvent(MSG* message, LRESULT &outResult);
    int handleNonClientHitTest(LPPOINT ptHit);

    void setRedraw(bool redraw);
    bool isIconic() const;
    void enforceMenuBackground();
    void paintNonClientArea(HDC hdc);
protected:
    virtual bool eventFilter(QObject* object, QEvent* event);
    virtual void mouseContextNonClientEvent(QContextMenuEvent* event);
    virtual void resizeNonClientEvent(const QSize& windowSize);
private:
    bool mouseEventFilter(QObject* object, QEvent* event);
    void installWindow(QWidget* window);
    void resetWindow();
    void fixupQtInternals(bool forceUpdate = true);
    void setTrackMouseEnabled(bool enabled);
    bool hasMaximizedStyle() const;
    bool hasFullScreenStyle() const;
    bool hasSysMenuStyle() const;
    void setSysMenuStyle(bool enabled);
    QMargins extraFrameMargins() const;
    QPoint mapFromGlobal(const QPoint& pos) const;
    QPoint mapToGlobal(const QPoint& pos) const;
    QMargins calcSystemFrameMargins() const;
    QMargins calcWidndows10FrameMargins() const;
    void checkCaptureAfterPress();
private:
    bool m_isActive;
    bool m_isFinalized;
    bool m_aeroGlassEnabled;
    bool m_lockStyleChange;
    Qt::MouseButtons m_lastMouseButtons;
    ImageHDC* m_ncpaintptr;
    bool m_lastMouseEventAccepted;
    bool m_ignoreMouseMoveEvent;
    LRESULT m_hitTest;
};

QTITAN_END_NAMESPACE

#endif //QTN_WINDOWTITLEBAR_WIN_H

