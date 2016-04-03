#ifndef OPERATIONSMANAGER_H
#define OPERATIONSMANAGER_H

#include "network/SM_NetworkManager.h"
#include "gui/SM_Items.h"
#include "settings/SM_AccountInfo.h"
#include "share/SM_TSingleton_Item.h"
#include "share/SM_Defs.h"
#include "network/SM_Queries.h"
#include "gui/forms/SM_CreateFolderDialog.h"
#include "network/operations/SM_Copy.h"
#include "network/operations/SM_Delete.h"
#include "network/operations/SM_Move.h"
#include "network/operations/SM_Create.h"
#include "network/operations/SM_Rename.h"
#include "network/operations/SM_Share.h"
#include "network/operations/SM_Children.h"

class SM_OperationsManager : public SM_NetworkManager
{
    Q_OBJECT
public:
    SM_OperationsManager(QObject *parent = 0);

public slots:
    void slotDeleteItem(void);
    void slotNewFolder(void);
    void slotCopyFile(void);
    void slotMoveFile(void);
    void slotRenameItem(void);
    void slotItemEditDone(void);
    void slotShareFile(void);
    void slotAcceptCreateFolder(const QString &name);
    void slotRejectCreateFolder(void);
    void slotFinishedCreateFolder(int result);

public:
    void setAccountInfo(const QString &accessToken, const QString &refreshToken = QString());
    bool operationPossible(void);

signals:
    void signalAccountInfoReadyToUse(void);

private slots:  
    void slotAccountInfoReceived(SM_AccountInfo::Data &postData);

private:
    SM_Items::Data fileUrlToDeleteForMoveOperation;
    SM_AccountInfo *accountInfo;
    SM_Queries queries;
    SM_CreateFolderDialog* createFolderDialog;
    QString editingItemText;
    SM_Copy copy;
    SM_Delete del;
    SM_Move move;
    SM_Create create;
    SM_Rename rename;
    SM_Share share;
    SM_Children children;
};

typedef SM_TSingleton_Item<SM_OperationsManager> SOperationsManager;

#endif // OPERATIONSMANAGER_H
