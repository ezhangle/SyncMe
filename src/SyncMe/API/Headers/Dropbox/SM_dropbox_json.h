#ifndef SM_DROPBOXJSON_H
#define SM_DROPBOXJSON_H

#include "SM_dropbox_global.h"

#include <QObject>
#include <QMap>
#include <QList>
#include <QDateTime>
#include <QStringList>

#ifdef SM_DROPBOX_DEBUG
#include <QDebug>
#endif

typedef char sm_dropbox_json_entry_type;

const sm_dropbox_json_entry_type SM_DROPBOX_JSON_NUM          = 'N';
const sm_dropbox_json_entry_type SM_DROPBOX_JSON_TYPE_STR     = 'S';
const sm_dropbox_json_entry_type SM_DROPBOX_JSON_TYPE_JSON    = 'J';
const sm_dropbox_json_entry_type SM_DROPBOX_JSON_TYPE_ARRAY   = 'A';
const sm_dropbox_json_entry_type SM_DROPBOX_JSON_TYPE_FLOAT   = 'F';
const sm_dropbox_json_entry_type SM_DROPBOX_JSON_TYPE_BOOL    = 'B';
const sm_dropbox_json_entry_type SM_DROPBOX_JSON_TYPE_UINT    = 'U';
const sm_dropbox_json_entry_type SM_DROPBOX_JSON_TYPE_UNKNOWN = '?';

class SM_Dropbox_Json;

//! Guarda los valores de un JSON
union sm_dropbox_json_value{
    SM_Dropbox_Json  *json; //!< Usado para guardar subjsons (Un JSON en un JSON)
    QString       *value; //!< Usado para guardar un valor real, todos los valores se convierten desde QString
};

//! Guarda las claves de un JSON
struct sm_dropbox_json_entry{
    sm_dropbox_json_entry_type type; //!< Datatype del valor
    sm_dropbox_json_value      value; //!< Referencia al struct del valor
};

//! Usado para guardar un JSON que se devuelve desde Dropbox.
/*!
  Casi toda la comunicacion con Dropbox se gestiona usando estructuras de datos JSON.
  JSON es originalmente un metodo de descripcion de datos complejos usado por JavaScript y PHP
  y por lo tanto esta diseñado para trabajar con lenguajes sin 'tipos' de dato. SM_Dropbox_Json
  provee una interfaz que mapea los distintos tipos de valores de JSON a tipos de dato nativos de C++ de
  la mejor manera posible.

  Un JSON se suele pasar como un string y se puede parsear pasando ese string al constructor o
  usando parseString(). Si algún error ocurre la instancia de SM_Dropbox_Json será marcada como
  inválida (ver isValid()).

  Los datos de una instancia válida de SM_Dropbox_Json pueden ser accedidos usando uno de los getters. Si
  el valor que quieres acceder no está mapeado al tipo de dato que has solicitado se devolverá un valor vacío.
  Siempre puedes poner una bandera de forzado de tipo, si lo haces el valor devuelto se convertirá pero
  puede devolver datos sin sentido. Usa esta bandera con cuidado y sólo si sabes lo que estás haciendo.

  \warning Los arrays en JSON no están soportados.
  \todo Implementar los setters y toString() para la generación de JSON (no necesario pero sería interesante)
 */
class SM_DROPBOXSHARED_EXPORT SM_Dropbox_Json : public QObject
{
    Q_OBJECT
public:
    /*!
      Crea un objeto JSON vacío.

      \param parent Puntero al QObject padre.
     */
    SM_Dropbox_Json(QObject *parent = 0);

    /*!
      Este constructor interpreta el string dado como un JSON.

      \param strJson JSON como string.
      \param parent QObject padre.
     */
    SM_Dropbox_Json(QString strJson, QObject *parent = 0);

    /*!
      Copia los datos de otro SM_Dropbox_Json.

      \param other El SM_Dropbox_Json que será copiado.
     */
    SM_Dropbox_Json(const SM_Dropbox_Json &other);

    /*!
      Limpia el JSON al destruir.
     */
    ~SM_Dropbox_Json();

    /*!
      El enum se usa para categorizar los tipos de dato de los valores JSON.
     */
    enum DataType{
        NumberType, //!< Tipo basado en número (interpretado como qint64)
        StringType, //!< Tipo basado en string de longitud variable
        JsonType,   //!< Un subjson
        ArrayType,  //!< Tipo de dato Array (No soportado actualmente)
        FloatType,  //!< Tipo de dato basado en Punto Flotante
        BoolType,   //!< Tipo de dato Booleano
        UnsignedIntType, //!< Tipo de dato unsigned (sólo se usa si NumberType no coincide)
        UnknownType //!< Tipo de dato no identificado
    };

    /*!
      Interpreta un string como JSON - o al menos lo intenta. Si esto no es posible
      la instancia SM_Dropbox_Json será marcada como inválida.

      \parem strJson JSON representado como String.
     */
    void parseString(QString strJson);

