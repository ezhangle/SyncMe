#include "../../../Headers/GDrive/Network/SM_Queries.h"
#include "../../../Headers/GDrive/Share/SM_CommonTools.h"
#include "../../../Headers/GDrive/Settings/SM_SettingsManager.h"
#include <QString>

SM_Queries::SM_Queries() :
    urlStartPart("https://www.googleapis.com/drive/v2/")
{
}

void SM_Queries::setRawHeader(const QString &accessToken, QNetworkRequest &request)
{
    request.setRawHeader("User-Agent", SM_SettingsManager().applicationName().toLatin1());
    request.setRawHeader("Authorization", QString("Bearer %1").arg(accessToken).toLatin1());
    request.setRawHeader("Content-Type", "application/json");
}

void SM_Queries::userAboutInfo(QString &userInfoQuery, QString &aboutInfoQuery)
{
    userInfoQuery = QString("https://www.googleapis.com/oauth2/v1/userinfo");
    aboutInfoQuery = urlStartPart + QString("about");
}

QByteArray SM_Queries::getCopyFileData(const QString &sourceName, const QString &destFolderUrl)
{
    return QString("{\"kind\": \"drive#file\", \"title\": \"%1\",\"parents\": [{\"id\":\"%2\"}]}").arg(sourceName).arg(SM_CommonTools::getIDFromURL(destFolderUrl)).toLatin1();
}

QUrl SM_Queries::constructCopyFileUrl(const QString &url)
{
    return QUrl(QString(urlStartPart + QString("files/") + SM_CommonTools::getIDFromURL(url) + QString("/copy")));
}

QByteArray SM_Queries::getRenameFileData(const QString &newName)
{
    return QString("{\"title\": \"%1\"}").arg(newName).toLatin1();
}

QUrl SM_Queries::constructRenameFileUrl(const QString &sourceName)
{
    return QUrl(urlStartPart + QString("files/") + SM_CommonTools::getIDFromURL(sourceName));
}

QByteArray SM_Queries::getCreateFolderData( const QString &name, const QString &folderUrl)
{
    return QString("{\"title\": \"%1\",\"parents\": [{\"id\": \"%2\"}],\"mimeType\": \"application/vnd.google-apps.folder\"}").arg(name).arg(SM_CommonTools::getIDFromURL(folderUrl)).toLatin1();
}

QUrl SM_Queries::constructCreateFolderUrl(void)
{
    return QUrl(urlStartPart + QString("files"));
}

QUrl SM_Queries::constructDeleteFileUrl(const QString &sourceName)
{
  return QUrl(urlStartPart + QString("files/") + SM_CommonTools::getIDFromURL(sourceName));
}

QString SM_Queries::construcChildrenUrl(const QString &Id)
{
    return QString(urlStartPart + QString("files?q=\"") + Id + QString("\" in parents"));
}
