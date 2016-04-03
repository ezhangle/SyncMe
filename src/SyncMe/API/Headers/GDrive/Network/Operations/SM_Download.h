#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QNetworkAccessManager>

class SM_Download : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit SM_Download(QObject *parent = 0);
};

#endif // DOWNLOAD_H
