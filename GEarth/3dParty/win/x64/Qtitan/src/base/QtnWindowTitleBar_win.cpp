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

#include <QThread>
#include <qevent.h>
#include <QApplication>
#include <QDesktopWidget>

#include "QtnWindowTitleBar_win.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
Q_DECLARE_METATYPE(QMargins)
#endif

QTITAN_USE_NAMESPACE

/*

bool WindowsStyleMouseHandlerWin::convertMouseWheelEvent(const MSG& msg)
{
    const Qt::KeyboardModifiers mods = stateKeyboardModifiers();

    int delta;
    if (msg.message == WM_MOUSEWHEEL || msg.message == WM_MOUSEHWHEEL)
        delta = GET_WHEEL_DELTA_WPARAM(msg.wParam);
    else
        delta = int(msg.wParam);

    Qt::Orientation orientation = (msg.message == WM_MOUSEHWHEEL
        || (mods & Qt::AltModifier)) ?
        Qt::Horizontal : Qt::Vertical;

    if (msg.message == WM_MOUSEHWHEEL)
        delta = -delta;
    const QPoint globalPos(GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam));

    return true;

    return false;
}
*/

static Qt::MouseButton qtn_get_button_from_message(UINT messageId)
{
    const bool mouseSwapped = GetSystemMetrics(SM_SWAPBUTTON);
    Qt::MouseButton result = Qt::NoButton;
    switch (messageId)
    {
        case WM_NCLBUTTONDOWN:
        case WM_NCLBUTTONUP:
        case WM_NCLBUTTONDBLCLK:
        case WM_LBUTTONUP:
            result = mouseSwapped ? Qt::RightButton : Qt::LeftButton;
            break;
        case WM_NCRBUTTONDOWN:
        case WM_NCRBUTTONUP:
        case WM_NCRBUTTONDBLCLK:
        case WM_RBUTTONUP:
            result = mouseSwapped ? Qt::LeftButton : Qt::RightButton;
            break;
        case WM_NCMBUTTONDOWN:
        case WM_NCMBUTTONUP:
        case WM_NCMBUTTONDBLCLK:
        case WM_MBUTTONUP:
            result = Qt::MidButton;
            break;
        default:
            break;
    }
    return result;
}

static Qt::MouseButtons qtn_state_mouse_buttons()
{
    Qt::MouseButtons result = 0;
    const bool mouseSwapped = GetSystemMetrics(SM_SWAPBUTTON);
    if (GetAsyncKeyState(VK_LBUTTON) < 0)
        result |= mouseSwapped ? Qt::RightButton : Qt::LeftButton;
    if (GetAsyncKeyState(VK_RBUTTON) < 0)
        result |= mouseSwapped ? Qt::LeftButton : Qt::RightButton;
    if (GetAsyncKeyState(VK_MBUTTON) < 0)
        result |= Qt::MidButton;
    if (GetAsyncKeyState(VK_XBUTTON1) < 0)
        result |= Qt::XButton1;
    if (GetAsyncKeyState(VK_XBUTTON2) < 0)
        result |= Qt::XButton2;
    return result;
}

static Qt::KeyboardModifiers qtn_state_keyboard_modifiers()
{
    Qt::KeyboardModifiers modifiers = Qt::NoModifier;
    if (GetKeyState(VK_SHIFT) < 0)
        modifiers |= Qt::ShiftModifier;
    if (GetKeyState(VK_CONTROL) < 0)
        modifiers |= Qt::ControlModifier;
    if (GetKeyState(VK_MENU) < 0)
        modifiers |= Qt::AltModifier;
    if (GetKeyState(VK_LWIN) < 0 || GetKeyState(VK_RWIN) < 0)
        modifiers |= Qt::MetaModifier;
    return modifiers;
}

static UINT get_clinet_message_down(UINT message)
{
    if (message == WM_NCLBUTTONDOWN)
        return WM_LBUTTONDOWN;
    else if (message == WM_NCRBUTTONDOWN)
        return WM_RBUTTONDOWN;
    else if (message == WM_NCMBUTTONDOWN)
        return WM_MBUTTONDOWN;
    else if (message == WM_NCLBUTTONDBLCLK)
        return WM_LBUTTONDBLCLK;
    else if (message == WM_NCRBUTTONDBLCLK)
        return WM_RBUTTONDBLCLK;
    else if (message == WM_NCMBUTTONDBLCLK)
        return WM_MBUTTONDBLCLK;
    Q_ASSERT(false);
    return 0;
}

static WPARAM get_clinet_message_down_wparam(UINT message)
{
    WPARAM result = 0;
    if (message == WM_NCLBUTTONDOWN)
        result = MK_LBUTTON;
    else if (message == WM_NCRBUTTONDOWN)
        result = MK_RBUTTON;
    else if (message == WM_NCMBUTTONDOWN)
        result = MK_MBUTTON;
    else if (message == WM_NCLBUTTONDBLCLK)
        result = MK_LBUTTON;
    else if (message == WM_NCRBUTTONDBLCLK)
        result = MK_RBUTTON;
    else if (message == WM_NCMBUTTONDBLCLK)
        result = MK_MBUTTON;
    Q_ASSERT(result != 0);
    return result;
}

static UINT get_clinet_message_up(UINT message)
{
    if (message == WM_NCLBUTTONDOWN)
        return WM_LBUTTONUP;
    else if (message == WM_NCRBUTTONDOWN)
        return WM_RBUTTONUP;
    else if (message == WM_NCMBUTTONDOWN)
        return WM_MBUTTONUP;
    else if (message == WM_NCLBUTTONDBLCLK)
        return WM_LBUTTONUP;
    else if (message == WM_NCRBUTTONDBLCLK)
        return WM_RBUTTONUP;
    else if (message == WM_NCMBUTTONDBLCLK)
        return WM_MBUTTONUP;
    else if (message == WM_NCLBUTTONUP || message == WM_LBUTTONUP)
        return WM_LBUTTONUP;
    else if (message == WM_NCRBUTTONUP || message == WM_RBUTTONUP)
        return WM_RBUTTONUP;
    else if (message == WM_NCMBUTTONUP || message == WM_MBUTTONUP)
        return WM_MBUTTONUP;
    Q_ASSERT(false);
    return 0;
}

