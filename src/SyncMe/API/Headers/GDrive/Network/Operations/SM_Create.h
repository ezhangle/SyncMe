#ifndef CREATE_H
#define CREATE_H

#include "network/SM_NetworkManager.h"
#include "SM_Operation.h"
#include "gui/SM_Items.h"

class SM_Create : public SM_NetworkManager, public SM_Operation
{
    Q_OBJECT
public:
    SM_Create(QObject *parent = 0);

protected slots:
    void slotPostFinished(QNetworkReply* reply);

public:
    void folder(const QString &name, const QString &parentFolderUrl);
};

#endif // CREATE_H
