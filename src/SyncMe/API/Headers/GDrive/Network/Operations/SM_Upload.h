#ifndef UPLOAD_H
#define UPLOAD_H

#include <QNetworkAccessManager>

class SM_Upload : public QNetworkAccessManager
{
public:
    SM_Upload(QObject *parent = 0);
};

#endif // UPLOAD_H
