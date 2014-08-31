/***********************************************************************
*PointDownload
*Copyright (C) 2014  PointTeam
*
* Author:     Choldrim <choldrim@foxmail.com>
* Maintainer: Choldrim <choldrim@foxmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/

#include "xwaretask.h"

#define UPDATE_XML_INTERVAL 0

XwareTask::XwareTask(QObject *parent) :
    QObject(parent)
{
    connect(XwareTaskEntity::getInstance(), SIGNAL(sRealTimeDataChanged(DownloadingItemInfo)),
            this, SLOT(updateRealTimeData(DownloadingItemInfo)));

    connect(XwareController::getInstance(), SIGNAL(sFinishDownload(QString)),
            this, SIGNAL(sFinishXwareDownload(QString)));
}

XwareTask * XwareTask::xwareTask = NULL;
XwareTask::~XwareTask()
{
    qDebug()<<"~XwareTask()  was be called !!";
}

XwareTask * XwareTask::getInstance()
{
    if (xwareTask == NULL)
        xwareTask = new XwareTask();
    return xwareTask;
}

void XwareTask::addNewDownload(PrepareDownloadInfo info)
{
    QString mappingDir = "";
    // mappingDir = XwareDloadFolderController::getInstance()->getMappingFolder(info.storageDir);

    XwarePopulateObject::getInstance()->addNewDownloadTask(info.downloadURL, mappingDir, QStringList()<<info.fileName);
}

void XwareTask::stopDownload(QString URL)
{
    suspendDownloading(URL);
}

void XwareTask::suspendDownloading(QString URL)
{
    QString tid = XwareTaskEntity::getInstance()->getTaskIdByUrl(URL);
    if(tid == "-1")
    {
        // error
        emit sXwareError(URL, QString("no task can not be found by URL:" + URL), Xware);
        return;
    }

    XwarePopulateObject::getInstance()->suspendDownloadingTask(tid);
}

void XwareTask::resumeDownloading(QString URL)
{
    QString tid = XwareTaskEntity::getInstance()->getTaskIdByUrl(URL);
    if(tid == "-1")
    {
        // error
        emit sXwareError(URL, QString("no task can not be found by URL:" + URL), Xware);
        return;
    }

    XwarePopulateObject::getInstance()->resumeDownloadingTask(tid);
}

void XwareTask::entryOfflineChannel(QString URL)
{
    QString tid = XwareTaskEntity::getInstance()->getTaskIdByUrl(URL);
    if(tid == "-1")
    {
        // error
        emit sXwareError(URL, QString("no task can not be found by URL:" + URL), Xware);
        return;
    }

    XwarePopulateObject::getInstance()->entryOfflineChannel(tid);
}

void XwareTask::entryHighSpeedChannel(QString URL)
{
    QString tid = XwareTaskEntity::getInstance()->getTaskIdByUrl(URL);
    if(tid == "-1")
    {
        // error
        emit sXwareError(URL, QString("no task can not be found by URL:" + URL), Xware);
        return;
    }

    XwarePopulateObject::getInstance()->entryHighSpeedChannel(tid);
}

void XwareTask::updateRealTimeData(DownloadingItemInfo info)
{
    emit sRealTimeData(info);
}

