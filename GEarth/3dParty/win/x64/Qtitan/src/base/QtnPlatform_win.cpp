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

#include <QWidget>
#include <QLibrary>
#include <QIcon>
#include <QPaintEngine>
#include <QApplication>
#include <QDesktopWidget>
#include <QStyle>
#include <QSysInfo>
#include <QSettings>

#ifdef QTN_PRIVATE_HEADERS
#include <qpa/qplatformnativeinterface.h>
#endif

#include "QtnPlatform.h"


#define FALSE_EXIT 2

PtrDwmDefWindowProc Qtitan::pDwmDefWindowProc = Q_NULL;
PtrDwmIsCompositionEnabled Qtitan::pDwmIsCompositionEnabled = Q_NULL;
PtrDwmGetWindowAttribute Qtitan::pDwmGetWindowAttribute = Q_NULL;
PtrDwmSetWindowAttribute Qtitan::pDwmSetWindowAttribute = Q_NULL;
PtrDwmExtendFrameIntoClientArea Qtitan::pDwmExtendFrameIntoClientArea = Q_NULL;
PtrDwmEnableBlurBehindWindow Qtitan::pDwmEnableBlurBehindWindow = Q_NULL;
PtrDwmGetColorizationColor Qtitan::pDwmGetColorizationColor = Q_NULL;
PtrDwmGetColorizationParameters Qtitan::pDwmGetColorizationParameters = Q_NULL;
PtrDwmSetColorizationParameters Qtitan::pDwmSetColorizationParameters = Q_NULL;


PtrDwmFlush Qtitan::pDwmFlush = Q_NULL;

PtrIsAppThemed Qtitan::pIsAppThemed = Q_NULL;
PtrOpenThemeData Qtitan::pOpenThemeData = Q_NULL;
PtrCloseThemeData Qtitan::pCloseThemeData = Q_NULL;
PtrGetThemeRect Qtitan::pGetThemeRect = Q_NULL;
PtrGetThemeSysSize Qtitan::pGetThemeSysSize = Q_NULL;
PtrGetThemeSysFont Qtitan::pGetThemeSysFont = Q_NULL;
PtrDrawThemeTextEx Qtitan::pDrawThemeTextEx = Q_NULL;
PtrDrawThemeIcon Qtitan::pDrawThemeIcon = Q_NULL;
PtrSetWindowTheme Qtitan::pSetWindowTheme = Q_NULL;
PtrGetThemeColor Qtitan::pGetThemeColor = Q_NULL;
PtrGetThemeSysColor Qtitan::pGetThemeSysColor = Q_NULL;
PtrSetWindowThemeAttribute Qtitan::pSetWindowThemeAttribute = Q_NULL;
PtrSetWindowCompositionAttribute Qtitan::pSetWindowCompositionAttribute = Q_NULL;


