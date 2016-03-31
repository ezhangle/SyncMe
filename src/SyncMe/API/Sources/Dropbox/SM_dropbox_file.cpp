#include "../../Headers/Dropbox/SM_dropbox_file.h"

SM_Dropbox_File::SM_Dropbox_File(QObject *parent) :
    QIODevice(parent),
    _conManager(this)
{
    _init(NULL, "", 1024);
    connectSignals();
}

SM_Dropbox_File::SM_Dropbox_File(SM_QDropbox *api, QObject *parent) :
    QIODevice(parent),
    _conManager(this)
{
    _init(api, "", 1024);
    obtainToken();
    connectSignals();
}

SM_Dropbox_File::SM_Dropbox_File(QString filename, SM_QDropbox *api, QObject *parent) :
    QIODevice(parent),
    _conManager(this)
{
    _init(api, filename, 1024);
   obtainToken();
   connectSignals();
}

SM_Dropbox_File::~SM_Dropbox_File()
{
    if(_buffer != NULL)
        delete _buffer;
    if(_evLoop != NULL)
        delete _evLoop;
}

bool SM_Dropbox_File::isSequential() const
{
    return true;
}

bool SM_Dropbox_File::open(QIODevice::OpenMode mode)
{
#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "SM_Dropbox_File::open(...)" << endl;
#endif
    if(!QIODevice::open(mode))
        return false;

  /*  if(isMode(QIODevice::NotOpen))
        return true; */

    if(_buffer == NULL)
        _buffer = new QByteArray();

#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "SM_Dropbox_File: opening file" << endl;
#endif

    // clear buffer and reset position if this file was opened in write mode
    // with truncate - or if append was not set
    if(isMode(QIODevice::WriteOnly) &&
       (isMode(QIODevice::Truncate) || !isMode(QIODevice::Append))
      )
    {
#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "SM_Dropbox_File: _buffer cleared." << endl;
#endif
        _buffer->clear();
        _position = 0;
    }
    else
    {
#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "SM_Dropbox_File: reading file content" << endl;
#endif
        if(!getFileContent(_filename))
            return false;

        if(isMode(QIODevice::WriteOnly)) // write mode here means append
            _position = _buffer->size();
        else if(isMode(QIODevice::ReadOnly)) // read mode here means start at the beginning
            _position = 0;
    }

    obtainMetadata();

    return true;
}

void SM_Dropbox_File::close()
{
    if(isMode(QIODevice::WriteOnly))
        flush();
    QIODevice::close();
    return;
}

void SM_Dropbox_File::setApi(SM_QDropbox *dropbox)
{
    _api = dropbox;
    return;
}

SM_QDropbox *SM_Dropbox_File::api()
{
    return _api;
}

void SM_Dropbox_File::setFilename(QString filename)
{
    _filename = filename;
    return;
}

QString SM_Dropbox_File::filename()
{
    return _filename;
}

bool SM_Dropbox_File::flush()
{
#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "SM_Dropbox_File::flush()" << endl;
#endif

    return putFile();
}

bool SM_Dropbox_File::event(QEvent *event)
{
#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "processing event: " << event->type() << endl;
#endif
    return QIODevice::event(event);
}

void SM_Dropbox_File::setFlushThreshold(qint64 num)
{
    if(num<0)
        num = 0;
    _bufferThreshold = num;
    return;
}

qint64 SM_Dropbox_File::flushThreshold()
{
    return _bufferThreshold;
}

void SM_Dropbox_File::setOverwrite(bool overwrite)
{
    _overwrite = overwrite;
    return;
}

bool SM_Dropbox_File::overwrite()
{
    return _overwrite;
}

qint64 SM_Dropbox_File::readData(char *data, qint64 maxlen)
{
#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "SM_Dropbox_File::readData(...), maxlen = " << maxlen << endl;
    QString buff_str = QString(*_buffer);
    qDebug() << "old bytes = " << _buffer->toHex() << ", str: " << buff_str <<  endl;
    qDebug() << "old size = " << _buffer->size() << endl;
#endif

    if(_buffer->size() == 0 || _position >= _buffer->size())
        return 0;

    if(_buffer->size() < maxlen)
        maxlen = _buffer->size();

    QByteArray tmp = _buffer->mid(_position, maxlen);
    const qint64 read = tmp.size();
    memcpy(data, tmp.data(), read);

#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "new size = " << _buffer->size() << endl;
    qDebug() << "new bytes = " << _buffer->toHex() << endl;
#endif

    _position += read;

    return read;
}

