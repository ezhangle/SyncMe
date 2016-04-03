#ifndef FILESTRANSFER_H
#define FILESTRANSFER_H

#include "../Core/SM_DriveEngine.h"

class SM_FilesTransferUI : public QObject
{
    Q_OBJECT
public:
    explicit SM_FilesTransferUI(QObject *parent = 0);

public:
    void upload(void);
    void download(void);

private slots:
    void slotDownload(void);
    void slotUpload(void);

};

#endif // FILESTRANSFER_H
