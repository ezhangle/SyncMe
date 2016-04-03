#include "../../../Headers/GDrive/Network/SM_NetworkManager.h"
#include "../../../Headers/GDrive/Share/SM_CommonTools.h"
#include "../../../Headers/GDrive/Share/SM_Debug.h"

SM_NetworkManager::SM_NetworkManager(QObject *parent) :
    QObject(parent),
    state(EReady),
    operationCanceled(false)
{
    this->parent = parent;
}

void SM_NetworkManager::init(void)
{
    networkManager.reset(new QNetworkAccessManager(this));
}

QNetworkAccessManager* SM_NetworkManager::getNetworkManager(void) const
{
    return  networkManager.data();
}

SM_NetworkManager::~SM_NetworkManager()
{
}

void SM_NetworkManager::slotReplyReadyRead()
{
    replyStr.append(reply->readAll());
}

void SM_NetworkManager::slotError(QNetworkReply::NetworkError error)
{
    DEBUG << "Código de error: " << error;

    state = EReady;

    if(error == QNetworkReply::AuthenticationRequiredError)
    {
        DEBUG << "Error | AuthenticationRequiredError (sin permisos), en la URL: " << request.url();
        reply->abort();
        emit signalAccessTokenRequired();
    }

    if(error == QNetworkReply::UnknownNetworkError)
    {
        static bool msgOnScreen = false;
        QString msgStr("Si aparece este error es que la libreria OpenSSL no esta instalado correctamente, lee acerca de esto en http://syncme.wordpress.com/");

        if(!msgOnScreen)
        {
            msgOnScreen = true;
            SM_CommonTools::errorMsg("QNetworkReply::UnknownNetworkError", msgStr);
            msgOnScreen = false;
        }
    }
}

void SM_NetworkManager::slotSslErrors(const QList<QSslError> &errors)
{
    foreach(const QSslError &e, errors) DEBUG << "Error: " << e.error();
}

SM_NetworkManager::EStates SM_NetworkManager::getState(void) const
{
    return state;
}

void SM_NetworkManager::setState(SM_NetworkManager::EStates currentState)
{
    state = currentState;
}

void SM_NetworkManager::connectErrorHandlers(void)
{
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(const QList<QSslError>&)),this, SLOT(slotSslErrors(const QList<QSslError>&)));
}

void SM_NetworkManager::setProgressBarSettings(const QString &fileName, const QString &progressBarDialogInfoText)
{
    state = EBusy;
    operationCanceled = false;

    file.setFileName(fileName);

    QFileInfo fi(file.fileName());

    progressBarDialog.setParent(static_cast<QWidget*>(parent), Qt::Dialog);
    progressBarDialog.setText(progressBarDialogInfoText + fi.fileName());
    progressBarDialog.show();

    connect(&progressBarDialog, SIGNAL(signalProgressCanceled()), this, SLOT(slotProgressCanceled()));  
}

void SM_NetworkManager::slotProgressCanceled()
{
    operationCanceled = true;
    reply->abort();
}

void SM_NetworkManager::slotPostFinished(QNetworkReply* reply)
{
    if (reply->error())
    {
        progressBarDialog.hide();
        DEBUG << "Respuesta con el error";

        return;
    }

    postFinishedActions(reply);
}

void SM_NetworkManager::getRequest(const QString &url)
{
    init();

    request.setUrl(QUrl(url));

    reply = networkManager->get(request);

    connect(networkManager.data(), SIGNAL(finished(QNetworkReply*)),this, SLOT(slotReplyFinished(QNetworkReply*)));
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReplyReadyRead()));

    connectErrorHandlers();
}

void SM_NetworkManager::postRequest(QUrl url)
{
    init();

    request.setUrl(url);
    reply = networkManager->post(request, postData);

    connect(networkManager.data(), SIGNAL(finished(QNetworkReply*)), this, SLOT(slotPostFinished(QNetworkReply*)));
    connectErrorHandlers();
}

void SM_NetworkManager::putRequest(const QString &url, const QByteArray &data)
{
    request.setUrl(QUrl(url));

    reply = networkManager->put(request, data);

    connect(reply, SIGNAL(finished()), this, SLOT(slotUploadFinished()));
    connect(reply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(slotUploadProgress(qint64, qint64)));

    connectErrorHandlers();
}

void SM_NetworkManager::putRequest(QUrl url)
{
    init();

    request.setUrl(url);
    reply = networkManager->put(request, postData);

    connect(reply, SIGNAL(finished()), this, SLOT(slotPutFinished()));
    connectErrorHandlers();
}

void SM_NetworkManager::deleteRequest(QUrl url)
{
    init();

    request.setUrl(url);
    reply = networkManager->deleteResource(request);

    connect(networkManager.data(), SIGNAL(finished(QNetworkReply*)), this, SLOT(slotReplyFinished(QNetworkReply*)));
    connectErrorHandlers();
}


const SM_NetworkManager* SM_NetworkManager::self(void) const
{
    return this;
}

QNetworkRequest SM_NetworkManager::getRequest(void) const
{
    return request;
}

void SM_NetworkManager::postFinishedActions(QNetworkReply*) {}
void SM_NetworkManager::slotReplyFinished(QNetworkReply*){}
void SM_NetworkManager::slotPutFinished(void){}
