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
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include "aboutdialog.h"
#include "weblink.h"
#define COMPILE_TIME_YEAR ((((__DATE__ [7] - '0') * 10 + (__DATE__ [8] - '0')) * 10 \
+ (__DATE__ [9] - '0')) * 10 + (__DATE__ [10] - '0'))


AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
QLabel * iconLabel = new QLabel();
iconLabel->setPixmap(QIcon(":/images/default.png").pixmap(392,221));

QHBoxLayout *versionLayout = new QHBoxLayout();
QLabel* pVersionLabel = new QLabel("version: ");
QLineEdit* pVersionLineEdit = new QLineEdit(QString(APP_FULL_VERSION_TAG) + QString(" ") + QString(APP_DATE));
pVersionLineEdit->setReadOnly(true);
pVersionLineEdit->selectAll();
versionLayout->addWidget(pVersionLabel);
versionLayout->addWidget(pVersionLineEdit);

//changelog
WebLink* pChangelogFilelabel = new WebLink("Changelog",QUrl("/usr/share/doc/semaphored/TODO"),this);

//webpage
WebLink* pOfficialWebPageLabel = new WebLink("Semaphored",QUrl("http://semaphored.safrm.net/"));
QString sMailTo = "mailto:semaphored@safrm.net?subject=Semaphored: version: " + QString(APP_FULL_VERSION_TAG) + " " + QString(APP_DATE);
WebLink* pSupportemailWebPageLabel = new WebLink("Send email to author",QUrl(sMailTo),this);

QLabel * logoLabel = new QLabel();
logoLabel->setPixmap(QIcon(":/icons/semaphored.png").pixmap(143,73));

QHBoxLayout *licenseLayout = new QHBoxLayout();
licenseLayout->setAlignment(Qt::AlignHCenter);
QLabel* copyrightLabel = new QLabel();
QString sCopyright =  "Copyright " +  QString(Qt::Key_copyright) + QString(" ") + QString().setNum(int(COMPILE_TIME_YEAR));
copyrightLabel->setTextFormat(Qt::RichText);
copyrightLabel->setText(sCopyright);
WebLink* webPageLabel = new WebLink("safrm.net",QUrl("http://www.safrm.net"),this);
licenseLayout->addWidget(webPageLabel);
licenseLayout->addWidget(copyrightLabel);
WebLink* licenseFilelabel = new WebLink("License LGPL 2.1",QUrl("/usr/share/doc/semaphored/LICENSE.LGPL"),this);

//BUTTONS
QPushButton* closeButton = new QPushButton(tr("&Close"));
connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
//ALL TOGETHER
QVBoxLayout *mainLayout = new QVBoxLayout(this);
mainLayout->addWidget(iconLabel,0,Qt::AlignHCenter);
mainLayout->addLayout(versionLayout);
mainLayout->addWidget(pChangelogFilelabel,0,Qt::AlignHCenter);
mainLayout->addWidget(pOfficialWebPageLabel,0,Qt::AlignHCenter);
mainLayout->addWidget(pSupportemailWebPageLabel,0,Qt::AlignHCenter);
mainLayout->addWidget(logoLabel,0,Qt::AlignHCenter);
mainLayout->addWidget(new QWidget());
mainLayout->addLayout(licenseLayout, 0);
mainLayout->addWidget(licenseFilelabel, 0, Qt::AlignHCenter);
mainLayout->addWidget(logoLabel,0,Qt::AlignHCenter);
mainLayout->addWidget(new QWidget());
mainLayout->addWidget(closeButton,0,Qt::AlignHCenter);
setLayout(mainLayout);

setWindowTitle("About Semaphored ...");
setWindowIcon(QIcon(":/icons/semaphored.svg"));
setModal(true);
}

QSize AboutDialog::sizeHint() const
{
    return QSize(400,200);
}
