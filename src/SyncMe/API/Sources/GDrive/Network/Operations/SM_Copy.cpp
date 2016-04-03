#include "SM_Copy.h"
#include "settings/SM_SettingsManager.h"

SM_Copy::SM_Copy(QObject *parent) :
    SM_NetworkManager(parent)
{
}

void SM_Copy::file(const SM_Items::Data &source, const QString &destFolderUrl)
{
    sourceData = source;
    postData = queries.getCopyFileData(source.name, destFolderUrl);

    queries.setRawHeader(SM_SettingsManager().accessToken(), request);
    postRequest(queries.constructCopyFileUrl(source.self));
}

void SM_Copy::slotPostFinished(QNetworkReply *reply)
{
    SM_NetworkManager::slotPostFinished(reply);
    updatePanelContent(true);

    emit fileCopied(sourceData);
}