#include "../../../../Headers/GDrive/Network/Operations/SM_Delete.h"
#include "../../../../Headers/GDrive/Settings/SM_SettingsManager.h"

SM_Delete::SM_Delete(QObject *parent)
    :SM_NetworkManager(parent)
{
}

void SM_Delete::item(const SM_Items::Data &source)
{
    queries.setRawHeader(SM_SettingsManager().accessToken(), request);
    deleteRequest(queries.constructDeleteFileUrl(source.self));
}

void SM_Delete::slotReplyFinished(QNetworkReply*)
{
   updatePanelContent(false);
}
