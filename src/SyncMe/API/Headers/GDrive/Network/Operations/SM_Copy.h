#ifndef COPY_H
#define COPY_H

#include "../../Network/SM_NetworkManager.h"
#include "SM_Operation.h"
#include "../../GUI/SM_Items.h"

class SM_Copy : public SM_NetworkManager, public SM_Operation
{
    Q_OBJECT
public:
    explicit SM_Copy(QObject *parent = 0);

signals:
    void fileCopied(SM_Items::Data &source);

protected slots:
    void slotPostFinished(QNetworkReply *reply);

public:
    void file(const SM_Items::Data &source, const QString &destFolderUrl);

private:
     SM_Items::Data sourceData;
};

#endif // COPY_H