void Qtitan::qtn_initializeDrawingResources()
{
    static bool tried = false;
    if (!tried) 
    {
        tried = true;

        QLibrary user32Lib(QString::fromLatin1("user32"));
        pSetWindowCompositionAttribute = (PtrSetWindowCompositionAttribute)user32Lib.resolve("SetWindowCompositionAttribute");

        QLibrary dwmLib(QString::fromLatin1("dwmapi"));
        pDwmIsCompositionEnabled = (PtrDwmIsCompositionEnabled)dwmLib.resolve("DwmIsCompositionEnabled");
        pDwmGetWindowAttribute = (PtrDwmGetWindowAttribute)dwmLib.resolve("DwmGetWindowAttribute");
        pDwmSetWindowAttribute = (PtrDwmSetWindowAttribute)dwmLib.resolve("DwmSetWindowAttribute");
        if (pDwmIsCompositionEnabled && pDwmSetWindowAttribute) 
        {
            pDwmDefWindowProc = (PtrDwmDefWindowProc)dwmLib.resolve("DwmDefWindowProc");
            pDwmExtendFrameIntoClientArea = (PtrDwmExtendFrameIntoClientArea)dwmLib.resolve("DwmExtendFrameIntoClientArea");
            pDwmEnableBlurBehindWindow = (PtrDwmEnableBlurBehindWindow)dwmLib.resolve("DwmEnableBlurBehindWindow");
            pDwmGetColorizationColor = (PtrDwmGetColorizationColor)dwmLib.resolve("DwmGetColorizationColor");
            pDwmGetColorizationParameters = (PtrDwmGetColorizationParameters)dwmLib.resolve("127");
            pDwmSetColorizationParameters = (PtrDwmSetColorizationParameters)dwmLib.resolve("131");
            pDwmFlush = (PtrDwmFlush)dwmLib.resolve("DwmFlush");
        }

        QLibrary themeLib(QString::fromLatin1("uxtheme"));
        pIsAppThemed = (PtrIsAppThemed)themeLib.resolve("IsAppThemed");

        if (pIsAppThemed) 
        {
            pOpenThemeData = (PtrOpenThemeData)themeLib.resolve("OpenThemeData");
            pCloseThemeData = (PtrCloseThemeData)themeLib.resolve("CloseThemeData");
            pGetThemeRect = (PtrGetThemeRect)themeLib.resolve("GetThemeRect");
            pGetThemeSysSize = (PtrGetThemeSysSize)themeLib.resolve("GetThemeSysSize");
            pGetThemeSysFont = (PtrGetThemeSysFont)themeLib.resolve("GetThemeSysFont");
            pDrawThemeTextEx = (PtrDrawThemeTextEx)themeLib.resolve("DrawThemeTextEx");
            pDrawThemeIcon = (PtrDrawThemeIcon)themeLib.resolve("DrawThemeIcon");
            pSetWindowTheme = (PtrSetWindowTheme)themeLib.resolve("SetWindowTheme");
            pGetThemeColor      = (PtrGetThemeColor)themeLib.resolve("GetThemeColor");
            pGetThemeSysColor   = (PtrGetThemeSysColor)themeLib.resolve("GetThemeSysColor");
            pSetWindowThemeAttribute = (PtrSetWindowThemeAttribute)themeLib.resolve("SetWindowThemeAttribute");
        }
    }
}

bool Qtitan::qtn_isCompositionAvailable()
{
    static bool tried = pDwmIsCompositionEnabled == 0;
    static BOOL dwmEnabled = FALSE;
    if (!tried)
        tried = SUCCEEDED(pDwmIsCompositionEnabled(&dwmEnabled));
    return dwmEnabled;
}

HDC Qtitan::qtn_createDC()
{
    HDC display_dc = GetDC(0);
    HDC hdc = CreateCompatibleDC(display_dc);
    ReleaseDC(0, display_dc);
    Q_ASSERT(hdc);
    return hdc;
}

HBITMAP Qtitan::qtn_createDIB(HDC hdc, int width, int height, QImage::Format format, uchar **bitsIn)
{
    BITMAPINFO_MASK bmi;
    memset(&bmi, 0, sizeof(bmi));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // top-down.
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biSizeImage = 0;

    if (format == QImage::Format_RGB16) {
        bmi.bmiHeader.biBitCount = 16;
        bmi.bmiHeader.biCompression = BI_BITFIELDS;
        bmi.redMask = 0xF800;
        bmi.greenMask = 0x07E0;
        bmi.blueMask = 0x001F;
    }
    else {
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        bmi.redMask = 0;
        bmi.greenMask = 0;
        bmi.blueMask = 0;
    }

    uchar *bits = Q_NULL;
    HBITMAP bitmap = CreateDIBSection(hdc, reinterpret_cast<BITMAPINFO *>(&bmi),
        DIB_RGB_COLORS, reinterpret_cast<void **>(&bits), 0, 0);
    if (Q_UNLIKELY(!bitmap || !bits))
        qFatal("%s: CreateDIBSection failed.", __FUNCTION__);

    *bitsIn = bits;
    return bitmap;
}


/* ImageHDC */
ImageHDC::ImageHDC(int width, int height,
    QImage::Format format) :
m_hdc(qtn_createDC()), m_bitmap(Q_NULL), m_null_bitmap(Q_NULL)
{
    if (width != 0 && height != 0) {
        uchar *bits;
        m_bitmap = qtn_createDIB(m_hdc, width, height, format, &bits);
        m_null_bitmap = static_cast<HBITMAP>(SelectObject(m_hdc, m_bitmap));
        m_image = QImage(bits, width, height, format);
    }
    else {
        m_image = QImage(width, height, format);
    }

    GdiFlush();
}

