/****************************************************************************
**
** Author: Miroslav Safr <miroslav.safr@gmail.com>
** Web: http://semaphored.safrm.net/
** Source: https://github.com/safrm/semaphored
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
#include <QStringList>
#include <QtDebug>
#include "version.h"

Version::Version()
{
}
//converts   X.Y.Z to uint min: 0	max: 4294 967 295 (4)
uint Version::shortVersionToNum(const QString & sShortVersion)
{
    QStringList list = sShortVersion.split(".");
    if(list.size()!=3) {
        qCritical() << "Short version should to be in X.Y.Z format: " <<  sShortVersion;
        return 0;
    }
    uint uiVersion = list.at(0).toUInt() * 1000000 + list.at(1).toUInt() * 1000 + list.at(2).toUInt();
    return uiVersion;
}
