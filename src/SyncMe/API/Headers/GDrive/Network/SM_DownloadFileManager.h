#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include "../Share/SM_CommonTools.h"
#include "SM_NetworkManager.h"

class SM_DownloadFileManager : public SM_NetworkManager
{
    Q_OBJECT
public:
    explicit SM_DownloadFileManager(QObject *parent = 0);

public:
    void startDownload(QUrl url, QString &fileName, const QString &type = "text/html");

protected slots:
    virtual void slotDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    virtual void slotDownloadFinished();
    virtual void slotDownloadReadyRead();
};

#endif // DOWNLOADMANAGER_H