ImageHDC::~ImageHDC()
{
    if (m_hdc) {
        if (m_bitmap) {
            if (m_null_bitmap)
                SelectObject(m_hdc, m_null_bitmap);
            DeleteObject(m_bitmap);
        }
        DeleteDC(m_hdc);
    }
}

QImage::Format ImageHDC::systemFormat()
{
    static int depth = -1;
    if (depth == -1) {
        if (HDC defaultDC = GetDC(0)) {
            depth = GetDeviceCaps(defaultDC, BITSPIXEL);
            ReleaseDC(0, defaultDC);
        }
        else {
            depth = 32;
        }
    }
    return depth == 16 ? QImage::Format_RGB16 : QImage::Format_ARGB32_Premultiplied;
}

/* WindowsHook */
WindowsHook::WindowsHook()
    : m_oldProc(Q_NULL), m_winId(Q_NULL), m_winEventHook(Q_NULL)
{
    QLibrary user32Lib(QString::fromLatin1("user32"));

    m_setWinEventProc = (LPFNSETWINEVENTHOOK)user32Lib.resolve("SetWinEventHook");
    m_unhookWinEventProc = (LPFNUNHOOKWINEVENT)user32Lib.resolve("UnhookWinEvent");
    Q_ASSERT(m_setWinEventProc != Q_NULL && m_unhookWinEventProc != Q_NULL);
}
WindowsHook::~WindowsHook()
{
    setWinId(0);
}

HWND WindowsHook::winId() const
{
    return m_winId;
}

void WindowsHook::setWinId(HWND winId)
{
    if (m_winId != 0)
    {
        if (IsWindowUnicode(m_winId))
            SetWindowLongPtrW(m_winId, GWLP_WNDPROC, (LONG_PTR)m_oldProc);
        else
            SetWindowLongPtrA(m_winId, GWLP_WNDPROC, (LONG_PTR)m_oldProc);

        m_oldProc = Q_NULL;

        Q_ASSERT(m_winEventHook != Q_NULL);
        m_unhookWinEventProc(m_winEventHook);
        m_winEventHook = Q_NULL;
    }

    m_winId = winId;
    if (m_winId == 0)
        return;

    if (IsWindowUnicode(m_winId))
    {
        m_oldProc = (WNDPROC)GetWindowLongPtrW(m_winId, GWLP_WNDPROC);
        SetWindowLongPtrW(m_winId, GWLP_WNDPROC, (LONG_PTR)WindowsHookManager::HookWindowProc);
    }
    else
    {
        m_oldProc = (WNDPROC)GetWindowLongPtrA(m_winId, GWLP_WNDPROC);
        SetWindowLongPtrA(m_winId, GWLP_WNDPROC, (LONG_PTR)WindowsHookManager::HookWindowProc);
    }

    Q_ASSERT(m_winEventHook == Q_NULL);
    if (m_winEventHook == Q_NULL)
        m_winEventHook = m_setWinEventProc(EVENT_OBJECT_REORDER, EVENT_OBJECT_REORDER, Q_NULL, &WindowsHookManager::HookWindowEventProc, GetCurrentProcessId(), 0, 0);
}

LRESULT WindowsHook::OldWindowProc(HWND hWnd, uint nMessage, WPARAM wParam, LPARAM lParam)
{
    return ::CallWindowProc(m_oldProc, hWnd, nMessage, wParam, lParam);
}

LRESULT WindowsHook::winProc(HWND hWnd, uint nMessage, WPARAM wParam, LPARAM lParam)
{
    Q_ASSERT(winId() == hWnd);
    return OldWindowProc(hWnd, nMessage, wParam, lParam);
}

void WindowsHook::winEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
    Q_ASSERT(hWinEventHook == m_winEventHook);
    Q_ASSERT(winId() == hWnd);

    Q_UNUSED(hWinEventHook);
    Q_UNUSED(event);
    Q_UNUSED(hWnd);
    Q_UNUSED(idObject);
    Q_UNUSED(idChild);
    Q_UNUSED(dwEventThread);
    Q_UNUSED(dwmsEventTime);
}

/* WindowsHookManager */
WindowsHookManager::WindowsHookManager()
{
}

WindowsHookManager::~WindowsHookManager()
{
    Q_ASSERT(m_hookHash.size() == 0);
    //removeAll();
}