static WPARAM get_clinet_mouse_wparam(const Qt::MouseButtons& buttons, Qt::KeyboardModifiers& modifiers)
{
    WPARAM result = 0;
    if (buttons & Qt::LeftButton)
        result |= MK_LBUTTON;
    if (buttons & Qt::RightButton)
        result |= MK_RBUTTON;
    if (buttons & Qt::MiddleButton)
        result |= MK_MBUTTON;
    if (buttons & Qt::XButton1)
        result |= MK_XBUTTON1;
    if (buttons & Qt::XButton2)
        result |= MK_XBUTTON2;
    if (modifiers & Qt::ShiftModifier)
        result |= MK_SHIFT;
    if (buttons & Qt::ControlModifier)
        result |= MK_CONTROL;
    return result;
}

/* WindowTitleBarPrivateWin */
WindowTitleBarPrivateWin::WindowTitleBarPrivateWin()
    : 
    m_isActive(true), m_isFinalized(true)
    , m_aeroGlassEnabled(false), m_lockStyleChange(false),
    m_lastMouseButtons(Qt::NoButton), m_ncpaintptr(Q_NULL),
    m_lastMouseEventAccepted(false),
    m_ignoreMouseMoveEvent(false)
{
    qtn_initializeDrawingResources();
    m_aeroGlassEnabled = isCompositionAvailable();
}

WindowTitleBarPrivateWin::~WindowTitleBarPrivateWin()
{
    setWindow(Q_NULL);
    Q_DELETE_AND_NULL(m_ncpaintptr);
}

void WindowTitleBarPrivateWin::setWindow(QWidget* window)
{
    resetWindow();
    installWindow(window);
}

void WindowTitleBarPrivateWin::update(const QRect& rect)
{
    if (extendViewIntoTitleBar())
        WindowTitleBarPrivate::update(rect);
    else
    {
        //Update non-client area...
        QRect qr = rect.translated(QPoint(-frameMargins().left(), -frameMargins().top()));
        RECT r;
        SetRect(&r, qr.left(), qr.top(), qr.right() + 1, qr.bottom() + 1);
        RedrawWindow(winId(), &r, NULL, RDW_FRAME | RDW_INVALIDATE);
    }
}

void WindowTitleBarPrivateWin::update(const QRegion& region)
{
    if (extendViewIntoTitleBar())
        WindowTitleBarPrivate::update(region);
    else
    {
        //Update non-client area...
        HRGN hrgn = qtn_create_region(region.translated(QPoint(-frameMargins().left(), -frameMargins().top())));
        RedrawWindow(winId(), NULL, hrgn, RDW_FRAME | RDW_INVALIDATE);
        DeleteObject(hrgn);
    }
}

bool WindowTitleBarPrivateWin::isActive() const
{
    return m_isActive;
}

LRESULT WindowTitleBarPrivateWin::winProc(HWND hWnd, uint nMessage, WPARAM wParam, LPARAM lParam)
{
    Q_ASSERT(hWnd == winId());
    MSG curMsg;
    curMsg.hwnd = hWnd;
    curMsg.message = nMessage;
    curMsg.wParam = wParam;
    curMsg.lParam = lParam;
    LRESULT lResult = 0;
    if (handleMessage(&curMsg, lResult))
        return lResult;
    return WindowsHook::winProc(hWnd, nMessage, wParam, lParam);
}

void WindowTitleBarPrivateWin::winEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
    WindowsHook::winEventProc(hWinEventHook, event, hWnd, idObject, idChild, dwEventThread, dwmsEventTime);
}

bool WindowTitleBarPrivateWin::handleNonClientCreate()
{
    initializeWindowUI();
    return false; 
}

bool WindowTitleBarPrivateWin::handleNonClientDestroy()
{
    return false; 
}

bool WindowTitleBarPrivateWin::handleNonClientPaint(HRGN updateRegion)
{
    if (frameMargins().isNull())
        return true;

    DWORD flags = DCX_WINDOW | DCX_USESTYLE;
    HDC hdc = updateRegion == (HRGN)1 ? GetDCEx(winId(), 0, flags) : GetDCEx(winId(), updateRegion, flags | DCX_INTERSECTRGN);
    Q_ASSERT(hdc != 0);
    paintNonClientArea(hdc);
    ReleaseDC(winId(), hdc);
    return true;
}

bool WindowTitleBarPrivateWin::updateDwmSettings(bool activated)
{
    if (pDwmExtendFrameIntoClientArea == Q_NULL || pDwmSetWindowAttribute == Q_NULL)
        return false;

    //Negative values extends the aero glass to all non-client area.
    QMargins margins = activated ? QMargins(-1, -1, -1, -1) : QMargins();
    HLP_MARGINS hlpMargins;
    hlpMargins.cxLeftWidth = margins.left();
    hlpMargins.cyTopHeight = margins.top();
    hlpMargins.cxRightWidth = margins.right();
    hlpMargins.cyBottomHeight = margins.bottom();
    HRESULT res = pDwmExtendFrameIntoClientArea(winId(), &hlpMargins);

    BOOL value = activated;
    res = pDwmSetWindowAttribute(winId(), DWMWA_ALLOW_NCPAINT, &value, sizeof(BOOL));
#if 0 
    DWMNCRENDERINGPOLICY policy = (!activated || isAeroGlassEnabled()) ? DWMNCRP_USEWINDOWSTYLE : DWMNCRP_DISABLED;
    res = pDwmSetWindowAttribute(winId(), DWMWA_NCRENDERING_POLICY, &policy, sizeof(DWMNCRENDERINGPOLICY));
#endif
    Q_UNUSED(res);
    return true;
}

