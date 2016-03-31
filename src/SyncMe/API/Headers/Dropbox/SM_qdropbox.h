#ifndef SM_QDropbox_H
#define SM_QDropbox_H

#include "SM_dropbox_global.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCryptographicHash>
#include <QDateTime>
#include <QUrl>
#include <QDomDocument>
#include <QEventLoop>
#include <QUrlQuery>

#ifdef QTDROPBOX_DEBUG
#include <QDebug>
#endif

#include "SM_dropbox_global.h"
#include "SM_dropbox_json.h"
#include "SM_dropbox_account.h"
#include "SM_dropbox_fileinfo.h"
#include "SM_dropbox_deltaresponse.h"

typedef int SM_QDropbox_request_type;

const SM_QDropbox_request_type SM_QDropbox_REQ_INVALID = 0x00;
const SM_QDropbox_request_type SM_QDropbox_REQ_CONNECT = 0x01;
const SM_QDropbox_request_type SM_QDropbox_REQ_RQTOKEN = 0x02;
const SM_QDropbox_request_type SM_QDropbox_REQ_AULOGIN = 0x03;
const SM_QDropbox_request_type SM_QDropbox_REQ_REDIREC = 0x04;
const SM_QDropbox_request_type SM_QDropbox_REQ_ACCTOKN = 0x05;
const SM_QDropbox_request_type SM_QDropbox_REQ_ACCINFO = 0x06;
const SM_QDropbox_request_type SM_QDropbox_REQ_RQBTOKN = 0x07;
const SM_QDropbox_request_type SM_QDropbox_REQ_BACCTOK = 0x08;
const SM_QDropbox_request_type SM_QDropbox_REQ_METADAT = 0x09;
const SM_QDropbox_request_type SM_QDropbox_REQ_BACCINF = 0x0A;
const SM_QDropbox_request_type SM_QDropbox_REQ_BMETADA = 0x0B;
const SM_QDropbox_request_type SM_QDropbox_REQ_SHRDLNK = 0x0C;
const SM_QDropbox_request_type SM_QDropbox_REQ_BSHRDLN = 0x0D;
const SM_QDropbox_request_type SM_QDropbox_REQ_REVISIO = 0x0E;
const SM_QDropbox_request_type SM_QDropbox_REQ_BREVISI = 0x0F;
const SM_QDropbox_request_type SM_QDropbox_REQ_DELTA   = 0x10;
const SM_QDropbox_request_type SM_QDropbox_REQ_BDELTA  = 0x11;

//! Struct interno para gestionar las peticiones de red enviadas por SM_QDropbox
/*!
  Esta estructura es usada internamente por SM_QDropbox. Se usa para conectar las peticiones
  de red que son enviadas a la API del servidor de Dropbox con consultas asincronas hechas
  a la API guardada en SM_dropbox
 */
struct SM_QDropbox_request{
    SM_QDropbox_request_type type; //!< Tipo de peticion
    QString method;             //!< Metodo usado para enviar la peticion (POST/GET)
    QString host;               //!< Host que recibe la peticion
    int linked;                 //!< ID de cualquier peticion enlazada (para reenvios de peticiones)
};

//! El punto de entrada principal de la API Dropbox de SyncMe. Provee varias facilidades de conexion
//! e informacion general.
/*!
  SM_QDropbox te da todas las utilidades requeridas para conectarte a cualquier cuenta de Dropbox. Por propositos
  de conexion esta clase provee una interfaz asincrona basada en señales y slots.

  <h3>Conectando a una nueva cuenta</h3>
  Si quieres iniciar una nueva conexion a una cuenta que no ha autorizado a SyncMe a acceder debes usar
  el metodo requestToken() y entonces llamar  requestAccessToken tan pronto como la señal requestTokenFinished()
  es emitida.

  Si el token que estas usando no esta autorizado o ha expirado se emitirá la señal tokenExpired().
  En este caso debes pedir al usuario que reautorice a SyncMe. Un enlace a la interfaz de autorizacion de
  Dropbox se le provee al usuario utilizando el metodo authorizeLink(). Esta API NO automatiza el proceso
  de autorizacion ya que esta caracteristica no la ofrece Dropbox... por lo tanto se mostrara el enlace
  en una ventana del navegador.

  Para reconectarte a una cuenta para un uso posterior de SyncMe se guardara el token y el token secreto
  obtenido despues de requestAccessToken(). Estos valores son distribuidos por las funciones token() y tokenSecret().

  <h3>Conexion a una cuenta autorizada</h3>
  Si la cuenta a la que quieres conectar ya ha autorizado a SyncMe y ya has obtenido un token autorizado y un token secret
  entonces se usara una atajo para conectarse. Se establecera el token y el token secret que se obtuvo en un uso previo
  de SyncMe con las funciones setToken() y setTokenSecret(). No necesitas invocar requestToken() o requestAccessToken().
  Estas funciones solo son llamadas cuando se utiliza SyncMe por primera vez y no hay token y token secret disponibles.

  Si el token o el token secret que estes usando ya ha expirado la señal tokenExpired() se emitirá. En este caso
  se le pedirá reautorización al usuario.

  <h3>Usando peticiones bloqueantes</h3>
  Todas las funciones que piden informacion al servidor tienen una version bloqueante y una no bloqueante.
  Una peticion bloqueante esperara hasta que el servidor responda a tu consulta antes de retornar mientras que
  una no bloqueante retornará inmediatamente. Normalmente las funciones bloqueantes retornan directamente
  un resultado y las no bloqueantes emitirán una señal acorde cuando la peticion haya finalizado.

  \warning El uso de una funcion bloqueante reinicializará la bandera de error. Por lo tanto despues de llamar
  a una funcion bloqueante la funcion error() devolvera SM_QDropbox::NoError si no ha ocurrido ningun error
  o el error que ocurrio cuando se intento procesar la peticion bloqueante.

  \bug La autenticacion HMAC-SHA1 no funciona actualmente

 */
