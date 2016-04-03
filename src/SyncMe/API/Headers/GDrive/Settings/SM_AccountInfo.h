#ifndef ABOUTINFO_H
#define ABOUTINFO_H

#include "../Network/SM_NetworkManager.h"

class SM_AccountInfo : public SM_NetworkManager
{
    Q_OBJECT
public:
    enum EQueries
    {
      EUserInfoQuery,
      EAboutInfoQuery
    };

    struct Data
    {
     QString name;
     QString email;        
     QString accessToken;
     QString refreshToken;
     QString domainSharingPolicy;
     QString permissionId;
     long long quotaBytesTotal;
     long long quotaBytesUsed;
     long long quotaBytesUsedInTrash;
    };

    SM_AccountInfo(const QString &uiq, const QString &aiq, const QString &at, const QString &rt = QString());

signals:
   void signalAccountInfoReceived(SM_AccountInfo::Data& data);

protected:
    void slotReplyFinished(QNetworkReply*);

public:
    void setInfo(void);

private:
    void parseReply(void);

private:
    QString userInfoQuery;
    QString aboutInfoQuery;
    EQueries query;
    QString queryStr;
    Data accountData;
};

#endif // ABOUTINFO_H