void WindowTitleBarPrivateWin::updateSysButtonsRect()
{
    if (styledFrame())
    {
        WindowTitleBarPrivate::updateSysButtonsRect();
    }
    else
    {
        RECT bounds;
        if (pDwmGetWindowAttribute != Q_NULL)
        {
            pDwmGetWindowAttribute(winId(), DWMWA_CAPTION_BUTTON_BOUNDS, &bounds, sizeof(bounds));
            m_systemButtonsRect = QRect(bounds.left, bounds.top, RectWidth(bounds), RectHeight(bounds));
        }
    }
}

void WindowTitleBarPrivateWin::updateWindowSize()
{
    RECT rect;
    GetWindowRect(winId(), &rect);
    QSize newSize = QSize(RectWidth(rect), RectHeight(rect));
    QMargins extraMargins = extraFrameMargins();
    newSize -= QSize(extraMargins.left() + extraMargins.right(), extraMargins.top() + extraMargins.bottom());
    if (windowSize() != newSize)
    {
        resizeNonClientEvent(newSize);
        update(getNonClientRegion());
    }
}

QMargins WindowTitleBarPrivateWin::calcSystemFrameMargins() const
{
    RECT rect = { 0,0,0,0 };
    DWORD style = WS_CAPTION | WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
    DWORD exStyle = WS_EX_WINDOWEDGE;

    if (!AdjustWindowRectEx(&rect, style, FALSE, exStyle)) { Q_ASSERT(false); }

    return QMargins(qAbs(rect.left), 
        qAbs(rect.top),
        qAbs(rect.right),
        qAbs(rect.bottom));
}

QMargins WindowTitleBarPrivateWin::calcWidndows10FrameMargins() const
{
    QMargins margins(0, 0, 0, 0);

    if (pDwmGetWindowAttribute == Q_NULL)
        return margins;

    RECT rect = { 0,0,0,0 };
    RECT frame = { 0,0,0,0 };
    GetWindowRect(winId(), &rect);
    pDwmGetWindowAttribute(winId(), DWMWA_EXTENDED_FRAME_BOUNDS, &frame, sizeof(frame));
    margins.setLeft(frame.left - rect.left);
    margins.setTop(frame.top - rect.top);
    margins.setRight(rect.right - frame.right);
    margins.setBottom(rect.bottom - frame.bottom);

    if (qtn_isWindows10())
        margins = QMargins(7, 0, 7, 7);
    return margins;
}

QStyle::SubControl WindowTitleBarPrivateWin::getHoverNativeSubControl(const QPoint& pos)
{
    LRESULT result = 0;
    QPoint gp = mapToGlobal(pos);
    bool b = pDwmDefWindowProc(winId(), WM_NCHITTEST, 0, MAKELPARAM(gp.x(), gp.y()), &result);
    if (b)
    {
        QStyle::SubControl sc = QStyle::SC_None;
        switch (result)
        {
        case HTMINBUTTON:
            sc = QStyle::SC_TitleBarMinButton;
            break;
        case HTMAXBUTTON:
            sc = QStyle::SC_TitleBarMaxButton;
            break;
        case HTCLOSE:
            sc = QStyle::SC_TitleBarCloseButton;
            break;
        case HTHELP:
            sc = QStyle::SC_TitleBarContextHelpButton;
            break;
        default:
            break;
        }
        return sc;
    }
    return QStyle::SC_None;
}

void WindowTitleBarPrivateWin::styleChanged()
{
    if (window() == Q_NULL)
        return;
    
    m_systemFrameMargins = calcSystemFrameMargins();
    setSysMenuStyle(!styledFrame());
    WindowTitleBarPrivate::styleChanged();
    if (!m_isFinalized)
    {
        fixupQtInternals();
        updateWindowSize();
    }
}

bool WindowTitleBarPrivateWin::handlePaint()
{
#if 0
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_winId, &ps);
    EndPaint(m_winId, &ps);
#endif
    return false;
}

#if 0
static bool qtn_isNonClientTrackMouseEnabled(HWND winId)
{
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_QUERY;
    if (!TrackMouseEvent(&tme))
        qWarning("TrackMouseEvent failed.");
    bool ret = tme.hwndTrack == winId && tme.dwFlags == (TME_LEAVE | TME_NONCLIENT);
    return ret;
}
#endif

void WindowTitleBarPrivateWin::setTrackMouseEnabled(bool enabled)
{
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    if (enabled)
        tme.dwFlags = TME_LEAVE | TME_NONCLIENT;
    else
        tme.dwFlags = TME_LEAVE | TME_NONCLIENT | TME_CANCEL;

    tme.hwndTrack = winId();
    tme.dwHoverTime = HOVER_DEFAULT;
    if (!TrackMouseEvent(&tme))
        qWarning("TrackMouseEvent failed.");
}

bool WindowTitleBarPrivateWin::hasMaximizedStyle() const
{
    return GetWindowLong(winId(), GWL_STYLE) & WS_MAXIMIZE;
}

bool WindowTitleBarPrivateWin::hasFullScreenStyle() const
{
    return !(GetWindowLong(winId(), GWL_STYLE) & WS_THICKFRAME);
}

bool WindowTitleBarPrivateWin::hasSysMenuStyle() const
{
    return GetWindowLong(winId(), GWL_STYLE) & WS_SYSMENU;
}

