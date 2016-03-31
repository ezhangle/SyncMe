#ifndef SM_DROPBOXFILE_H
#define SM_DROPBOXFILE_H

#include <QObject>
#include <QIODevice>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QEvent>

#include "SM_dropbox_global.h"
#include "SM_dropbox_json.h"
#include "SM_qdropbox.h"
#include "SM_dropbox_fileinfo.h"

const QString QDROPBOX_FILE_CONTENT_URL = "https://api-content.dropbox.com";

//! Permite acceso a los ficheros guardados en Dropbox
/*!
  SM_Dropbox_File permite acceder a los ficheros guardados en Dropbox. Puedes
  usar esta clase como cualquier QIODevice, muy similar a la clase QFile por defecto.
  Es posible utilizarla junto con QTextStream y QDataStream para acceder a los contenidos
  de los ficheros.

  Cuando accedes a ficheros en Dropbox recuerda usar rutas válidas. Tales rutas
  empiezan con /dropbox/ o /sandbox/ dependiendo del nivel de acceso de la aplicación.

  Es importante saber que SM_Dropbox_File guarda los contenidos del fichero remoto de forma
  local usando open(). Esto significa que el contenido del fichero no es automáticamente
  actualizado si éste cambia en el servidor de Dropbox, lo que significa que puede que no
  siempre tengas la versión más actualizada de los contenidos del fichero.

  \todo Implementar utilidades para acceder a la revisión (obtener la lista de revisiones y la actual
        revisión)

 */
class SM_DROPBOXSHARED_EXPORT SM_Dropbox_File : public QIODevice
{
    Q_OBJECT
public:
    /*!
      Constructor por defecto. Usa setApi() y setFilename() para acceder a Dropbox.

      \param parent QObject padre
     */
    SM_Dropbox_File(QObject* parent = 0);

    /*!
      Crea una instancia de SM_Dropbox_File que puede conectar Dropbox si el SM_QDropbox
      que le pasas ya está conectado. Usa setFilename() antes dee intentar acceder a algún fichero.

      \param api Puntero al SM_QDropbox que está conectado a la cuenta.
      \param parent QObject padre
     */
    SM_Dropbox_File(SM_QDropbox* api, QObject* parent = 0);

    /*!
      Crea una instancia de SM_Dropbox_File que puede acceder a un fichero en Dropbox.

      \param filename Ruta de Dropbox al fichero que quieres acceder.
      \param api SM_QDropbox que está conectado a la cuenta del usuario.
      \param parent QObject padre
     */
    SM_Dropbox_File(QString filename, SM_QDropbox* api, QObject* parent = 0);

    /*!
      Este destructor limpia todo cuando el objeto es destruido.
     */
    ~SM_Dropbox_File();

    /*!
      SM_Dropbox_File está implementado actualmente como un 'dispositivo' secuencial. Se intentará
      en un futuro sacar una versión con capacidad de procesamiento paralelo (multihilo).
     */
    bool isSequential() const;

    /*!
      Obtiene el contenido del fichero del servidor de Dropbox y lo guarda localmente.
      Dependiendo en el OpenMode se tendrá acceso de lectura o de escritura.

      \param mode El modo de acceso al fichero. Equivalente a QIODevice.
     */
    bool open(OpenMode mode);

    /*!
      Cierra el buffer del fichero. Si el fichero estaba abierto con QIODevice::WriteOnly (o
      QIODevice::ReadWrite) el buffer que contiene los contenidos del fichero será limpiado y escrito
      en el fichero.
     */
    void close();

    /*!
      Setea la instancia de SM_QDropbox que es usada para acceder a Dropbox.

      \param dropbox Puntero al objeto SM_QDropbox
     */
    void setApi(SM_QDropbox* dropbox);

    /*!
      Devuelve un puntero a la instancia SM_QDropbox que es usada para conectar a Dropbox.
     */
    SM_QDropbox* api();

    /*!
      Setea el nombre del fichero al que quieres acceder. Recuerda usar una ruta de Dropbox correcta
      que empiece por /dropbox/ o /sandbox/.

      \param filename Ruta del fichero.
     */
    void setFilename(QString filename);

    /*!
      Devuelve la ruta del fichero que está siendo accedido por esta instancia.
     */
    QString filename();

