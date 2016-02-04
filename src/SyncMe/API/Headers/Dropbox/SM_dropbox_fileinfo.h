#ifndef SM_DROPBOXFILEINFO_H
#define SM_DROPBOXFILEINFO_H

#include <QObject>
#include <QDateTime>
#include <QString>
#include <QList>

#ifdef SM_DROPBOX_DEBUG
#include <QDebug>
#endif

#include "SM_dropbox_json.h"

//! Provee información y metadatos sobre ficheros y directorios
/*!
  Esta clase es un versión más especializada de SM_Dropbox_Json. Provee acceso a
  metadatos de un fichero o directorio que está guardado en Dropbox.

  Para obtener la información de los metadatos de cualquier fichero guardado
  en Dropbox debes usar SM_QDropbox::metadata() o SM_Dropbox_FileInfo::metadata(). Estas
  funciones devuelve una instancia de esta clase que contiene la información requerida.
  Si ocurre un error obteniendo los metadatos la función isValid() devolverá 'false'.


  Navegando por el sistema de ficheros de Dropbox:
  contents(). La función contents() provee los metadatos de todos los ficheros y directorios
  que hay en un directorio. Debido a las limitaciones de la Dropbox REST API estos metadatos
  no contienen contenidos o subdirectorios. Llamando a contents() en un metadato que obtuviste
  haciendo una llamada previa a contents() te devolverá una lista vacía. Tienes que obtener los
  metadatos de un subdirectorio de nuevo usando SM_QDropbox::requestMetadata() o SM_QDropbox::requestMetadataAndWait().


  \bug modified() y clientModified() no funcionan por un problema en la clase
  SM_Dropbox_Json
 */
class SM_DROPBOXSHARED_EXPORT SM_Dropbox_FileInfo : public SM_Dropbox_Json
{
    Q_OBJECT
public:

    /*!
      Crea una instancia vacía de SM_Dropbox_FileInfo.
      \warning Uso interno
      \param parent QObject padre
    */
    SM_Dropbox_FileInfo(QObject *parent = 0);

    /*!
      Crea una instancia de SM_Dropbox_FileInfo basada en los datos que hay en
      el JSON representado como string.

      \param jsonStr Metadatos JSON representado como string
      \param parent Puntero al QObject padre
    */
    SM_Dropbox_FileInfo(QString jsonStr, QObject *parent = 0);

    /*!
       Create una copia de otra instancia de SM_Dropbox_FileInfo.

       \param other Instancia original
     */
    SM_Dropbox_FileInfo(const SM_Dropbox_FileInfo &other);

    /*!
      Destructor por defecto. Se ocupa limpiar el objeto cuando es destruido.
    */
    ~SM_Dropbox_FileInfo();

    /*!
      Copia los valores de otra instancia de SM_Dropbox_FileInfo a la instancia actual
      \param other Instancia original
    */
    void copyFrom(const SM_Dropbox_FileInfo &other);

    /*!
      Funciona como copyFrom() pero como operador.

      \param other Instancia original
    */
    SM_Dropbox_FileInfo& operator=(const SM_Dropbox_FileInfo& other);

    /*!
      Tamaño del fichero en 'formato humano'.
    */
    QString   size() const;

    /*!
      Número de revisión actual.
     */
    quint64   revision() const;

    /*!
      Determina si un icono está disponible (del fichero).
     */
    bool      thumbExists()  const;

    /*!
      Tamaño del fichero en bytes.
     */
    quint64   bytes()  const;

    /*!
      Timestamp de la última modificación.
      \bug No funciona actualmente
     */
    QDateTime modified();

    /*!
      Timestamp de la subida en el cliente de escritorio.
     */
    QDateTime clientModified();

    /*!
      Nombre del icono.
     */
    QString   icon() const;

    /*!
      Directores de la raíz. Pueden ser <i>/dropbox</i> o <i>/sandbox</i>
    */
    QString   root() const;

    /*!
      Ruta canónica completa del fichero.
    */
    QString   path()  const;

    /*!
      Determina si el item seleccionado es un directorio.
    */
    bool      isDir()  const;

    /*!
      Mime-Type del item (https://en.wikipedia.org/wiki/Media_type).
    */
    QString   mimeType()  const;

    /*!
      Indica si el item ha sido eliminado del servidor.
    */
    bool      isDeleted()  const;

    /*!
      Revisión actual como string hash.
    */
    QString   revisionHash()  const;

    /*!
      Devuelve el contenido de un directorio.
      Esta función devolverá una lista de tamaño 0 si el item no
      es un directorio.
    */
    QList<SM_Dropbox_FileInfo> contents() const;

signals:

public slots:

private:
    void dataFromJson();
    void _init();

    QString   _size;
    quint64   _revision;
    bool      _thumbExists;
    quint64   _bytes;
    QDateTime _modified;
    QDateTime _clientModified;
    QString   _icon;
    QString   _root;
    QString   _path;
    bool      _isDir;
    QString   _mimeType;
    bool      _isDeleted;
    QString   _revisionHash;
    QList<SM_Dropbox_FileInfo>* _content;
};

#endif // SM_DROPBOXFILEINFO_H