void WindowTitleBarPrivateWin::setSysMenuStyle(bool enabled)
{
#if 0
    HRESULT result = _SetWindowThemeNonClientAttributes(winId(), WTNCA_VALIDBITS, WTNCA_VALIDBITS);
    Q_UNUSED(result);
#endif
    m_lockStyleChange = true;
    if (enabled)
        SetWindowLong(winId(), GWL_STYLE, GetWindowLong(winId(), GWL_STYLE) | WS_SYSMENU);
    else
        SetWindowLong(winId(), GWL_STYLE, GetWindowLong(winId(), GWL_STYLE) & ~WS_SYSMENU);
    m_lockStyleChange = false;
}

void WindowTitleBarPrivateWin::checkCaptureAfterPress()
{
    bool captureChanged = GetCapture() != winId() && GetCapture() != 0;
    if (underMouse() && captureChanged)
    {
        m_nonClientUnderMouse = false;
        mouseNonClientOut(QPoint(-1, -1));
    }
}

bool WindowTitleBarPrivateWin::handleNonClientMouseEvent(MSG* message, LRESULT &outResult)
{
    QPoint globalPosition(GET_X_LPARAM(message->lParam), GET_Y_LPARAM(message->lParam));

    switch (message->message)
    {
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
        {
           if (!underMouse())
                return false; //Ignore client messages if mouse is not captured.
            POINT pt = { globalPosition.x() , globalPosition.y() };
            ClientToScreen(winId(), &pt);
            globalPosition = QPoint(pt.x, pt.y);
        }
        break;
    default:
        break;
    }

    const QPoint nonClientPosition = mapFromGlobal(globalPosition);

    const Qt::MouseButtons buttons = qtn_state_mouse_buttons();
    Qt::KeyboardModifiers modifiers = qtn_state_keyboard_modifiers();
    Qt::MouseButton button = qtn_get_button_from_message(message->message);

    m_lastMouseButtons = buttons;

    bool result = false;
    
    switch (message->message)
    {
        case WM_NCMOUSELEAVE:
        {
            setTrackMouseEnabled(false);
            m_nonClientUnderMouse = false;
            mouseNonClientOut(nonClientPosition);
        }
        break;

        case WM_NCMOUSEMOVE:
        {
            if (!m_nonClientUnderMouse)
            {
                setTrackMouseEnabled(true);
                m_nonClientUnderMouse = true;
                mouseNonClientOver(nonClientPosition);
            }

            m_lastMouseEventAccepted = false;
            QPoint p = window()->mapFromGlobal(globalPosition);
            QMouseEvent e(QEvent::MouseMove, p, globalPosition, button, buttons, modifiers);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
            eventFilter(window()->windowHandle(), &e);
#endif
            if (m_lastMouseEventAccepted)
            {
                m_ignoreMouseMoveEvent = true;
                OldWindowProc(winId(), WM_MOUSEMOVE, get_clinet_mouse_wparam(buttons, modifiers), MAKELPARAM(p.x(), p.y()));
                QApplication::processEvents();
            }
            m_ignoreMouseMoveEvent = false;
            result = true;
        }
        break;
    case WM_NCLBUTTONDOWN:
    case WM_NCRBUTTONDOWN:
    case WM_NCMBUTTONDOWN:
    case WM_NCLBUTTONDBLCLK:
    case WM_NCRBUTTONDBLCLK:
    case WM_NCMBUTTONDBLCLK:
        {
            setTrackMouseEnabled(false);
            m_lastMouseEventAccepted = false;
            QPoint p = window()->mapFromGlobal(globalPosition);

            bool pressNonClient = !defaultResizeHandler();
            if (pressNonClient)
            {
                OldWindowProc(winId(),
                    get_clinet_message_down(message->message),
                    get_clinet_message_down_wparam(message->message), MAKELPARAM(p.x(), p.y()));
                QApplication::processEvents();
                checkCaptureAfterPress();
            }
            
            if (!m_lastMouseEventAccepted)
            {
                if (pressNonClient)
                {
                    OldWindowProc(winId(), get_clinet_message_up(message->message), 0, MAKELPARAM(p.x(), p.y()));
                    QApplication::processEvents();
                }

                outResult = DefWindowProcW(winId(), message->message, message->wParam, message->lParam);
                setTrackMouseEnabled(true);
            }

            result = true;
        }
        break;

#if 1
    case WM_NCLBUTTONUP:
    case WM_NCRBUTTONUP:
    case WM_NCMBUTTONUP:
#endif
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:

    {
        m_lastMouseEventAccepted = false;
        QPoint p = window()->mapFromGlobal(globalPosition);
        result = OldWindowProc(winId(), get_clinet_message_up(message->message), 0, MAKELPARAM(p.x(), p.y()));
        QApplication::processEvents();
        setTrackMouseEnabled(true);
        result = true;
    }
    break;
    default:
        {
            Q_ASSERT(false);
        }
        break;
    }
    
    return result;
}

int WindowTitleBarPrivateWin::handleNonClientHitTest(LPPOINT ptHit)
{
    int result = HTERROR;
    const QPoint clientPosition = mapFromGlobal(QPoint(ptHit->x, ptHit->y));

    Qt::WindowFrameSection frameSection;
    QStyle::SubControl sc = hitTest(clientPosition, frameSection);
    Q_UNUSED(sc);
    switch (frameSection)
    {
    case Qt::LeftSection:
        result = HTLEFT;
        break;
    case Qt::RightSection:
        result = HTRIGHT;
        break;
    case Qt::BottomSection:
        result = HTBOTTOM;
        break;
    case Qt::TopSection:
        result = HTTOP;
        break;
    case Qt::TopLeftSection:
        result = HTTOPLEFT;
        break;
    case Qt::BottomRightSection:
        result = HTBOTTOMRIGHT;
        break;
    case Qt::TopRightSection:
        result = HTTOPRIGHT;
        break;
    case Qt::BottomLeftSection:
        result = HTBOTTOMLEFT;
        break;
    case Qt::TitleBarArea:
        result = HTCAPTION;
        break;
    default:
        break;
    }


    if (result == HTCAPTION)
    {
        switch (sc)
        {
        case QStyle::SC_TitleBarSysMenu:
            result = HTSYSMENU;
            break;
        case QStyle::SC_TitleBarMinButton:
            result = HTMINBUTTON;
            break;
        case QStyle::SC_TitleBarMaxButton:
            result = HTMAXBUTTON;
            break;
        case QStyle::SC_TitleBarCloseButton:
            result = HTCLOSE;
            break;
        case QStyle::SC_TitleBarNormalButton:
            result = HTZOOM;
            break;
        case QStyle::SC_TitleBarShadeButton:
            result = result; //TODO
            break;
        case QStyle::SC_TitleBarUnshadeButton:
            result = result; //TODO
            break;
        case QStyle::SC_TitleBarContextHelpButton:
            result = HTHELP;
            break;
        case QStyle::SC_TitleBarLabel:
        default:
            break;
        }
    }

    return result;
}

