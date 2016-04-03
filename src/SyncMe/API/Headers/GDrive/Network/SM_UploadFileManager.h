#ifndef UPLOADMANAGER_H
#define UPLOADMANAGER_H

#include "../Share/SM_CommonTools.h"
#include "SM_NetworkManager.h"
#include <QFileInfo>

class SM_UploadFileManager : public SM_NetworkManager
{
    Q_OBJECT
public:  
    explicit SM_UploadFileManager(QObject *parent = 0);

signals:
    void signalUpdateFileList();

public slots:
    virtual void slotUploadFinished();
    virtual void slotUploadProgress(qint64 bytesSent, qint64 bytesTotal);

public:
    void setUploadSettings(void);
    void startUpload(QUrl url, const QString &fileName);

protected:
    void setProgressBarSettings(const QString &fileName, const QString &progressBarDialogInfoText);
    void postFinishedActions(QNetworkReply* reply);

private:
    QString getContentTypeByExtension(const QString &ext);

private:
    bool allowPut;
};

#endif // UPLOADMANAGER_H