qint64 SM_Dropbox_File::writeData(const char *data, qint64 len)
{
#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "old content: " << _buffer->toHex() << endl;
#endif

    qint64 oldlen = _buffer->size();
    _buffer->insert(_position, data, len);

#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "new content: " << _buffer->toHex() << endl;
#endif

    // flush if the threshold is reached
    _currentThreshold += len;
    if(_currentThreshold > _bufferThreshold)
        flush();

    int written_bytes = len;

    if(_buffer->size() != oldlen+len)
        written_bytes = (oldlen-_buffer->size());

    _position += written_bytes;

    return written_bytes;
}

void SM_Dropbox_File::networkRequestFinished(QNetworkReply *rply)
{
    rply->deleteLater();

#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "SM_Dropbox_File::networkRequestFinished(...)" << endl;
#endif

    if (rply->error() != QNetworkReply::NoError)
    {
        lastErrorCode = rply->error();
        stopEventLoop();
        return;
    }

    switch(_waitMode)
    {
    case waitForRead:
        rplyFileContent(rply);
        stopEventLoop();
        break;
    case waitForWrite:
        rplyFileWrite(rply);
        stopEventLoop();
        break;
    case notWaiting:
        break; // when we are not waiting for anything, we don't do anything - simple!
    default:
#ifdef SM_QTDROPBOX_DEBUG
        // debug information only - this should not happen, but if it does we
        // ignore replies when not waiting for anything
        qDebug() << "SM_Dropbox_File::networkRequestFinished(...) got reply in unknown state (" << _waitMode << ")" << endl;
#endif
        break;
    }
}

void SM_Dropbox_File::obtainToken()
{
    _token       = _api->token();
    _tokenSecret = _api->tokenSecret();
    return;
}

void SM_Dropbox_File::connectSignals()
{
    connect(&_conManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(networkRequestFinished(QNetworkReply*)));
    return;
}

bool SM_Dropbox_File::isMode(QIODevice::OpenMode mode)
{
    return ( (openMode()&mode) == mode );
}

bool SM_Dropbox_File::getFileContent(QString filename)
{
#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "SM_Dropbox_File::getFileContent(...)" << endl;
#endif
    QUrl request;
    request.setUrl(QDROPBOX_FILE_CONTENT_URL, QUrl::StrictMode);
    request.setPath(QString("/%1/files/%2")
                    .arg(_api->apiVersion().left(1))
                    .arg(filename));

    QUrlQuery query;
    query.addQueryItem("oauth_consumer_key", _api->appKey());
    query.addQueryItem("oauth_nonce", SM_QDropbox::generateNonce(128));
    query.addQueryItem("oauth_signature_method", _api->signatureMethodString());
    query.addQueryItem("oauth_timestamp", QString::number((int) QDateTime::currentMSecsSinceEpoch()/1000));
    query.addQueryItem("oauth_token", _api->token());
    query.addQueryItem("oauth_version", _api->apiVersion());

    QString signature = _api->oAuthSign(request);
    query.addQueryItem("oauth_signature", signature);

    request.setQuery(query);

#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "SM_Dropbox_File::getFileContent " << request.toString() << endl;
#endif

    QNetworkRequest rq(request);
    QNetworkReply *reply = _conManager.get(rq);
    connect(this, &SM_Dropbox_File::operationAborted, reply, &QNetworkReply::abort);
    connect(reply, &QNetworkReply::downloadProgress, this, &SM_Dropbox_File::downloadProgress);

    _waitMode = waitForRead;
    startEventLoop();

    if(lastErrorCode != 0)
    {
#ifdef SM_QTDROPBOX_DEBUG
        qDebug() << "SM_Dropbox_File::getFileContent ReadError: " << lastErrorCode << lastErrorMessage << endl;
#endif
        if(lastErrorCode ==  SM_DROPBOX_ERROR_FILE_NOT_FOUND)
        {
            _buffer->clear();
#ifdef SM_QTDROPBOX_DEBUG
        qDebug() << "SM_Dropbox_File::getFileContent: file does not exist" << endl;
#endif
        }
        else
            return false;
    }

    return true;
}