bool WindowTitleBarPrivateWin::isAeroGlassEnabled() const
{
    return m_aeroGlassEnabled;
}

void WindowTitleBarPrivateWin::setAeroGlassEnabled(bool enabled)
{
    if (m_aeroGlassEnabled == enabled)
        return;

    if (!isCompositionAvailable() && enabled)
        return; //You can't set glass effect on Windows that does not suport composition.

    m_aeroGlassEnabled = enabled;

    if (winId() != 0)
        initializeWindowUI();
}

void WindowTitleBarPrivateWin::showSystemMenu(const QPoint& pos)
{
    if (window() == Q_NULL)
        return;

    Qt::WindowType flags = window()->windowType();
    HMENU menu = GetSystemMenu(winId(), FALSE);
    if (!menu)
        return;

#define enabled (MF_BYCOMMAND | MF_ENABLED)
#define disabled (MF_BYCOMMAND | MF_GRAYED)

    ::EnableMenuItem(menu, SC_MINIMIZE, (flags & Qt::WindowMinimizeButtonHint) ? enabled : disabled);
    bool maximized = IsZoomed(winId());

    ::EnableMenuItem(menu, SC_MAXIMIZE, !(flags & Qt::WindowMaximizeButtonHint) || maximized ? disabled : enabled);
    EnableMenuItem(menu, SC_RESTORE, maximized ? enabled : disabled);

    EnableMenuItem(menu, SC_SIZE, (flags & Qt::MSWindowsFixedSizeDialogHint) || maximized ? disabled : enabled);
    EnableMenuItem(menu, SC_MOVE, maximized ? disabled : enabled);
    EnableMenuItem(menu, SC_CLOSE, enabled);
    MENUITEMINFO closeItem;
    closeItem.cbSize = sizeof(MENUITEMINFO);
    closeItem.fMask = MIIM_STATE;
    closeItem.fState = MFS_DEFAULT;
    SetMenuItemInfo(menu, SC_CLOSE, FALSE, &closeItem);
#undef enabled
#undef disabled

    const int ret = TrackPopupMenuEx(menu, TPM_LEFTALIGN | TPM_TOPALIGN |
        TPM_NONOTIFY | TPM_RETURNCMD, pos.x(), pos.y(), winId(), 0);
    if (ret)
        OldWindowProc(winId(), WM_SYSCOMMAND, WPARAM(ret), 0);
}

void WindowTitleBarPrivateWin::setTransitionsEffectEnabled(bool enabled)
{
    if (pDwmSetWindowAttribute != Q_NULL)
    {
        BOOL value = !enabled;
        HRESULT res = pDwmSetWindowAttribute(winId(), DWMWA_TRANSITIONS_FORCEDISABLED, &value, sizeof(BOOL));
        Q_UNUSED(res);
    }
}

bool WindowTitleBarPrivateWin::isCompositionAvailable() const
{
    static bool tried = pDwmIsCompositionEnabled == 0;
    static BOOL dwmEnabled = FALSE;
    if (!tried)
        tried = SUCCEEDED(pDwmIsCompositionEnabled(&dwmEnabled));
    return dwmEnabled;
}

void WindowTitleBarPrivateWin::setRedraw(bool redraw)
{ 
    DefWindowProcW(winId(), WM_SETREDRAW, (WPARAM)(BOOL)redraw, 0);
}

bool WindowTitleBarPrivateWin::isIconic() const
{ 
    return IsIconic(winId());
} 

void WindowTitleBarPrivateWin::forceNonClientSizeUpdate() 
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#else
#endif
    // Inform the application about changes in the nc frame.
    RECT rect;
    GetWindowRect(winId(), &rect);
    SetWindowPos(winId(), 
                 NULL, 
                 rect.left, rect.top,
                 RectWidth(rect), RectHeight(rect),
                 SWP_FRAMECHANGED);
}

void WindowTitleBarPrivateWin::initializeWindowUI() 
{
    m_frameWindows10 = calcWidndows10FrameMargins();

    updateDwmSettings(true);
    if (!isAeroGlassEnabled())
    {
        RECT rc;
        GetWindowRect(winId(), &rc);
        SendMessageW(winId(), WM_NCCALCSIZE, false, (LPARAM)&rc);
    }
 
    if (pSetWindowTheme != Q_NULL)
    {
        if (isAeroGlassEnabled())
            pSetWindowTheme(winId(), L"Explorer", Q_NULL);
        else
            pSetWindowTheme(winId(), L"", L"");
    }

    m_isActive = GetActiveWindow() == winId();
    m_maximized = hasMaximizedStyle();
    m_fullScreen = hasFullScreenStyle();

    m_isFinalized = false;
}
 
