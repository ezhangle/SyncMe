#include "SM_ResManager.h"
#include "share/SM_CommonTools.h"

SM_ResManager::SM_ResManager(QObject *parent):
            SM_DownloadFileManager(parent)
{
}

void SM_ResManager::cash(const QString &url)
{
  QString fileNameToSave(QFileInfo(url).fileName());

  startDownload(QUrl(url), fileNameToSave, "image/png");
}

void SM_ResManager::slotDownloadFinished()
{
    SM_DownloadFileManager::slotDownloadFinished();
    emit signalResDownloaded();
}