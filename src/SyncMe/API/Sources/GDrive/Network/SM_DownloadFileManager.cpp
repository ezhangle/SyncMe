#include "SM_DownloadFileManager.h"
#include "gui/forms/SM_GDrive_MainWindow.h"
#include "settings/SM_SettingsManager.h"
#include "share/SM_Debug.h"
#include "ui_mainwindow.h"

SM_DownloadFileManager::SM_DownloadFileManager(QObject *parent) :
    SM_NetworkManager(parent)
{
}

void SM_DownloadFileManager::startDownload(QUrl url, QString& fileName, const QString& type)
{
    init();

    fileType = type;

    SM_CommonTools::setHeader(SM_SettingsManager().accessToken(), request);

    setProgressBarSettings(fileName, tr("Descargando fichero: "));

    request.setUrl(url);

    file.open(QIODevice::WriteOnly);

    reply = networkManager->get(request);

    connect(reply, SIGNAL(finished()), this, SLOT(slotDownloadFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotDownloadReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(slotDownloadProgress(qint64,qint64)));

    connectErrorHandlers();
}

void SM_DownloadFileManager::slotDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    progressBarDialog.setMaximum(bytesTotal);
    progressBarDialog.setValue(bytesReceived);
}

void SM_DownloadFileManager::slotDownloadFinished()
{
    progressBarDialog.hide();

    state = EReady;

    file.flush();
    file.close();
}

void SM_DownloadFileManager::slotDownloadReadyRead()
{
    file.write(reply->readAll());
}