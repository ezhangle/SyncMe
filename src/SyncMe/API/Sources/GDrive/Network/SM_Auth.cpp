#include "../../../Headers/GDrive/Network/SM_Auth.h"
#include "../../../Headers/GDrive/Parsers/SM_JSONParser.h"
#include "../../../Headers/GDrive/Share/SM_Debug.h"
#include <QUrlQuery>


SM_Auth::SM_Auth(QObject *parent) :
    SM_NetworkManager(parent)
{
}

SM_Auth::~SM_Auth()
{
}

QUrl SM_Auth::getOAuth2CodeUrl(const QString &scope, const QString &redirectUri, const QString &clientId, bool accessType, bool approvalPrompt, const QString &state)
{
    QUrl url("https://accounts.google.com/o/oauth2/auth?");
    QList<QPair<QString, QString> > query;

    query.append(qMakePair(QString("scope"), scope));
    query.append(qMakePair(QString("redirect_uri"), redirectUri));
    query.append(qMakePair(QString("response_type"), QString("code")));
    query.append(qMakePair(QString("client_id"), clientId));
    query.append(qMakePair(QString("access_type"), accessType ? QString("online") : QString("offline")));
    query.append(qMakePair(QString("approval_prompt"), approvalPrompt ? QString("force") : QString("auto")));
    query.append(qMakePair(QString("state"), state));

    QUrlQuery urlQuery;
    urlQuery.setQueryItems(query);
    url.setQuery(urlQuery);

    return url;
}

QString SM_Auth::getOAuth2Code(const QUrl &url)
{
    QUrlQuery urlQuery(url);
    return urlQuery.queryItemValue("code");
}

void SM_Auth::getTokens(const QString &code, const QString &clientId, const QString &clientSecret, const QString &redirectUri)
{
    currentRequest = EAllTokens;

    postData = "code=";
    postData += code.toLatin1();
    postData += "&client_id=";
    postData += clientId.toLatin1();
    postData += "&client_secret=";
    postData += clientSecret.toLatin1();
    postData += "&redirect_uri=";
    postData += redirectUri.toLatin1();
    postData += "&grant_type=authorization_code";

    performRequest();
}

void SM_Auth::getAccessToken(const QString &clientId, const QString &clientSecret, const QString &refreshToken)
{
    currentRequest = EAccessToken;

    postData = "client_id=";
    postData += clientId.toLatin1();
    postData += "&client_secret=";
    postData += clientSecret.toLatin1();
    postData += "&refresh_token=";
    postData += refreshToken.toLatin1();
    postData += "&grant_type=refresh_token";

    performRequest();
}

void SM_Auth::postFinishedActions(QNetworkReply* reply)
{
    replyStr = reply->readAll();

    SM_JSONParser jParser;

    QString accessToken(jParser.getValue(replyStr, "access_token"));
    QString refreshToken(jParser.getValue(replyStr, "refresh_token"));

    if(currentRequest == EAccessToken)
    {
        emit signalAuthResponse(accessToken);
    }

    if(currentRequest == EAllTokens)
    {
        emit signalAuthResponse(accessToken, refreshToken);
    }
}

void SM_Auth::performRequest(void)
{
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    postRequest(QUrl("https://accounts.google.com/o/oauth2/token"));
}
