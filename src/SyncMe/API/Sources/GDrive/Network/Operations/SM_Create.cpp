#include "../../../../Headers/GDrive/Network/Operations/SM_Create.h"
#include "../../../../Headers/GDrive/Settings/SM_SettingsManager.h"

SM_Create::SM_Create(QObject *parent) :
    SM_NetworkManager(parent)
{
}

void SM_Create::folder(const QString &name, const QString &parentFolderUrl)
{    
    postData = queries.getCreateFolderData(name, parentFolderUrl);

    queries.setRawHeader(SM_SettingsManager().accessToken(), request);
    postRequest(queries.constructCreateFolderUrl());
}

void SM_Create::slotPostFinished(QNetworkReply* reply)
{
    SM_NetworkManager::slotPostFinished(reply);
    updatePanelContent(false);
}