WindowsHookManager* WindowsHookManager::instance()
{
    static WindowsHookManager manager;
    return &manager;
}

LRESULT CALLBACK WindowsHookManager::HookWindowProc(HWND hWnd, uint message, WPARAM wParam, LPARAM lParam)
{
    WindowsHook* hook = WindowsHookManager::instance()->hookByWinId(hWnd);
    if (hook != Q_NULL)
        return hook->winProc(hWnd, message, wParam, lParam);
    return S_FALSE;
}

void CALLBACK WindowsHookManager::HookWindowEventProc(HWINEVENTHOOK hWinEventHook,
    DWORD event, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
    WindowsHook* hook = WindowsHookManager::instance()->hookByWinId(hWnd);
    if (hook != Q_NULL)
        hook->winEventProc(hWinEventHook, event, hWnd, idObject, idChild, dwEventThread, dwmsEventTime);
}

void WindowsHookManager::addHook(HWND hWnd, WindowsHook* hook)
{
    if (m_hookHash.contains(hWnd))
    {
        Q_ASSERT(false); //Can't add hook twice. 
        return;
    }
    hook->setWinId(hWnd);
    m_hookHash.insert(hWnd, hook);
}

void WindowsHookManager::removeHook(WindowsHook* hook)
{
    if (hook == Q_NULL || !m_hookHash.contains(hook->winId()))
    {
        Q_ASSERT(false); ////Can't remove hook twice.
        return;
    }
    m_hookHash.remove(hook->winId());
    hook->setWinId(0);
}

WindowsHook* WindowsHookManager::hookByWinId(HWND hWnd)
{
    if (!m_hookHash.contains(hWnd))
        return Q_NULL;
    return m_hookHash[hWnd];
}

QPixmap Qtitan::qtn_createPixmap(HBITMAP bitmap, int hbitmapFormat)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    return qt_pixmapFromWinHBITMAP(bitmap, hbitmapFormat);
#else
    return QPixmap::fromWinHBITMAP(bitmap, (QPixmap::HBitmapFormat)hbitmapFormat);
#endif
}

HBITMAP Qtitan::qtn_createHBITMAP(const QPixmap& pixmap, int hbitmapFormat)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    return qt_pixmapToWinHBITMAP(pixmap, hbitmapFormat);
#else
    return pixmap.toWinHBITMAP((QPixmap::HBitmapFormat)hbitmapFormat);
#endif
}

HICON Qtitan::qtn_createHICON(const QIcon& icon, QSize& inOutSize)
{
    QPixmap pixmap = icon.pixmap(inOutSize);
    inOutSize = pixmap.size();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    return qt_pixmapToWinHICON(pixmap);
#else
    return pixmap.toWinHICON();
#endif
}

HDC Qtitan::qtn_getDC(QPainter* painter)
{
    HDC hdc = 0;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#ifdef QTN_PRIVATE_HEADERS 
    QPaintDevice* paintDevice = painter->device();
    if (paintDevice->devType() != QInternal::Widget)
        return Q_NULL;
    QWidget* widget = static_cast<QWidget*>(paintDevice);
    QPlatformNativeInterface* nativeInterface = QGuiApplication::platformNativeInterface();
    QBackingStore* backingStore = widget->backingStore();
    hdc = static_cast<HDC>(nativeInterface->nativeResourceForBackingStore(QByteArrayLiteral("getDC"), backingStore));
#else
    Q_UNUSED(painter);
#endif
#else
    QPaintEngine* engine = painter->paintEngine();
    hdc = engine != Q_NULL ? engine->getDC() : 0;
#endif
    return hdc;
}

void Qtitan::qtn_releaseDC(QPainter* painter, HDC dc)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    Q_UNUSED(painter);
    Q_UNUSED(dc);
#else
    QPaintEngine* engine = painter->paintEngine();
    engine != Q_NULL ? engine->releaseDC(dc) : 0;