    /*!
      Borra todos los datos JSON que estaban guardados.
     */
    void clear();

    /*!
      Devuelve 'true' si la instancia de SM_Dropbox_Json contiene datos válidos de un JSON. Si ocurre
      un error durante el parseo del string está función devolverá 'false'.
     */
    bool isValid();

    /*!
      Devuelve 'true' si la instancia de SM_Dropbox_Json contiene la clave dada.

      \param key La clave solicitada.
     */
    bool hasKey(QString key);

    /*!
      Devuelve el tipo de dato del valor mapeado a la clave
      \param key La clave a ser comprobada.
     */
    DataType type(QString key);

    /*!
      Devuelve un int guardado identificado por la clave dada. Si la clave
      no se mapea correctamente se devuelve un 0. Si la bandera de forzado se
      activa, la comprobación de tipo de dato se omite y se intenta convertir los valores
      sin importar el tipo real de dato.
     */
    qint64 getInt(QString key, bool force = false);

    void setInt(QString key, qint64 value);

    /*!
      Devuelve un unsigned guardado identificado por la clave dada. Si la clave
      no se mapea correctamente se devuelve un 0. Si la bandera de forzado se
      activa, la comprobación de tipo de dato se omite y se intenta convertir los valores
      sin importar el tipo real de dato.
     */
    quint64       getUInt(QString key, bool force = false);

    void setUInt(QString key, quint64 value);

    /*!
      Devuelve un string guardado identificado por la clave dada. Si la clave
      no se mapea correctamente se devuelve un 0. Si la bandera de forzado se
      activa, la comprobación de tipo de dato se omite y se intenta convertir los valores
      sin importar el tipo real de dato.
     */
    QString       getString(QString key, bool force = false);

    void setString(QString key, QString value);

    /*!
      Devuelve un sub JSON identificado por la clave dada. Si la clave no mapea a un
      JSON se devuelve un puntero NULL. No es posible forzar la conversión.
     */
    SM_Dropbox_Json *getJson(QString key);

    void setJson(QString key, SM_Dropbox_Json value);

    /*!
      Devuelve un float guardado identificado por la clave dada. Si la clave
      no se mapea correctamente se devuelve un 0.0. Si la bandera de forzado se
      activa, la comprobación de tipo de dato se omite y se intenta convertir los valores
      sin importar el tipo real de dato.
     */
    double        getDouble(QString key, bool force = false);

    void setDouble(QString key, double value);

    /*!
      Devuelve un booleano guardado identificado por la clave dada. Si la clave
      no se mapea correctamente se devuelve un 'false'. Si la bandera de forzado se
      activa, la comprobación de tipo de dato se omite y se intenta convertir los valores
      sin importar el tipo real de dato.
     */
    bool          getBool(QString key, bool force = false);

    void setBool(QString key, bool value);

    /*!
      Devuelve la representación string del JSON
     */
    QString strContent() const;

    /*!
      Se devuelven los valores de un string como QDateTime (tiempo/timestamp). El timestamp será inválido
      si el string no se pudo convertir.
    */
    QDateTime getTimestamp(QString key, bool force = false);

    void setTimestamp(QString key, QDateTime value);

    /*!
      Devuelve un Array como una lista de items string. Si la clave no existe o no está
      guardado como Array, la función devolverá una lista vacía. Si necesitas los items en un
      tipo especifico de dato debes hacer el 'cast' de tipo equivalente.
    */
    QStringList getArray(QString key, bool force = false);

    /*!
      Devuelve el contenido de un Array como una lista de items string, si el JSON contiene un
      array anónimo (ver isAnonymousArray()). Como con getArray(QString key, bool force = false)
      también tienes que parsear el contenido del Array tú mismo.
      Returns the content of a stored array as a list of string items <i>if the JSON contains
    */
    QStringList getArray();

    /**!
      Sobrecarga de operador para copiar un objeto SM_Dropbox_Json.
    */
    SM_Dropbox_Json& operator =(SM_Dropbox_Json&);

    /**!
      UN JSON puede ser un Array anónimo como este:
      \code
      [
        "a": "valueA",
        "b": "valueB"
      ]
      \endcode

      Con esta función identificar si un array JSON es anónimo.
      \returns <code>true</code> si el JSON es un array anónimo.
    */
    bool isAnonymousArray();

    /**!
      Compara dos objetos JSON para ver si son iguales.
      Esto significa que tienen las mismas claves y con los mismos valores.

      \param other El JSON con el que quieres comprobar
      \returns 0 si los objetos JSON son iguales
    */
    int compare(const SM_Dropbox_Json& other);

protected:
        bool valid;

private:
    QMap<QString, sm_dropbox_json_entry> valueMap;
    bool _anonymousArray;

    void emptyList();
    sm_dropbox_json_entry_type interpretType(QString value);
    int parseSubJson(QString str, int start, sm_dropbox_json_entry *jsonEntry);
    void _init();
};

#endif // SM_DROPBOXJSON_H
