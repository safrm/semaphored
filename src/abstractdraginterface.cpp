/****************************************************************************
**
** Author: Miroslav Safr <miroslav.safr@gmail.com>
** Web: http://semaphored.safrm.net/
** Source: http://safrm.net/projects/semaphored
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/

#include <QDateTime>
#include "abstractdraginterface.h"

QString AbstractDragInterface::TIMESTAMP_FORMAT("yyyyMMdd_hhmmss");

AbstractDragInterface::AbstractDragInterface(DragWidget* canvasWidget) :
    m_CanvasWidget(canvasWidget),
    m_i64TimeStamp(QDateTime::currentDateTime().toMSecsSinceEpoch())
{
}
void AbstractDragInterface::setTimeStamp(qint64 mSecsSinceEpoch)
{
    m_i64TimeStamp = mSecsSinceEpoch;
}

QString AbstractDragInterface::creationTimeStampString()
{
    QDateTime timeStamp;
    timeStamp.setMSecsSinceEpoch(m_i64TimeStamp);
    return  timeStamp.toString(AbstractDragInterface::TIMESTAMP_FORMAT);
}

qint64 AbstractDragInterface::creationTimeStamp()
{
    return m_i64TimeStamp;
}
