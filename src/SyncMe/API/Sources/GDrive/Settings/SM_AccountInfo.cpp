#include "../../../Headers/GDrive/Settings/SM_AccountInfo.h"
#include "../../../Headers/GDrive/Share/SM_CommonTools.h"
#include "../../../Headers/GDrive/Parsers/SM_JSONParser.h"
#include "../../../Headers/GDrive/Network/SM_Queries.h"
#include "../../../Headers/GDrive/Share/SM_Debug.h"

SM_AccountInfo::SM_AccountInfo(const QString &uiq, const QString &aiq, const QString &at, const QString &rt) :
    userInfoQuery(uiq),
    aboutInfoQuery(aiq),
    query(EUserInfoQuery),
    queryStr(userInfoQuery)
{
    accountData.accessToken = at;
    accountData.refreshToken = rt;
}

void SM_AccountInfo::slotReplyFinished(QNetworkReply*)
{
    parseReply();

    replyStr.clear();

    if(query == EUserInfoQuery)
    {
        queryStr = aboutInfoQuery;
        query = EAboutInfoQuery;

        setInfo();
    }
    else if(query == EAboutInfoQuery)
    {
        queryStr = userInfoQuery;
        query = EUserInfoQuery;

        emit signalAccountInfoReceived(accountData);
    }
}

void SM_AccountInfo::setInfo(void)
{
    SM_Queries().setRawHeader(accountData.accessToken, request);
    getRequest(queryStr);
}

void SM_AccountInfo::parseReply(void)
{
    DEBUG << "<===============================================================================================================";
    DEBUG << "replyStr: " << replyStr;
    DEBUG << "===============================================================================================================>";

    SM_JSONParser jParser;

    if(query == EUserInfoQuery)
    {
        accountData.name = jParser.getValue(replyStr, QString("name"));
        accountData.email = jParser.getValue(replyStr, QString("email"));
    }
    else if(query == EAboutInfoQuery)
    {
        accountData.domainSharingPolicy = jParser.getValue(replyStr, QString("domainSharingPolicy"));
        accountData.permissionId = jParser.getValue(replyStr, QString("permissionId"));
        accountData.quotaBytesTotal = jParser.getValue(replyStr, QString("quotaBytesTotal")).toLongLong();
        accountData.quotaBytesUsed = jParser.getValue(replyStr, QString("quotaBytesUsed")).toLongLong();
        accountData.quotaBytesUsedInTrash = jParser.getValue(replyStr, QString("quotaBytesUsedInTrash")).toLongLong();
    }
}
