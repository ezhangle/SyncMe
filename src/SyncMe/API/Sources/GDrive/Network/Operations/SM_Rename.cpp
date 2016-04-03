#include "SM_Rename.h"
#include "settings/SM_SettingsManager.h"

SM_Rename::SM_Rename(QObject *parent) :
    SM_NetworkManager(parent)
{
}

void SM_Rename::item(const SM_Items::Data &source, const QString &newName)
{
    postData = queries.getRenameFileData(newName);
    queries.setRawHeader(SM_SettingsManager().accessToken(), request);
    putRequest(QUrl(queries.constructRenameFileUrl(source.self)));
}

void SM_Rename::slotPutFinished(void)
{
    updatePanelContent(false);
}