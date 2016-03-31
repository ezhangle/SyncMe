#ifndef SM_DROPBOXDELTARESPONSE_H
#define SM_DROPBOXDELTARESPONSE_H

#include <QObject>
#include <QIODevice>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QEvent>
#include <QSharedPointer>

#include "SM_dropbox_global.h"
#include "SM_dropbox_json.h"
#include "SM_dropbox_fileinfo.h"

//! Tipo para mapear desde las rutas de un fichero a los metadatos del fichero.
typedef QMap<QString, QSharedPointer<SM_Dropbox_FileInfo> > SM_Dropbox_DeltaEntryMap;

//! Respuesta de una llamada delta.
/*!
  Esta estructura se usa para llevar la respuesta (multi-parte) de una llamada delta a la API.
 */
class SM_Dropbox_DeltaResponse
{
public:
    //! Construye un objeto SM_Dropbox_DeltaResponse limpio.
    SM_Dropbox_DeltaResponse();

    //! Construye un objeto SM_Dropbox_DeltaResponse desde una respuesta JSON.
    SM_Dropbox_DeltaResponse(QString response);

    //! Obtiene el mapeo string-a-metadatos.
    /*!
      Esto es un mapeo de las rutas del fichero a entradas de metadatos (SM_Dropbox_FileInfo)

      \note Los valores en el mapeo pueden tener objetos QSharedPointer que sean 'null',
            que representan entradas que deberían ser borradas del seguimiento local.
     */
    const SM_Dropbox_DeltaEntryMap getEntries() const;

    //! Determina si el mecanismo local de seguimiento debería borrar su estado actual.
    bool shouldReset() const;

    //! Devuelve el cursor que debería ser pasado a la siguiente llamada delta a la API.
    QString getNextCursor() const;

    //! Determina si la siguiente llamada delta a la API es parte de la misma respuesta.
    /*!
      \return si es true: hace una llamada delta a la API con el mismo cursor y lo trata como
              parte de la misma respuesta;
              si es false: espera un tiempo (5 minutos por ej.) bantes de hacer otra llamada delta.
      */
    bool hasMore() const;


private:
    SM_Dropbox_DeltaEntryMap _entries;
    bool _reset;
    QString _cursor;
    bool _has_more;

    void _init();
};

#endif // SM_DROPBOXDELTARESPONSE_H