#endif
}

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
static LOGFONT qfont_to_LOGFONT(const QFont& f)
{
    LOGFONT lf;
    memset(&lf, 0, sizeof(LOGFONT));

    lf.lfHeight = -qRound((double)f.pixelSize());
    lf.lfWidth = 0;
    lf.lfEscapement = 0;
    lf.lfOrientation = 0;
    if (f.weight() == 50)
        lf.lfWeight = FW_DONTCARE;
    else
        lf.lfWeight = (f.weight() * 900) / 99;
    lf.lfItalic = f.style() != QFont::StyleNormal;
    lf.lfCharSet = DEFAULT_CHARSET;

    int strat = OUT_DEFAULT_PRECIS;
    if (f.styleStrategy() & QFont::PreferBitmap) {
        strat = OUT_RASTER_PRECIS;
#ifndef Q_OS_WINCE
    }
    else if (f.styleStrategy() & QFont::PreferDevice) {
        strat = OUT_DEVICE_PRECIS;
    }
    else if (f.styleStrategy() & QFont::PreferOutline) {
        strat = OUT_OUTLINE_PRECIS;
    }
    else if (f.styleStrategy() & QFont::ForceOutline) {
        strat = OUT_TT_ONLY_PRECIS;
#endif
    }

    lf.lfOutPrecision = strat;

    int qual = DEFAULT_QUALITY;

    if (f.styleStrategy() & QFont::PreferMatch)
        qual = DRAFT_QUALITY;
#ifndef Q_OS_WINCE
    else if (f.styleStrategy() & QFont::PreferQuality)
        qual = PROOF_QUALITY;
#endif

    if (f.styleStrategy() & QFont::PreferAntialias) {
        if (QSysInfo::WindowsVersion >= QSysInfo::WV_XP) {
            qual = CLEARTYPE_QUALITY;
        }
        else {
            qual = ANTIALIASED_QUALITY;
        }
    }
    else if (f.styleStrategy() & QFont::NoAntialias) {
        qual = NONANTIALIASED_QUALITY;
    }

    lf.lfQuality = qual;

    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;

    int hint = FF_DONTCARE;
    switch (f.styleHint()) {
    case QFont::Helvetica:
        hint = FF_SWISS;
        break;
    case QFont::Times:
        hint = FF_ROMAN;
        break;
    case QFont::Courier:
        hint = FF_MODERN;
        break;
    case QFont::OldEnglish:
        hint = FF_DECORATIVE;
        break;
    case QFont::System:
        hint = FF_MODERN;
        break;
    default:
        break;
    }

    lf.lfPitchAndFamily = DEFAULT_PITCH | hint;

    QString fam = f.family();

    if (fam.isEmpty())
        fam = QStringLiteral("MS Sans Serif");

    if ((fam == QStringLiteral("MS Sans Serif"))
        && (f.style() == QFont::StyleItalic || (-lf.lfHeight > 18 && -lf.lfHeight != 24))) {
        fam = QStringLiteral("Arial"); // MS Sans Serif has bearing problems in italic, and does not scale
    }
    if (fam == QStringLiteral("Courier") && !(f.styleStrategy() & QFont::PreferBitmap))
        fam = QStringLiteral("Courier New");

    memcpy(lf.lfFaceName, fam.utf16(), sizeof(wchar_t) * qMin(fam.length() + 1, 32));  // 32 = Windows hard-coded

    return lf;
}
#endif

HFONT Qtitan::qtn_get_qfont_handle(const QFont& f)
{
    Q_ASSERT(false); //Does not work under Qt5 :((
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    LOGFONT lf = qfont_to_LOGFONT(f);
    return CreateFontIndirect(&lf);
#else
    return f.handle();
#endif
}

void Qtitan::qtn_release_qfont_handle(HFONT hfont)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    DeleteObject(hfont);
#else
    Q_UNUSED(hfont);
#endif
}

void Qtitan::qtn_setWidgetPostion(QWidget* widget, const QPoint& position)
{
    MoveWindow((HWND)widget->internalWinId(), position.x(), position.y(), widget->width(), widget->height(), false);
}

bool Qtitan::qtn_isWindows10()
{
    return QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS10;
}