void WindowTitleBarPrivateWin::finalizeWindowUI()
{
    m_isFinalized = true;
    resizeNonClientEvent(QSize());
    if (isDestroyed())
        return;

    if (pSetWindowTheme != Q_NULL)
        pSetWindowTheme(winId(), L"Explorer", Q_NULL);

    updateDwmSettings(false);
    
    fixupQtInternals();
    setSysMenuStyle(true);
    window()->setWindowFlags(m_oldFlags);
    window()->show();
}

static bool isNcCalcSizeSupportedByQt()
{
    QStringList version = QString(qVersion()).split('.');
    Q_ASSERT(version.size() == 3);
    if (version.size() != 3)
        return false;
    int major = version[0].toInt();
    int minor = version[1].toInt();
    int patch = version[2].toInt();
    return QT_VERSION_CHECK(major, minor, patch) >= QT_VERSION_CHECK(5, 9, 0);
}

bool WindowTitleBarPrivateWin::handleNonClientCalcSize(NCCALCSIZE_PARAMS* sizeParams, bool calcRects)
{
    if (!calcRects)
        return false;
#if 0
    RECT oldrect = sizeParams->rgrc[0];
    OldWindowProc(winId(), WM_NCCALCSIZE, calcRects, (LPARAM)sizeParams);
    sizeParams->rgrc[0] = oldrect;
    return true;
#endif
    if (isNcCalcSizeSupportedByQt() && isAeroGlassEnabled())
        return false;

    QMargins margins = frameMargins() + extraFrameMargins();

    sizeParams->rgrc[0].left += margins.left();
    sizeParams->rgrc[0].top += margins.top();
    sizeParams->rgrc[0].right -= margins.right();
    sizeParams->rgrc[0].bottom -= margins.bottom();

    return true;
}

bool WindowTitleBarPrivateWin::handleMessage(MSG* msg, LRESULT& result)
{
    if (m_isFinalized)
        return false;

    if ((msg->message == WM_STYLECHANGING || msg->message == WM_STYLECHANGED) && m_lockStyleChange)
    {
        if (msg->message == WM_STYLECHANGED)
            m_lockStyleChange = false;
        return true;
    }
    switch (msg->message)
    {
        case WM_NCCREATE:
            {
                if (handleNonClientCreate())
                    return true;
            }
            break;
        case WM_NCDESTROY:
            {
            }
            break;
        case WM_STYLECHANGED:
        case WM_GETMINMAXINFO:
            {
                bool changed = false;
                bool maximized = hasMaximizedStyle();
                bool fullScreen = hasFullScreenStyle();
                if (m_maximized != maximized)
                {
                    m_maximized = maximized;
                    changed = true;
                }
                if (m_fullScreen != fullScreen)
                {
                    m_fullScreen = fullScreen;
                    changed = true;
                }
                if (changed)
                    fixupQtInternals(false);
            }
            break;
        case WM_SYSKEYDOWN :
            if (!hasSysMenuStyle() && quint32(msg->wParam) == VK_SPACE && 
                ((GetKeyState(VK_LMENU) & 0x80) || (GetKeyState(VK_RMENU) & 0x80)))
            {
                showSystemMenu(window()->geometry().topLeft());
                return true;
            }
            break;
        case WM_NCACTIVATE:
        case WM_NCPAINT:
            {
                if (msg->message == WM_NCACTIVATE)
                {
                    m_isActive = (BOOL)msg->wParam;
                    nonClientActivateEvent(m_isActive);
                    if (!isActive())
                    {
                        setTrackMouseEnabled(false);
                    }
                    else
                    {
                        if (underMouse())
                        {
                            Qt::MouseButtons buttons = qtn_state_mouse_buttons();
                            if (buttons & Qt::LeftButton)
                                setTrackMouseEnabled(true);
                        }
                    }
                }

                if (isAeroGlassEnabled())
                    result = DefWindowProcW(msg->hwnd, msg->message, msg->wParam, msg->lParam);

                updateWindowSize();

                if (handleNonClientPaint(msg->message == WM_NCACTIVATE ? (HRGN)1 : (HRGN)msg->wParam)) 
                    return true;
                return true;
            }
            break; 
        case WM_NCCALCSIZE:
            {
                if (handleNonClientCalcSize(reinterpret_cast<NCCALCSIZE_PARAMS *>(msg->lParam), (BOOL)msg->wParam))
                    return true;

                //Dirty hack to avoid toolbar paint artifacts on old Windows (without DWM - XP, NT etc.)
                DWORD oldStyles = GetWindowLong(winId(), GWL_STYLE);
                DWORD newStyles = oldStyles & ~(WS_DLGFRAME | WS_VSCROLL | WS_HSCROLL | WS_BORDER | WS_SIZEBOX);
                if (!isAeroGlassEnabled())
                {
                    m_lockStyleChange = true;
                    SetWindowLong(winId(), GWL_STYLE, newStyles);
                    m_lockStyleChange = false;
                }

                result = OldWindowProc(winId(), msg->message, msg->wParam, msg->lParam);

                if (!isAeroGlassEnabled())
                {
                    m_lockStyleChange = true;
                    SetWindowLong(winId(), GWL_STYLE, oldStyles);
                    m_lockStyleChange = false;
                }
                return true;
            }
            break;
        case WM_SETTEXT:
            {
                result = OldWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
                if (!isAeroGlassEnabled())
                    handleNonClientPaint((HRGN)1);
                return true;
            }
            break;
        case WM_SIZE:
            {
                updateWindowSize();
            }
            break;
        case WM_PAINT:
            {
                if (handlePaint())
                    return true;
            }
            break;
        case WM_CLOSE:
            {
                if (isAeroGlassEnabled())
                    setTransitionsEffectEnabled(false);
            }
            break;
        case WM_SYSCOMMAND:
            {
                if (msg->wParam == SC_MAXIMIZE)
                {
                }
            }
            break;
        break;

        case WM_NCMOUSELEAVE:
        case WM_NCMOUSEMOVE:

        case WM_NCLBUTTONDOWN:
        case WM_NCRBUTTONDOWN:
        case WM_NCMBUTTONDOWN:

        case WM_NCLBUTTONDBLCLK:
        case WM_NCRBUTTONDBLCLK:
        case WM_NCMBUTTONDBLCLK:

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
#if 1
        case WM_NCLBUTTONUP:
        case WM_NCRBUTTONUP:
        case WM_NCMBUTTONUP:
#endif
        {
            if (handleNonClientMouseEvent(msg, result))
                return true;
            }
            break;
        case WM_SETCURSOR:
            {
                if (m_hitTest != HTCLIENT && !defaultResizeHandler())
                    return true;
            }
            break;
        case WM_CAPTURECHANGED:
            {
            }
            break;
        case WM_NCHITTEST:
            {
                POINT pt;
                POINTSTOPOINT(pt, msg->lParam);
                m_hitTest = handleNonClientHitTest(&pt);
                if (m_hitTest == HTERROR)
                    m_hitTest = OldWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
                result = m_hitTest;
                return true;
            }
        break;
        case WM_THEMECHANGED:
            {
                styleChanged();
            }
            break;
        case WM_DWMCOLORIZATIONCOLORCHANGED:
            {
                styleChanged();
            }
            break;
        default:
            {
            }
            break;
    }
    return false;
}

