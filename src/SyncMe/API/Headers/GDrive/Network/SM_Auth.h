#ifndef AUTH_H
#define AUTH_H

#include "SM_NetworkManager.h"

class SM_Auth : public SM_NetworkManager
{
    Q_OBJECT
public:
    enum ERequests
    {
        EAccessToken,
        EAllTokens
    };

    explicit SM_Auth(QObject *parent = 0);
    ~SM_Auth();

signals:
    void signalAuthResponse(const QString &accessToken);
    void signalAuthResponse(const QString &accessToken, const QString &refreshToken);

protected:
    virtual void postFinishedActions(QNetworkReply* reply);

public:
    QUrl getOAuth2CodeUrl(const QString &scope, const QString &redirectUri, const QString &clientId , bool accessType = true, bool approvalPrompt = false, const QString &state = QString());
    QString getOAuth2Code(const QUrl &url);
    void getAccessToken(const QString &clientId, const QString &clientSecret, const QString &refreshToken);
    void getTokens(const QString &code, const QString &clientId, const QString &clientSecret, const QString &redirectUri);

private:
    void performRequest(void);

private:
    ERequests currentRequest;
};

#endif // AUTH_H