bool Qtitan::qtn_setBlurBehindWindow(QWidget* widget)
{
    QWidget* window = widget->isTopLevel() ? widget : widget->nativeParentWidget();
    if (window == Q_NULL)
        return false;


    bool result = false;

    if (pSetWindowCompositionAttribute != Q_NULL)
    {
        QColor color = qtn_getColorizationColor();
        DWORD color_value = RGB(
            qRound(color.red() * ((double)color.alpha() / 255) + 255 - color.alpha()),
            qRound(color.green() * ((double)color.alpha() / 255) + 255 - color.alpha()),
            qRound(color.blue() * ((double)color.alpha() / 255) + 255 - color.alpha()));

        ACCENTPOLICY policy = { ACCENT_ENABLE_BLURBEHIND, 2, color_value, 0 };
        WINCOMPATTRDATA data = { WCA_ACCENT_POLICY, &policy, sizeof(ACCENTPOLICY) };
        result = pSetWindowCompositionAttribute((HWND)window->winId(), &data);
    }
    else if (pDwmEnableBlurBehindWindow != Q_NULL)
    {
        DWM_BLURBEHIND bb;
        ZeroMemory(&bb, sizeof(DWM_BLURBEHIND));

        bb.dwFlags = DWM_BB_ENABLE;
        bb.fEnable = true;
        bb.hRgnBlur = NULL;
        result = SUCCEEDED(pDwmEnableBlurBehindWindow((HWND)window->winId(), &bb));
    }

    return result;
}

bool Qtitan::qtn_setWindowColorization(QWidget* widget, const QColor& color)
{
    QWidget* window = widget->isTopLevel() ? widget : widget->nativeParentWidget();
    if (window == Q_NULL)
        return false;

    bool result = false;

    if (pSetWindowCompositionAttribute != Q_NULL)
    {
        DWORD color_value = RGB(
            qRound(color.red() * ((double)color.alpha() / 255) + 255 - color.alpha()),
            qRound(color.green() * ((double)color.alpha() / 255) + 255 - color.alpha()),
            qRound(color.blue() * ((double)color.alpha() / 255) + 255 - color.alpha()));

        AccentTypes accent = ACCENT_ENABLE_GRADIENT; //ACCENT_ENABLE_TRANSPARENTGRADIENT;
        ACCENTPOLICY policy = { accent, 2, color_value, 0 };
        WINCOMPATTRDATA data = { WCA_ACCENT_POLICY, &policy, sizeof(ACCENTPOLICY) };
        result = pSetWindowCompositionAttribute((HWND)window->winId(), &data);
    }
    return result;
}

QColor Qtitan::qtn_getColorizationColor()
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows", QSettings::NativeFormat);
    if (settings.contains("DWM/ColorizationColor"))
    {
        if (!settings.value("DWM/EnableWindowColorization").toBool())
            return Qt::white;

        DWORD colorizationColor = settings.value("DWM/ColorizationColor").toInt();
        DWORD colorizationColorBalance = settings.value("DWM/ColorizationColorBalance").toInt();
        BYTE ALPHA = 255 * colorizationColorBalance / 100; // Convert from 0-100 to 0-255

        int r = ((qRed(colorizationColor) * ALPHA) + (0xD9 * (255 - ALPHA))) / 255;
        int g = ((qGreen(colorizationColor) * ALPHA) + (0xD9 * (255 - ALPHA))) / 255;
        int b = ((qBlue(colorizationColor) * ALPHA) + (0xD9 * (255 - ALPHA))) / 255;
        return QColor(r, g, b);
    }

    qtn_initializeDrawingResources();
    if (!pDwmGetColorizationColor)
        return QColor(Qt::darkGray);

    DWORD colorizationColor;
    BOOL opaqueBlend = FALSE;
    pDwmGetColorizationColor(&colorizationColor, &opaqueBlend);
    QColor color = QColor(qRed(colorizationColor), qGreen(colorizationColor), qBlue(colorizationColor), qAlpha(colorizationColor));
    return color;
}