class SM_DROPBOXSHARED_EXPORT SM_QDropbox : public QObject
{
    Q_OBJECT
public:
    //! Metodo para la autenticacion OAuth
    /*! Estos metodos se utilzian para autenticarse con el protocolo OAuth
        \bug La codificacion HMAC-SHA1 no funciona
     */
    enum OAuthMethod{
        Plaintext, /*!< Autenticacion con texto plano, se usa HTTPS automaticamente. */
        HMACSHA1 /*!< Autenticacion con codificacion HMAC-SHA1 */
    };

    //! Estados de error de SM_QDropbox
    /*!
      Este enum se usa para determinar el estado de error actual de la conexion con Dropbox.
      Si ocurre un error se puede acceder al mismo usando la funcion error().
     */
    enum Error{
        NoError,                        /*!< Ningun error */
        CommunicationError,             /*!< Error en la comunicacion con el servidor */
        VersionNotSupported,            /*!< La version de la API de Dropbox usada no es soportada por el servidor */
        UnknownAuthMethod,              /*!< El metodo de autenti. no es soportado */
        ResponseToUnknownRequest,       /*!< La API SM_dropbox ha recibido una respuesta inesperada del servidor */
        APIError,                       /*!< El servidor remoto ha violado el protocolo de la Dropbox REST API enviando datos erroneos. */
        UnknownQueryMethod,             /*!< Error interno. La API intento enviar con un metodo HTTP no soportado. */
        BadInput,                       /*!< Parametro de entrada errorneo fue enviado a la REST API de Dropbox. Dropbox API error 400*/
        BadOAuthRequest,                /*!< Peticion OAuth erroneo recibida por el servidor (el time stamp ha expirado,
                                             ... etc.). Dropbox API error 403 */
        WrongHttpMethod,                /*!< La peticion a la REST API ha usado un metodo HTTP erroneo. Dropbox API error 405 */
        MaxRequestsExceeded,            /*!< Dropbox API error 503 */
        UserOverQuota,                  /*!< Dropbox API error 507 */
        TokenExpired                    /*!< El token de acceso ha expirado. Dropbox API error 401*/
    };

    /*!
      Este constructor crea una instancia no configurada de SM_QDropbox. La URL del servidor se establece a api.dropbox.com,
      se usa la REST API version 1.0 (por ahora SyncMe solo soporta esta) y el metodo de autenticacion es SM_QDropbox::Plaintext.

      Necesitas establecer la key de la API y el shared secret utilizando setKey(QString key) y setSharedSecret(QString sharedSecret).
     */
    explicit SM_QDropbox(QObject *parent = 0);

    /*!
      Este constructor inicializa SM_QDropbox con una clave y un shared secret. El metodo de autenticacion seleccionado y la URL de
      la API se estableceran tambien.

      \param Clave de la API para SyncMe (la da Dropbox)
      \param sharedSecret El secret de SyncMe (la da Dropbox)
      \param method Metodo de autenticacion
      \param url URL del servidor de la API
      \param parent Objeto padre de SM_QDropbox

     */
    explicit SM_QDropbox(QString key, QString sharedSecret,
                      OAuthMethod method = SM_QDropbox::Plaintext,
                      QString url = "api.dropbox.com", QObject *parent = 0);

    /*!
      Si ocurre un error puedes acceder al ultimo codigo de error usando esta funcion.
     */
    Error  error();

    /*!
      Despues de que ocurra un error obtendras una descripcion del ultimo error usando
      esta funcion.
     */
    QString errorString();

    /*!
      Usa esta funcion si quieres cambiar la URL del servidor de API al que accedes.
      Normalmente es http://api.dropbox.com
     */
    void setApiUrl(QString url);