#define CEF_COLOR_BACKGROUND_ACTIVE         RGB(60, 63, 65)
#define CEF_COLOR_BACKGROUND_INACTIVE       RGB(68, 71, 73) 

void WindowTitleBarPrivateWin::enforceMenuBackground()
{
    static HBRUSH mBackgroundActiveBrush = 0;
    static HBRUSH mBackgroundInactiveBrush = 0; 

    if (mBackgroundActiveBrush == NULL) {                            
        mBackgroundActiveBrush = CreateSolidBrush(CEF_COLOR_BACKGROUND_ACTIVE);
    }

    if (mBackgroundInactiveBrush == NULL) {                            
        mBackgroundInactiveBrush = CreateSolidBrush(CEF_COLOR_BACKGROUND_INACTIVE);
    } 

    HMENU menu = GetSystemMenu(winId(), false);
    MENUINFO mi;
    ZeroMemory(&mi, sizeof(mi));
    mi.cbSize = sizeof(mi);
    mi.fMask = MIM_BACKGROUND;
    GetMenuInfo(menu, &mi);

    if (m_isActive)
    {
        if (mi.hbrBack != mBackgroundActiveBrush)
        {
            mi.hbrBack = mBackgroundActiveBrush;
            SetMenuInfo(menu, &mi);
        }
    }
    else
    {
        if (mi.hbrBack != mBackgroundInactiveBrush)
        {
            mi.hbrBack = mBackgroundInactiveBrush;
            SetMenuInfo(menu, &mi);
        }
    }
} 

#if 0
RECT WindowTitleBarPrivateWin::getLogicalClientRect() const
{
    WINDOWINFO wi;
    ZeroMemory(&wi, sizeof(wi));
    wi.cbSize = sizeof(wi);
    GetWindowInfo(winId(), &wi);
   
    RECT rect;
    rect.top = wi.rcClient.top - wi.rcWindow.top;
    rect.left = wi.rcClient.left - wi.rcWindow.left;
    rect.bottom = rect.top + RectHeight(wi.rcClient);
    rect.right = rect.left + RectWidth(wi.rcClient);

    return rect;
}
#endif

void WindowTitleBarPrivateWin::paintNonClientArea(HDC hdc)
{
    if (m_ncpaintptr == Q_NULL)
        return;

    QRegion clipRegion = getNonClientRegion();

    if (clipRegion.isEmpty())
        return;

    {
        m_ncpaintptr->image().fill(Qt::transparent);
        QPainter painter(&m_ncpaintptr->image());
        painter.setClipRegion(clipRegion);
        paintNonClientEvent(&painter);
    }

    QMargins margins = extraFrameMargins();
    QVector<QRect> rects = clipRegion.rects();
    for (int i = 0; i < rects.size(); ++i)
    {
        QRect r = rects[i];
        BitBlt(hdc, r.left() + margins.left(), r.top() + margins.top(),
            r.width(), r.height(), m_ncpaintptr->hdc(), r.left(), r.top(), SRCCOPY);
    }
}

bool WindowTitleBarPrivateWin::mouseEventFilter(QObject* object, QEvent* event)
{
    if (QWidget::mouseGrabber() != Q_NULL)
        return false;

    bool result = false;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    if (object == window()->windowHandle())
#else
    if (object == window())
#endif
    {
        switch (event->type())
        {
        case QEvent::MouseMove:
        {
            MouseEventQt me(*static_cast<QMouseEvent *>(event));
            QMargins margins = frameMargins();
            me.setPos(me.pos() + QPoint(margins.left(), margins.top()));
            if (containsPoint(me.pos()))
            {
                if (!m_ignoreMouseMoveEvent)
                {
                    mouseMoveNonClientEvent(&me);
                    m_lastMouseEventAccepted = me.isAccepted();
                }
                result = true;
            }
        }
        break;
        case QEvent::MouseButtonPress:
        {
            MouseEventQt me(*static_cast<QMouseEvent *>(event));
            QMargins margins = frameMargins();
            me.setPos(me.pos() + QPoint(margins.left(), margins.top()));
            if (underMouse())
            {
                mousePressNonClientEvent(&me);
                m_lastMouseEventAccepted = me.isAccepted();
                result = true;
            }
        }
        break;
        case QEvent::MouseButtonDblClick:
        {
            MouseEventQt me(*static_cast<QMouseEvent *>(event));
            QMargins margins = frameMargins();
            me.setPos(me.pos() + QPoint(margins.left(), margins.top()));
            if (underMouse())
            {
                mouseDoubleClickNonClientEvent(&me);
                m_lastMouseEventAccepted = me.isAccepted();
                result = true;
            }
        }
        break;
        case QEvent::MouseButtonRelease:
        {
            MouseEventQt me(*static_cast<QMouseEvent *>(event));
            QMargins margins = frameMargins();
            me.setPos(me.pos() + QPoint(margins.left(), margins.top()));
            if (underMouse())
            {
                mouseReleaseNonClientEvent(&me);
                m_lastMouseEventAccepted = me.isAccepted();
                result = true;
            }
        }
        break;
        default:
            break;
        }
    }
    return result;
}