void Qtitan::qtn_paintAirEffect(QPainter* painter, const QRect& rect, const QRegion& clip)
{
    QPaintDevice* paintDevice = painter->device();
    if (!qtn_isCompositionAvailable() || paintDevice->devType() != QInternal::Widget)
        return;

    QWidget* widget = static_cast<QWidget*>(paintDevice);
    QWidget* window = widget->isTopLevel() ? widget : widget->nativeParentWidget();
    if (window == Q_NULL)
        return;
    HDC hdc = Qtitan::qtn_getDC(painter);
    if (hdc == Q_NULL)
        return;

    QPoint offset = widget->mapTo(window, QPoint(painter->worldTransform().dx(), painter->worldTransform().dy()));
    QVector<QRect> rects = clip.rects();
    for (int i = 0; i < rects.count(); ++i)
    {
        QRect r = rects[i].intersected(rect);
        if (r.isEmpty())
            continue;
        r.translate(offset);
        RECT rc;
        rc.left = r.left();
        rc.top = r.top();
        rc.right = r.right() + 1;
        rc.bottom = r.bottom() + 1;
        COLORREF old = ::SetBkColor(hdc, RGB(0, 0, 0));
        ::ExtTextOutW(hdc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
        ::SetBkColor(hdc, old);
    }

    qtn_releaseDC(painter, hdc);
}

static bool qtn_getNonClientMetrics(NONCLIENTMETRICS& ncm)
{
    const UINT cbProperSize = sizeof(NONCLIENTMETRICS);
    ZeroMemory(&ncm, sizeof(NONCLIENTMETRICS));
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    return ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, cbProperSize, &ncm, 0);
}

static LOGFONT qtn_getCaptionLogFont(HANDLE hTheme)
{
    LOGFONT lf = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,{ 0 } };
    HRESULT result = S_FALSE;
    if (hTheme)
        result = pGetThemeSysFont(hTheme, HLP_TMT_CAPTIONFONT, &lf);

    if (!SUCCEEDED(result))
    {
        NONCLIENTMETRICS ncm;
        qtn_getNonClientMetrics(ncm);
        lf = ncm.lfCaptionFont;
    }
    return lf;
}

static HFONT qtn_createDwmCaptionFont(HANDLE hTheme)
{
    LOGFONT lf = qtn_getCaptionLogFont(hTheme);
    return ::CreateFontIndirectW(&lf);
}

void Qtitan::qtn_paintTitleBarText(QPainter* painter, const QString& text, const QRect& rect, bool active, const QColor& color)
{
    if (!rect.isValid())
        return;

    HANDLE hTheme = pOpenThemeData((HWND)QApplication::desktop()->winId(), L"WINDOW");
    if (!hTheme)
        return;

    ImageHDC imagehdc(rect.width(), rect.height(), ImageHDC::systemFormat());

    HFONT hCaptionFont = qtn_createDwmCaptionFont(hTheme);
    HFONT hOldFont = (HFONT)SelectObject(imagehdc.hdc(), (HGDIOBJ)hCaptionFont);


    const UINT uFormat = DT_VCENTER | DT_CENTER | DT_END_ELLIPSIS | DT_SINGLELINE | DT_NOPREFIX;
    RECT rctext = { 0, 0, rect.width(), rect.height() };

    HLP_DTTOPTS dto;
    memset(&dto, 0, sizeof(HLP_DTTOPTS));
    dto.dwSize = sizeof(HLP_DTTOPTS);
    dto.dwFlags = HLP_DTT_COMPOSITED | HLP_DTT_GLOWSIZE;
    dto.iGlowSize = 5;//glowSize();

    if (color.isValid())
    {
        dto.iGlowSize = 0;
        dto.dwFlags |= HLP_DTT_TEXTCOLOR;
        dto.crText = RGB(color.red(), color.green(), color.blue());
    }

    HRESULT result = pDrawThemeTextEx(hTheme, imagehdc.hdc(), 
        false ? HLP_WP_MAXCAPTION : HLP_WP_CAPTION, active ? HLP_FS_ACTIVE : HLP_FS_INACTIVE,
        (LPCWSTR)text.utf16(), -1, uFormat, &rctext, &dto);
    Q_UNUSED(result);

    SelectObject(imagehdc.hdc(), (HGDIOBJ)hOldFont);
    DeleteObject(hCaptionFont);

    painter->drawImage(rect, imagehdc.image());
}

QPixmap Qtitan::qtn_getTitleBarIcon(QWidget* widget)
{
    HICON hIcon = (HICON)(DWORD_PTR)::SendMessageW((HWND)widget->winId(), WM_GETICON, ICON_SMALL, 0);
    if (!hIcon)
        hIcon = (HICON)(DWORD_PTR)::GetClassLongPtrW((HWND)widget->winId(), GCLP_HICONSM);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    return qt_pixmapFromWinHICON(hIcon);
#else
    return QPixmap::fromWinHICON(hIcon);
#endif
}

