#ifndef SM_DROPBOXACCOUNT_H
#define SM_DROPBOXACCOUNT_H

#include <QObject>
#include <QUrl>
#include "SM_dropbox_json.h"

//! Guarda toda la información de la cuenta de un usuario
/*!
  Esta clase se utiliza para guardar la información de una cuenta obtenida usando
  SM_Dropbox::accountInfo(). Los datos guardados corresponden directamente a
  la petición account_info de la API de Dropbox.

  SM_Dropbox_Account interpreta los datos obtenidos en un SM_Dropbox_JSON.
  Si los datos pueden ser interpretados y por lo tanto son válidos, el objeto resultante
  será válido.
  Si ocurre cualquier error interpretando los datos el objeto SM_Dropbox_Account resultante
  será inválido. Esto se puede comprobar usando isValid().

  Ver https://www.dropbox.com/developers/reference/api#account-info para los detalles.

 */
class SM_DROPBOXSHARED_EXPORT SM_Dropbox_Account : public SM_Dropbox_Json
{
    Q_OBJECT
public:
    /*!
      Crea una instancia vacía del objeto. Es automáticaente inválido y no contiene
      ningún dato de utilidad

      \param parent Parent QObject.
     */
    SM_Dropbox_Account(QObject *parent = 0);

    /*!
      Este constructor crea un objeto basado en los datos contenidos en un
      string dado que está en formato JSON

      \param jsonString JSON data in string representation
      \param parent Parent QObject.
     */
    SM_Dropbox_Account(QString jsonString, QObject *parent = 0);

    /*!
      Constructor de copia

      \param other Original SM_Dropbox_Account
     */
    SM_Dropbox_Account(const SM_Dropbox_Account& other);

    /*!
      Devuelve el 'referral link' del usuario
     */
    QUrl    referralLink()  const;

    /*!
      Devuelve el nombre de la cuenta (el nombre público a mostrar).
     */
    QString displayName()  const;

    /*!
      Devuelve el UID de la cuenta
     */
    qint64  uid()  const;

    /*!
      Devuelve el país al que está asociada la cuenta
     */
    QString country()  const;

    /*!
      Devuelve el E-Mail que usa el propietario de la cuenta
     */
    QString email()  const;

    /*!
      Devuelve la cantidad de espacio usado en carpetas compartidas por el usuario (en Bytes)
     */
    quint64  quotaShared()  const;

    /*!
      Devuelve la cantidad total de espacio usado por el usuario (en Bytes)
     */
    quint64  quota()  const;

    /*!
      Devuelve la cantidad de espacio usado FUERA de las carpetas compartidas (en Bytes)
     */
    quint64  quotaNormal()  const;

    /*!
      Sobrecarga del operador = para copiar un objeto SM_Dropbox_Account, internamente
      se llama a copyFrom().
     */
    SM_Dropbox_Account& operator =(SM_Dropbox_Account&);

    /*!
      Esta función se utiliza para copiar los datos de otro objeto SM_Dropbox_Account
     */
    void copyFrom(const SM_Dropbox_Account& a);

private:
    QUrl    _referralLink;
    QString _displayName;
    quint64 _uid;
    QString _country;
    QString _email;
    quint64 _quotaShared;
    quint64 _quota;
    quint64 _quotaNormal;

    void _init();
};

#endif // SM_DROPBOXACCOUNT_H
