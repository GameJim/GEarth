/****************************************************************************
**
** Qtitan Library by Developer Machines (Microsoft-Ribbon implementation for Qt.C++)
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
#include "QtnCommonStyleV5.h"
#include "QtnCommonStyleV5Private.h"
#include <qt_windows.h>


QTITAN_USE_NAMESPACE

int CommonStyleV5Private::pixelMetricFromSystemDp(QStyle::PixelMetric pm, const QStyleOption* option, const QWidget* widget)
{
    Q_UNUSED(option);
    switch (pm) 
    {
        case QStyle::PM_TitleBarHeight:
            return widget && (widget->windowType() == Qt::Tool)
                ? GetSystemMetrics(SM_CYSMCAPTION) + GetSystemMetrics(SM_CXSIZEFRAME)
                : GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXSIZEFRAME);
        default:
            break;
    }
    return CommonStyleV5Private::InvalidMetric;
}