    /*!
      Devuelve la URL de la API
     */
    QString apiUrl();

    /*!
      Metodo para cambiar el metodo de autenticacion
     */
    void setAuthMethod(OAuthMethod m);

    /*!
      Devuelve el metodo de autenticacion actual
     */
    OAuthMethod authMethod();

    /*!
      Establece la version de la API a utilizar
     */
    void setApiVersion(QString apiversion);

    /*!
      Devuelve la version de la API usada actualmente
     */
    QString apiVersion();

    /*!
      Este metodo establece la key
     */
    void setKey(QString key);

    /*!
      Devuelve la key que se esta utilizando
    */
    QString key();

    /*!
      Este metodo establece el shared secret
     */
    void setSharedSecret(QString sharedSecret);

    /*!
      Devuelve el shared secret
     */
    QString sharedSecret();

    /*!
      Este metodo establece el token
     */
    void setToken(QString t);

    /*!
      Este metodo devuelve el token utilizado actualmente
     */
    QString token();

    /*!
      Este metodo establece el token secret
     */
    void setTokenSecret(QString s);

    /*!
      Este metodo devuelve el token secret que se esta usando actualmente
     */
    QString tokenSecret();

    /*!
      Devuelve la key que esta usando SyncMe ahora mismo
     */
    QString appKey();

    /*!
      Devuelve el shared secret usado actualmente por SyncMe
     */
    QString appSharedSecret();

    /*!
      Esta funcion pide un token de acceso que sera valida para el proceso
      de autenticacion. Cuando se recibe el token se emite la señal requestTokenFinished(...)

      Despues de que se obtiene el token de acceso se continua con la autenticacion pidiendo
      al usuario que autorize a SyncMe.
     */
    int requestToken(bool blocking = false);

    /*!
      Esta funcion es la version bloqueante de requestToken(...)
    */
    bool requestTokenAndWait();

    /*!
      Esta funcion deberia automatizar el proceso de autorizacion
      \warning No soportada por la Dropbox API 1.0, debes usar authorizeLink().
     */
    int authorize(QString mail, QString password);

    /*!
      Devuelve la URL que el usuario tendra que usar para autorizar la conexion
      de SyncMe. Se usara este link en conjunto con QDesktopServices::openUrl(...)
      para abrir un navegador con la URL retornada.
     */
    QUrl authorizeLink();

    /*!
      Esta funcion obtiene un token de acceso de Dropbox
     */
    int requestAccessToken(bool blocking = false);

    /*!
      Esta funcion es la version bloqueante de requestAccessToken(...)
    */
    bool requestAccessTokenAndWait();

    /*!
      Esta funcion devuelve la informacion de la cuenta conectada. Cuando se obtenga la informacion
      se emitira la señal SM_QDropbox::accountInfoReceived()

     */
    void requestAccountInfo(bool blocking = false);

    /*!
      Version bloqueante de requestAccountInfo
     */
    SM_Dropbox_Account requestAccountInfoAndWait();

    /*!
      Metodo para firmar las peticiones realizadas a la API
     */
    QString oAuthSign(QUrl base, QString method = "GET");

    /*!
      Devuelve el metodo de autenticacion
     */
    QString signatureMethodString();

    /*!
      Esta funcion genera un nonce (numero aleatorio para encriptar) y lo
      devuelve como String
     */
    static QString generateNonce(qint32 length);

    /*!
      Obtener los metadatos de un fichero, el servidor de la API respondera con la señal
      SM_QDropbox::metadataReceived()
    */
    void requestMetadata(QString file, bool blocking = false);

    /*!
      Version bloqueante de requestMetadata(...)

      \param file Ruta absoluta del fichero (ejemplo: <i>/dropbox/test.txt</i>)
     */
    SM_Dropbox_FileInfo requestMetadataAndWait(QString file);

    /*!
     * Crea y retorna un link de Dropbox para que el usuario pueda ver un archivo en el navegador
     */
    void requestSharedLink(QString file, bool blocking = false);

    /*!
    * Version bloqueante de requestSharedLink(...)
    */
    QUrl requestSharedLinkAndWait(QString file);

    /*!
      Resetea la bandera de error
    */
    void clearError();

    /*!
      Pide la ultima revision de un fichero, emite la señal SM_QDropbox::revisionsReceived()

      \param max Maximo numero de revisiones requeridas
     */
    void requestRevisions(QString file, int max = 10, bool blocking = false);

    /*!
      Version bloqueante de requestRevisions(...)
     */
    QList<SM_Dropbox_FileInfo> requestRevisionsAndWait(QString file, int max = 10);


    /*!
      Produce una lista de entradas delta. Emite la señal SM_QDropbox::deltaEntriesReceived()
     */
    void requestDelta(QString cursor, QString path_prefix, bool blocking = false);

