#include "../../../../Headers/GDrive/Network/Operations/SM_OperationsManager.h"
#include "../../../../Headers/GDrive/Core/SM_DriveEngine.h"
#include "../../../../Headers/GDrive/Settings/SM_SettingsManager.h"
#include <QStringList>

SM_OperationsManager::SM_OperationsManager(QObject *parent):
    SM_NetworkManager(parent)
{
}

void SM_OperationsManager::setAccountInfo(const QString &accessToken, const QString &refreshToken)
{
    QString userInfoQuery, aboutInfoQuery;
    queries.userAboutInfo(userInfoQuery, aboutInfoQuery);

    accountInfo = new SM_AccountInfo(userInfoQuery, aboutInfoQuery, accessToken, refreshToken);

    connect(accountInfo, SIGNAL(signalAccountInfoReceived(SM_AccountInfo::Data&)), this, SLOT(slotAccountInfoReceived(SM_AccountInfo::Data&)));

    accountInfo->setInfo();
}

void SM_OperationsManager::slotAccountInfoReceived(SM_AccountInfo::Data &data)
{
    SM_SettingsManager settingsManager;

    if(!settingsManager.isAnyAccount())
    {
        settingsManager.setCurrentAccount(static_cast<int> (ELeft), data.email);
        settingsManager.setCurrentAccount(static_cast<int> (ERight), data.email);
    }

    settingsManager.saveAccountInfo(data);

    accountInfo->deleteLater();
    emit signalAccountInfoReadyToUse();
}

bool SM_OperationsManager::operationPossible(void)
{
    bool is = false;

    int index = SDriveEngine::inst()->getContentMngr()->getPanel()->currentIndex().row();

    if(index >= 0)
    {
        QString itemText = SDriveEngine::inst()->getContentMngr()->getPanel()->currentItem()->text(0);

        if(itemText != PARENT_FOLDER_SIGN)
        {
            is = true;
        }
    }

    return is;
}

void SM_OperationsManager::slotNewFolder(void)
{
    createFolderDialog = new SM_CreateFolderDialog(SDriveEngine::inst()->getParent());

    connect(createFolderDialog, SIGNAL(signalAccept(const QString&)), this, SLOT(slotAcceptCreateFolder(const QString&)));
    connect(createFolderDialog, SIGNAL(signalReject()), this, SLOT(slotRejectCreateFolder()));
    connect(createFolderDialog, SIGNAL(signalFinished(int)), this, SLOT(slotFinishedCreateFolder(int)));

    createFolderDialog->setNameLabel();
    createFolderDialog->exec();
}

void SM_OperationsManager::slotRejectCreateFolder(void)
{
    delete createFolderDialog;
}

void SM_OperationsManager::slotFinishedCreateFolder(int result)
{
    Q_UNUSED(result);
    delete createFolderDialog;
}

void SM_OperationsManager::slotCopyFile(void)
{
    if(!operationPossible())
    {
        SM_CommonTools::msg(tr("Ningún fichero ha sido seleccionado."));
        return;
    }

    SM_Items::Data source(SDriveEngine::inst()->getContentMngr()->getCurrentFileInfo());
    QString destFolderUrl(SDriveEngine::inst()->getContentMngr(true)->getParentFolderUrl());

    copy.file(source, destFolderUrl);
}

void SM_OperationsManager::slotMoveFile(void)
{
    if(!operationPossible())
    {
        SM_CommonTools::msg(tr("Ningún fichero ha sido seleccionado."));
        return;
    }

    SM_Items::Data source(SDriveEngine::inst()->getContentMngr()->getCurrentFileInfo());
    QString destFolderUrl(SDriveEngine::inst()->getContentMngr(true)->getParentFolderUrl());

    move.item(source, destFolderUrl);
}

void SM_OperationsManager::slotDeleteItem(void)
{
    del.item(SDriveEngine::inst()->getContentMngr()->getCurrentFileInfo());
}

void SM_OperationsManager::slotRenameItem(void)
{
    if(!operationPossible())
    {
        SM_CommonTools::msg(tr("Ningún fichero ha sido seleccionado."));
        return;
    }

    QTreeWidgetItem *item = SDriveEngine::inst()->getContentMngr()->getPanel()->currentItem();

    editingItemText = item->text(0);

    item->setFlags(item->flags() | Qt::ItemIsEditable);
    SDriveEngine::inst()->getContentMngr()->getPanel()->editItem(item, 0);

    connect(SDriveEngine::inst()->getContentMngr()->getPanel()->itemDelegate(), SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)), this, SLOT(slotItemEditDone()));
}

void SM_OperationsManager::slotItemEditDone(void)
{
    QTreeWidgetItem *item = SDriveEngine::inst()->getContentMngr()->getPanel()->currentItem();
    SM_Items::Data source = SDriveEngine::inst()->getContentMngr()->getCurrentFileInfo();

    QString itemTextAfterEditing = item->text(0);

    if(editingItemText != itemTextAfterEditing)
    {
        rename.item(source, itemTextAfterEditing);
        editingItemText.clear();
    }
}

void SM_OperationsManager::slotShareFile(void)
{
    //share.file(SDriveEngine::inst()->getContentMngr()->getCurrentFileInfo());
    share.file();
}

void SM_OperationsManager::slotAcceptCreateFolder(const QString &name)
{
    if(name == "" || name.contains(QRegExp("[/.<>]")) || name.contains(QRegExp("\\\\")) || name.contains(QRegExp("\"")))
    {
        SM_CommonTools::msg(tr("¡Por favor introduce un nombre válido!"));
        return;
    }

//    if(operationPossible()) createFolder(name, SDriveEngine::inst()->getFilesMngr()->getParentFolderUrl());
//    else CommonTools::msg(tr("¡Por favor selecciona un panel!"));

    QString parentFolderUrl(SDriveEngine::inst()->getContentMngr()->getParentFolderUrl());

    create.folder(name, parentFolderUrl);
    delete createFolderDialog;
}
