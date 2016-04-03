#ifndef OPERATIONSMANAGER_H
#define OPERATIONSMANAGER_H

#include "../../Network/SM_NetworkManager.h"
#include "../../GUI/SM_Items.h"
#include "../../Settings/SM_AccountInfo.h"
#include "../../Share/SM_TSingleton_Item.h"
#include "../../Share/SM_Defs.h"
#include "../../Network/SM_Queries.h"
#include "../../GUI/Forms/SM_CreateFolderDialog.h"
#include "SM_Copy.h"
#include "SM_Delete.h"
#include "SM_Move.h"
#include "SM_Create.h"
#include "SM_Rename.h"
#include "SM_Share.h"
#include "SM_Children.h"

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