void SM_Dropbox_File::rplyFileContent(QNetworkReply *rply)
{
    lastErrorCode = 0;

    QByteArray response = rply->readAll();
    QString resp_str;
    SM_Dropbox_Json json;

#ifdef SM_QTDROPBOX_DEBUG
    resp_str = QString(response.toHex());
    qDebug() << "SM_Dropbox_File::rplyFileContent response = " << resp_str << endl;

#endif

    switch(rply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt())
    {
    case SM_DROPBOX_ERROR_BAD_INPUT:
    case SM_DROPBOX_ERROR_EXPIRED_TOKEN:
    case SM_DROPBOX_ERROR_BAD_OAUTH_REQUEST:
    case SM_DROPBOX_ERROR_FILE_NOT_FOUND:
    case SM_DROPBOX_ERROR_WRONG_METHOD:
    case SM_DROPBOX_ERROR_REQUEST_CAP:
    case SM_DROPBOX_ERROR_USER_OVER_QUOTA:
        resp_str = QString(response);
        json.parseString(response.trimmed());
        lastErrorCode = rply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "SM_Dropbox_File::rplyFileContent jason.valid = " << json.isValid() << endl;
#endif
        if(json.isValid())
            lastErrorMessage = json.getString("error");
        else
            lastErrorMessage = "";
        return;
        break;
    default:
        break;
    }

    _buffer->clear();
    _buffer->append(response);
    emit readyRead();
    return;
}

void SM_Dropbox_File::rplyFileWrite(QNetworkReply *rply)
{
#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "SM_Dropbox_File::rplyFileWrite(...)" << endl;
#endif

    lastErrorCode = 0;

    QByteArray response = rply->readAll();
    QString resp_str;
    SM_Dropbox_Json json;

#ifdef SM_QTDROPBOX_DEBUG
    resp_str = response;
    qDebug() << "SM_Dropbox_File::rplyFileWrite response = " << resp_str << endl;

#endif

    switch(rply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt())
    {
    case SM_DROPBOX_ERROR_BAD_INPUT:
    case SM_DROPBOX_ERROR_EXPIRED_TOKEN:
    case SM_DROPBOX_ERROR_BAD_OAUTH_REQUEST:
    case SM_DROPBOX_ERROR_FILE_NOT_FOUND:
    case SM_DROPBOX_ERROR_WRONG_METHOD:
    case SM_DROPBOX_ERROR_REQUEST_CAP:
    case SM_DROPBOX_ERROR_USER_OVER_QUOTA:
        resp_str = QString(response);
        json.parseString(response.trimmed());
        lastErrorCode = rply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "SM_Dropbox_File::rplyFileWrite jason.valid = " << json.isValid() << endl;
#endif
        if(json.isValid())
            lastErrorMessage = json.getString("error");
        else
            lastErrorMessage = "";
        return;
        break;
    default:
        delete _metadata;

        _metadata = new SM_Dropbox_FileInfo{QString{response}.trimmed(), this};
        if (!_metadata->isValid())
            _metadata->clear();
        break;
    }

    emit bytesWritten(_buffer->size());
    return;
}

void SM_Dropbox_File::startEventLoop()
{
#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "SM_Dropbox_File::startEventLoop()" << endl;
#endif
    if(_evLoop == NULL)
        _evLoop = new QEventLoop(this);
    _evLoop->exec();
    return;
}

void SM_Dropbox_File::stopEventLoop()
{
#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "SM_Dropbox_File::stopEventLoop()" << endl;
#endif
    if(_evLoop == NULL)
        return;
    _evLoop->exit();
    return;
}