bool WindowTitleBarPrivateWin::eventFilter(QObject* object, QEvent* event)
{
    bool result = false;
    if (result || mouseEventFilter(object, event))
        return true;

    return WindowTitleBarPrivate::eventFilter(object, event);
}

void WindowTitleBarPrivateWin::mouseContextNonClientEvent(QContextMenuEvent* event)
{
    WindowTitleBarPrivate::mouseContextNonClientEvent(event);
}

void WindowTitleBarPrivateWin::resizeNonClientEvent(const QSize& windowSize)
{
    if (frameMargins().isNull())
    {
        Q_DELETE_AND_NULL(m_ncpaintptr);
    }
    else
    {
        if (m_ncpaintptr == Q_NULL || m_ncpaintptr->size() != windowSize)
        {
            if (m_ncpaintptr != Q_NULL)
                Q_DELETE_AND_NULL(m_ncpaintptr);
            if (windowSize != QSize())
                m_ncpaintptr = new ImageHDC(windowSize.width(), windowSize.height(), ImageHDC::systemFormat());
        }
    }
    WindowTitleBarPrivate::resizeNonClientEvent(windowSize);
}

QMargins WindowTitleBarPrivateWin::extraFrameMargins() const
{
    if (!isMaximized())
        return QMargins(0, 0, 0, 0);
    
    int borderThicknessDef = systemFrameMargins().left();
    return  QMargins(
        borderThicknessDef - borderThickness(),
        borderThicknessDef - borderThickness(),
        borderThicknessDef - borderThickness(),
        borderThicknessDef - borderThickness());
}

QPoint WindowTitleBarPrivateWin::mapFromGlobal(const QPoint& pos) const
{
    RECT windowRect;
    GetWindowRect(winId(), &windowRect);
    QMargins margins = extraFrameMargins();
    return pos - QPoint(windowRect.left + margins.left(), windowRect.top + margins.top());
}

QPoint WindowTitleBarPrivateWin::mapToGlobal(const QPoint& pos) const
{
    RECT windowRect;
    GetWindowRect(winId(), &windowRect);
    QMargins margins = extraFrameMargins();
    return pos + QPoint(windowRect.left + margins.left(), windowRect.top + margins.top());
}

void WindowTitleBarPrivateWin::installWindow(QWidget* _window)
{
    Q_ASSERT(window() == Q_NULL);

    if (_window == Q_NULL)
        return;

    HWND winId = (HWND)_window->winId();
    if (GetWindowLong(winId, GWL_STYLE) & WS_CHILD)
    {
        Q_ASSERT(false); //Can't manage child window.
        return;
    }

    WindowsHookManager::instance()->addHook(winId, this);
    initializeWindowUI();
    WindowTitleBarPrivate::setWindow(_window);
    m_oldFlags = _window->windowFlags();
    fixupQtInternals();
}

void WindowTitleBarPrivateWin::resetWindow()
{
    if (window() == Q_NULL)
        return;

    finalizeWindowUI();
    WindowsHookManager::instance()->removeHook(this);
    Q_ASSERT(winId() == 0);
    WindowTitleBarPrivate::setWindow(Q_NULL);
}

void WindowTitleBarPrivateWin::fixupQtInternals(bool forceUpdate)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QWindow* _window = window()->windowHandle();
    Q_ASSERT(_window != Q_NULL);
    if (_window == Q_NULL)
        return;
    
    QMargins margins = QMargins();
    
    if (!m_isFinalized && !isFullScreen())
    {
        margins = frameMargins() + extraFrameMargins();
        margins -= systemFrameMargins();
    }

#if 0
    QMargins contentsMargins = m_origContentsMargins;
    if (frameMargins().isNull())
    {
        int border = m_isFinalized || !isMaximized() ? 0 : systemFrameMargins().left();
        contentsMargins += QMargins(border, border, border, border);
    }

    if (window()->contentsMargins() != contentsMargins)
        window()->setContentsMargins(contentsMargins);
#endif

    _window->setProperty("_q_windowsCustomMargins", qVariantFromValue(margins));

#ifdef QTN_PRIVATE_HEADERS
    QPlatformWindow* platformWindow = _window->handle();
    Q_ASSERT(platformWindow != Q_NULL);
    if (platformWindow == Q_NULL)
        return;
    QGuiApplication::platformNativeInterface()->setWindowProperty(platformWindow, QStringLiteral("WindowsCustomMargins"), qVariantFromValue(margins));
#endif
#else
    NONCLIENTMETRICS sysMetrics;
    ZeroMemory(&sysMetrics, sizeof(NONCLIENTMETRICS));
    sysMetrics.cbSize = sizeof(NONCLIENTMETRICS);
    SystemParametersInfo( SPI_GETNONCLIENTMETRICS, NULL, &sysMetrics, NULL);

    //We have to reset data->fstrut_dirty = 1; in QWidget structure to right client area calculation.
    OldWindowProc(winId(), WM_SETTINGCHANGE, SPI_SETNONCLIENTMETRICS, (LPARAM)&sysMetrics);
#endif
    if (forceUpdate)
        forceNonClientSizeUpdate();
}