    /*!
      Escribe el contenido del buffer en el fichero (sólo si el fichero está abierto
      con permisos de escritura; write mode).
     */
    bool flush();

    /*!
      Reimplementado de QIODevice
     */
    bool event(QEvent* event);

    /*!
      A veces el contenido del fichero es limpiado automáticamente cuando el buffer
      interno tiene más de 1024 nuevos bytes o cuando usas close(). Si quieres que el
      SM_Dropbox_File se limpie automáticamente antes de esos 1024 bytes, puedes usar
      esta función y así reducir este umbral de tiempo.

      \param num SM_Dropbox_File limpiará automáticamente el buffer del fichero cuando haya más de 'num'
                 nuevos bytes de datos.
     */
    void setFlushThreshold(qint64 num);

    /*!
      Devuelve el actual umbral de tiempo para limpiar el buffer.
     */
    qint64 flushThreshold();

    /*!
      Por defecto un fichero ya existente será reemplazado. Si no quieres que pase esto
      puedes usar está función para poner la bandera de overwrite a 'false'. Si un fichero
      con el mismo nombre ya existe este será renombrado automáticamente por Dropbox
      a algo como "fichero (1).txt".

      \param overwrite Bandera de reemplazo
     */
    void setOverwrite(bool overwrite);

    /*!
      Devuelve el estado actual de la bandera de reemplazo.
     */
    bool overwrite();

    /*!
      Devuelve los metadatos del fichero como un objeto SM_Dropbox_FileInfo.
    */
    SM_Dropbox_FileInfo metadata();

    /*!
      Verifica si el fichero ha cambiado en Dropbox mientras estaba abierto localmente.
      Esta función devolverá 'false' si el fichero no ha sido abierto previamente y un error
      ocurrió durante la obtención de los metadatos del fichero. De todas formas es más seguro
      abrir el fichero primero y luego comprobar con hasChanged().

      \returns 'true' si el fichero ha cambiado o 'false' si no ha cambiado.
    */
    bool hasChanged();

    /*!
      Obtiene y devuelve todas las revisiones disponibles del fichero.
      \param max Cuando se defina, la función sólo listará hasta 'max' cantidad de revisiones.
      \returns Lista de las últimas revisiones del fichero.
    */
    QList<SM_Dropbox_FileInfo> revisions(int max = 10);

    /*!
      Reimplementado de QIODevice::seek().
      Mira en la posición (byte) dada en el fichero. Al contrario que QFile::seek() esta función
      no mirá más allá del final del fichero. Cuando busca más allá del fichero de un fichero está función
      para después del último byte del contenido actual y devuelve 'false'.
    */
    bool seek(qint64 pos);

    /*!
      Reimplementado de QIODevice::pos().
      Devuelve la posición actual en el fichero.
    */
    qint64 pos() const;

    /*!
      Reimplementado de QIODevice::reset().
      Busca hasta el principio del fichero. Ver seek().
    */
    bool reset();

public slots:
    void abort();

signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void uploadProgress(qint64 bytesReceived, qint64 bytesTotal);

    void operationAborted();

protected:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

private slots:
    void networkRequestFinished(QNetworkReply* rply);

private:
    QNetworkAccessManager _conManager;

    QByteArray *_buffer;

    QString _token;
    QString _tokenSecret;
    QString _filename;

    SM_QDropbox *_api;


    enum WaitState{
        notWaiting,
        waitForRead,
        waitForWrite
    };

    WaitState _waitMode;

    QEventLoop* _evLoop;

    int     lastErrorCode;
    QString lastErrorMessage;

    qint64 _bufferThreshold;
    qint64 _currentThreshold;

    bool _overwrite;

    int _position;

    SM_Dropbox_FileInfo *_metadata;

    void obtainToken();
    void connectSignals();

    bool isMode(QIODevice::OpenMode mode);
    bool getFileContent(QString filename);
    void rplyFileContent(QNetworkReply* rply);
    void rplyFileWrite(QNetworkReply* rply);
    void startEventLoop();
    void stopEventLoop();
    bool putFile();
    void obtainMetadata();

    void _init(SM_QDropbox *api, QString filename, qint64 bufferTh);
};

#endif // SM_DROPBOXFILE_H
