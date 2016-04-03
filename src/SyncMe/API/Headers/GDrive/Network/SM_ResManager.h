#ifndef RESMANAGER_H
#define RESMANAGER_H

#include "../Share/SM_Defs.h"
#include "../Network/SM_DownloadFileManager.h"

class SM_ResManager: public SM_DownloadFileManager
{
     Q_OBJECT
public:
    explicit SM_ResManager(QObject *parent = 0);

public:
    void cash(const QString &url);

public slots:
    virtual void slotDownloadFinished();

signals:
    void signalResDownloaded();
};

#endif // RESMANAGER_H