void Qtitan::qtn_paintTitleBarIcon(QPainter* painter, const QIcon& icon, const QRect& rect)
{
    if (icon.isNull() || !rect.isValid())
        return;

    QSize size = rect.size();
    HICON hIcon = qtn_createHICON(icon, size);
    if (hIcon == Q_NULL)
        return;
    QRect r = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size, rect);
    ImageHDC imagehdc(rect.width(), rect.height(), ImageHDC::systemFormat());
    DrawIconEx(imagehdc.hdc(), r.left() - rect.left(), r.top() - rect.top(), hIcon, 0, 0, 0, NULL, DI_NORMAL | DI_COMPAT);
    painter->drawImage(rect, imagehdc.image());
    DestroyIcon(hIcon);
}

static void qtn_add_rect(HRGN& hrgn, const QRect& r)
{
    HRGN temp = CreateRectRgn(r.left(), r.top(), r.x() + r.width(), r.y() + r.height());
    if (temp) {
        HRGN dest = CreateRectRgn(0, 0, 0, 0);
        int result = CombineRgn(dest, hrgn, temp, RGN_OR);
        if (result)
        {
            DeleteObject(hrgn);
            hrgn = dest;
        }
        DeleteObject(temp);
    }
}

HRGN Qtitan::qtn_create_region(const QRegion& region)
{
    HRGN rgn = CreateRectRgn(0, 0, 0, 0);
    QVector<QRect> rects = region.rects();
    for (int i = 0; i < rects.count(); ++i)
    {
        QRect r = rects[i];
        qtn_add_rect(rgn, r);
    }
    return rgn;
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0)) || !defined(QTN_PRIVATE_HEADERS)
static QFont::Weight qtn_weightFromInteger(int weight)
{
    if (weight < 400)
        return QFont::Light;
    else if (weight < 600)
        return QFont::Normal;
    else if (weight < 700)
        return QFont::DemiBold;
    else if (weight < 800)
        return QFont::Bold;
    else
        return QFont::Black;
}
#endif

static QFont qtn_LOGFONTtoQFont(const LOGFONT& lf, int dpi)
{
    QFont result;
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0)) || !defined(QTN_PRIVATE_HEADERS)
    Q_UNUSED(dpi);
    QString family = QString::fromWCharArray(lf.lfFaceName);
    result = QFont(family);
    result.setItalic(lf.lfItalic);
    if (lf.lfWeight != FW_DONTCARE)
        result.setWeight(qtn_weightFromInteger(lf.lfWeight));
    int lfh = qAbs(lf.lfHeight);
    const HDC displayDC = GetDC(0);
    result.setPointSizeF(lfh * 72.0 / GetDeviceCaps(displayDC, LOGPIXELSY));
    ReleaseDC(0, displayDC);
    result.setUnderline(false);
    result.setOverline(false);
    result.setStrikeOut(false);
#else
    QPlatformNativeInterface* nativeInterface = QGuiApplication::platformNativeInterface();
    Q_ASSERT(nativeInterface != Q_NULL);
    bool ret = QMetaObject::invokeMethod(nativeInterface, "logFontToQFont", Qt::DirectConnection,
        Q_RETURN_ARG(QFont, result),
        Q_ARG(const void *, &lf),
        Q_ARG(int, dpi));
    Q_ASSERT(ret);
#endif
    return result;
}


QFont Qtitan::qtn_getWindowCaptionFont(QWidget* window, int dpi)
{
    if (dpi == -1)
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        dpi = window->devicePixelRatio() * window->logicalDpiY();
#else
        dpi = 0;
#endif
    }

    HWND hWnd = Q_NULL;
    if (!pOpenThemeData)
        return QApplication::font("QMdiSubWindowTitleBar");

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0)) || !defined(QTN_PRIVATE_HEADERS)
    hWnd = (HWND)window->winId();
#else
    if (window->windowHandle())
        hWnd = static_cast<HWND>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow(QByteArrayLiteral("handle"), window->windowHandle()));
#endif
    const HANDLE hTheme = hWnd ? pOpenThemeData(hWnd, L"WINDOW") : Q_NULL;
    if (!hTheme)
        return QApplication::font("QMdiSubWindowTitleBar");
    const LOGFONT logFont = qtn_getCaptionLogFont(hTheme);
    return qtn_LOGFONTtoQFont(logFont, dpi);
}