    /*!
      Version bloqueante de requestDelta(...)
     */
     SM_Dropbox_DeltaResponse requestDeltaAndWait(QString cursor, QString path_prefix);

     /*!
       Provee informacion extra acerca de una peticion, se usa para debugging
     */
     SM_QDropbox_request requestInfo(int rqnr);

     /*!
        Para debugging, guarda las peticiones finalizadas para asi poder obtener informacion
        sobre ellas
     */
     void setSaveFinishedRequests(bool save);

     /*!
        Indica si la informacion sobre las peticiones terminadas es persistente
     */
     bool saveFinishedRequests();

signals:
    /*!
      Señal que se emite cuando ocurre un error
     */
    void errorOccured(SM_QDropbox::Error errorcode);

    /*!
      Emitida cuando expira el token de autenticacion
     */
    void tokenExpired();

    /*!
      Esta señal no se utiliza, pero deberia emitirse cuando se intenta acceder a un fichero que no existe,
      actualmente no se puede acceder a ficheros directamente desde SyncMe (editor in-built)
     */
    void fileNotFound();

    /*!
      SM_QDropbox usa una interfaz basada en operaciones para reaccionar a los mensajes.
      Esta señal se emite cuando una peticion a la API de Dropbox finaliza.
     */
    void operationFinished(int requestnr);

    /*!
      Cuando comienza una operacion se emite esta señal junto con su numero identificador
    */
    void operationStarted(int requestnr);

    /*!
      Señal emitida cuando se obtiene el token
     */
    void requestTokenFinished(QString token, QString secret);

    /*!
      Señal emitida cuando la funcion accessToken finaliza
     */
    void accessTokenFinished(QString token, QString secret);

    /*!
      Emitida cuando cambia el token
     */
    void tokenChanged(QString token, QString secret);

    /*!
      Emitida cuando se recibe la informacion de la cuenta
     */
    void accountInfoReceived(QString accountJson);

    /*!
      Emitida cuando se reciben los metadatos de un fichero o de un directorio
    */
    void metadataReceived(QString metadataJson);

    /*!
      Emitida cuando se recibe el link para compartir
    */
    void sharedLinkReceived(QString sharedLink);

    /*!
      Emitida cuando se reciben las revisiones de un fichero
    */
    void revisionsReceived(QString revisionJson);

    /*!
      Emitida cuando se recibe una respuesta delta
    */
    void deltaReceived(QString deltaJson);

public slots:

private slots:
    void requestFinished(int nr, QNetworkReply* rply);
    void networkReplyFinished(QNetworkReply* rply);

private:
    enum {
        SHA1_DIGEST_LENGTH      = 20,
        SHA1_BLOCK_SIZE         = 64,
        HMAC_BUF_LEN            = 4096
    } ;

    QNetworkAccessManager conManager;

    Error   errorState;
    QString errorText;

    QString _appKey;
    QString _appSharedSecret;

    QUrl        apiurl;
    QString     nonce;
    long        timestamp;
    OAuthMethod oauthMethod;
    QString     _version;

    QString oauthToken;
    QString oauthTokenSecret;

    QMap <QNetworkReply*,int>  replynrMap;
    int  lastreply;
    QMap<int,SM_QDropbox_request> requestMap;
    QMap<int,int> delayMap;

    QString mail;
    QString password;

    // Para funciones bloqueantes
    QEventLoop *_evLoop;
    void startEventLoop();
    void stopEventLoop();

    // Memoria temporal
    SM_Dropbox_Json _tempJson;

    SM_Dropbox_Account _account;

    // Indica cuando las peticiones finalizadas deben ser guardadas para debugging, cuidado con esto ya que el rendimiento decae drasticamente
    bool _saveFinishedRequests;

    QString hmacsha1(QString key, QString baseString);
    void prepareApiUrl();
    int  sendRequest(QUrl request, QString type = "GET", QByteArray postdata = 0, QString host = "");
    void responseTokenRequest(QString response);
    void responseBlockedTokenRequest(QString response);
    int  responseDropboxLogin(QString response, int reqnr);
    void responseAccessToken(QString response);
    void responseBlockingAccessToken(QString response);
    void parseToken(QString response);
    void parseAccountInfo(QString response);
    void parseSharedLink(QString response);
    void checkReleaseEventLoop(int reqnr);
    void parseMetadata(QString response);
    void parseBlockingAccountInfo(QString response);
    void parseBlockingMetadata(QString response);
    void parseBlockingSharedLink(QString response);
    void parseRevisions(QString response);
    void parseBlockingRevisions(QString response);
    void parseDelta(QString response);
    void parseBlockingDelta(QString response);
    void removeRequestFromMap(int rqnr);
};

#endif // SM_QDropbox_H
