#include "../../../../Headers/GDrive/Network/Operations/SM_Children.h"
#include "../../../../Headers/GDrive/Settings/SM_SettingsManager.h"
#include "../../../../Headers/GDrive/Parsers/SM_JSONParser.h"
#include "../../../../Headers/GDrive/Share/SM_Debug.h"

SM_Children::SM_Children(QObject *parent) :
    SM_NetworkManager(parent)
{
}

void SM_Children::fetch()
{
    queries.setRawHeader(SM_SettingsManager().accessToken(), request);
    getRequest(queries.construcChildrenUrl(QString("root")));
}

void SM_Children::slotReplyFinished()
{
    DEBUG << "==========================================================================================================================\n";
    DEBUG << replyStr;
    DEBUG << "==========================================================================================================================\n";

    SM_JSONParser jParser;
    QStringList pathValues;

    pathValues << "items" << "parents";
    DEBUG << "!!!!" << jParser.getParams(replyStr, pathValues, QString("id"));

    // Hay que arreglar el parser JSON para leer bien los paquetes de Google Drive

    //DEBUG << jParser.getParams(replyStr, pathValues, QString("selfLink"));
    //DEBUG << jParser.getParams(replyStr, pathValues, QString("childLink"));

    //updatePanelContent(false);
}