bool SM_Dropbox_File::putFile()
{

#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "SM_Dropbox_File::putFile()" << endl;
#endif

    QUrl request;
    request.setUrl(QDROPBOX_FILE_CONTENT_URL, QUrl::StrictMode);
    request.setPath(QString("/%1/files_put/%2")
                    .arg(_api->apiVersion().left(1))
                    .arg(_filename));

    QUrlQuery urlQuery;
    urlQuery.addQueryItem("oauth_consumer_key", _api->appKey());
    urlQuery.addQueryItem("oauth_nonce", SM_QDropbox::generateNonce(128));
    urlQuery.addQueryItem("oauth_signature_method", _api->signatureMethodString());
    urlQuery.addQueryItem("oauth_timestamp", QString::number((int) QDateTime::currentMSecsSinceEpoch()/1000));
    urlQuery.addQueryItem("oauth_token", _api->token());
    urlQuery.addQueryItem("oauth_version", _api->apiVersion());
    urlQuery.addQueryItem("overwrite", (_overwrite?"true":"false"));

    QString signature = _api->oAuthSign(request);
    urlQuery.addQueryItem("oauth_signature", signature);

    request.setQuery(urlQuery);

#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "SM_Dropbox_File::put " << request.toString() << endl;
#endif

    QNetworkRequest rq(request);
    QNetworkReply *reply = _conManager.put(rq, *_buffer);
    connect(this, &SM_Dropbox_File::operationAborted, reply, &QNetworkReply::abort);
    connect(reply, &QNetworkReply::uploadProgress, this, &SM_Dropbox_File::uploadProgress);

    _waitMode = waitForWrite;
    startEventLoop();

    if(lastErrorCode != 0)
    {
#ifdef SM_QTDROPBOX_DEBUG
        qDebug() << "SM_Dropbox_File::putFile WriteError: " << lastErrorCode << lastErrorMessage << endl;
#endif
        return false;
    }

    _currentThreshold = 0;

    return true;
}

void SM_Dropbox_File::_init(SM_QDropbox *api, QString filename, qint64 bufferTh)
{
    _api              = api;
    _buffer           = NULL;
    _filename         = filename;
    _evLoop           = NULL;
    _waitMode         = notWaiting;
    _bufferThreshold  = bufferTh;
    _overwrite        = true;
    _metadata         = NULL;
    lastErrorCode     = 0;
    lastErrorMessage  = "";
    _position         = 0;
    _currentThreshold = 0;
    return;
}


SM_Dropbox_FileInfo SM_Dropbox_File::metadata()
{
    if(_metadata == NULL)
        obtainMetadata();

    return _api->requestMetadataAndWait(_filename);
}

bool SM_Dropbox_File::hasChanged()
{
    if(_metadata == NULL)
    {
        if(!metadata().isValid()) // get metadata
            return false;         // if metadata was invalid
    }

    SM_Dropbox_FileInfo serverMetadata = _api->requestMetadataAndWait(_filename);
#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "SM_Dropbox_File::hasChanged() local  revision hash = " << _metadata->revisionHash() << endl;
    qDebug() << "SM_Dropbox_File::hasChanged() remote revision hash = " << serverMetadata.revisionHash() << endl;
#endif
    return serverMetadata.revisionHash().compare(_metadata->revisionHash())!=0;
}

void SM_Dropbox_File::obtainMetadata()
{
    // get metadata of this file
    _metadata = new SM_Dropbox_FileInfo(_api->requestMetadataAndWait(_filename).strContent(), this);
    if(!_metadata->isValid())
        _metadata->clear();
    return;
}

QList<SM_Dropbox_FileInfo> SM_Dropbox_File::revisions(int max)
{
    QList<SM_Dropbox_FileInfo> revisions = _api->requestRevisionsAndWait(_filename, max);
    if(_api->error() != SM_QDropbox::NoError)
        revisions.clear();

    return revisions;
}

bool SM_Dropbox_File::seek(qint64 pos)
{
    if(pos > _buffer->size())
        return false;

    QIODevice::seek(pos);
    _position = pos;
    return true;
}

qint64 SM_Dropbox_File::pos() const
{
    return _position;
}

bool SM_Dropbox_File::reset()
{
    QIODevice::reset();
    _position = 0;
    return true;
}

void SM_Dropbox_File::abort()
{
    emit operationAborted();
}
