#include "SM_SyncMe_Dropbox_Test.h"

typedef QMap<QString, QSharedPointer<SM_Dropbox_FileInfo> > SM_Dropbox_FileInfoMap;

SM_SyncMe_Dropbox_Test::SM_SyncMe_Dropbox_Test()
{
}


/*!
 * Lectura de JSON con String.
 */
void SM_SyncMe_Dropbox_Test::jsonCase1()
{
    SM_Dropbox_Json json("{\"string\":\"asdf\"}");
    QVERIFY2(json.isValid(), "Validez del JSON");
    QVERIFY2(json.getString("string").compare("asdf") == 0, "Los String no coinciden");
}

/*!
 * Lectura de JSON con enteros.
 */
void SM_SyncMe_Dropbox_Test::jsonCase2()
{
    SM_Dropbox_Json json("{\"int\":1234}");
    QVERIFY2(json.isValid(), "Validez del JSON");
    QVERIFY2(json.getInt("int") == 1234, "Los enteros no coinciden");
}

/*!
 * Test de validez/invalidez de JSON
 */
void SM_SyncMe_Dropbox_Test::jsonCase3()
{
    SM_Dropbox_Json json("{\"test\":\"foo\"");
    QVERIFY2(!json.isValid(), "Invalidez de JSON comprobada correctamente");
}

/*!
 * Lectura de JSON con booleanos.
 */
void SM_SyncMe_Dropbox_Test::jsonCase4()
{
    SM_Dropbox_Json json("{\"bool\":true}");
    QVERIFY2(json.isValid(), "Validez del JSON");
    QVERIFY2(json.getBool("bool"), "Los booleanos no coinciden");
}

/*!
 * Lectura de JSON con flotantes.
 */
void SM_SyncMe_Dropbox_Test::jsonCase5()
{
    SM_Dropbox_Json json("{\"double\":14.323667}");
    QVERIFY2(json.isValid(), "Validez del JSON");
    QVERIFY2(json.getDouble("double"), "Los Double no coinciden");
}

/*!
 * Lectura de subjson
 */
void SM_SyncMe_Dropbox_Test::jsonCase6()
{
    SM_Dropbox_Json json("{\"json\": {\"string\":\"abcd\"}}");
    QVERIFY2(json.isValid(), "Validez del JSON");

    SM_Dropbox_Json* subjson = json.getJson("json");

    QVERIFY2(subjson!=NULL, "El subjson esta vacio");
    QVERIFY2(subjson->isValid(), "Subjson invalido");
}

/*!
 * Lectura de JSON con enteros sin signo.
 */
void SM_SyncMe_Dropbox_Test::jsonCase7()
{
    SM_Dropbox_Json json("{\"uint\":4294967295}");
    QVERIFY2(json.isValid(), "Validez del JSON");
    QVERIFY2(json.getUInt("uint") == 4294967295, "Los enteros sin signo no coinciden");
}

/**
 * Comprueba si clear() funciona correctamente
 */
void SM_SyncMe_Dropbox_Test::jsonCase8()
{
    SM_Dropbox_Json json("{\"uint\":4294967295}");
    QVERIFY2(json.isValid(), "Validez del JSON");
    json.clear();
    QVERIFY2(json.getUInt("uint") == 0, "Lista interna no limpiada");
    QVERIFY2(json.strContent().isEmpty(), "El String JSON no ha sido limpiado");
}

/**
 * Comprueba si la interpretacion en array y el acceso es correcto
 */
void SM_SyncMe_Dropbox_Test::jsonCase9()
{
    SM_Dropbox_Json json("{\"array\": [1, \"test\", true, 7.3]}");
    QVERIFY2(json.isValid(), "Validez del JSON");

    QStringList l = json.getArray("array");
    QVERIFY2(l.size() == 4, "El array tiene un size erroneo");
    QVERIFY2(l.at(0).compare("1") == 0, "Elemento entero mal formateado");
    QVERIFY2(l.at(1).compare("test") == 0, "Elemento String mal formateado");
    QVERIFY2(l.at(2).compare("true") == 0, "Elemento booleano mal formateado");
    QVERIFY2(l.at(3).compare("7.3") == 0, "Elemento double mal formateado");
}

/**
 * Comprueba que un JSON como array es accesible
 */
void SM_SyncMe_Dropbox_Test::jsonCase10()
{
    SM_Dropbox_Json json("{\"jsonarray\":[{\"key\":\"value\"}]}");
    QVERIFY2(json.isValid(), "Validez del JSON");

    QStringList l = json.getArray("jsonarray");
    QVERIFY2(l.size() == 1, "El array tiene un size erroneo");

    SM_Dropbox_Json arrayJson(l.at(0));
    QVERIFY2(arrayJson.isValid(), "El JSON desde el array es invalido");
    QVERIFY2(arrayJson.getString("key").compare("value") == 0, "El JSON desde el array contiene un valor erroneo");
}

/**
 * Comprueba si compare() funciona bien haciendo una auto-comparacion
 */
void SM_SyncMe_Dropbox_Test::jsonCase11()
{
    QString jsonStr = "{\"int\": 1, \"string\": \"test\", \"bool\": true, \"json\": {\"key\": \"value\"}, "
                      "\"array\": [1, 3.5, {\"arraykey\": \"arrayvalue\"}]}";
    SM_Dropbox_Json json(jsonStr);
    QVERIFY2(json.isValid(), "Validez del JSON");
    QVERIFY2(json.compare(json) == 0, "Comparando el mismo json dio en un resultado negativo");
}

/**
 * Este test crea un JSON y otro JSON que se basa en el valor devuelto por strContent()
 * del primer JSON. Ambos JSON son comparados deespues y esperamos que sean iguales.
 */
void SM_SyncMe_Dropbox_Test::jsonCase12()
{
    QString jsonStr = "{\"int\": 1, \"string\": \"test\", \"bool\": true, \"json\": {\"key\": \"value\"}, "
                      "\"array\": [1, 3.5, {\"arraykey\": \"arrayvalue\"}], \"timestamp\": \"Sat, 21 Aug 2010 22:31:20 +0000\"}";
    SM_Dropbox_Json json(jsonStr);
    QVERIFY2(json.isValid(), "Validez del JSON");

    QString jsonContent = json.strContent();
    SM_Dropbox_Json json2(jsonContent);
    QString j2c = json2.strContent();

    int compare = json.compare(json2);

    QVERIFY2(compare == 0, "El contenido del string es incorrecto");
}

/**
 * Este test verifica que los setters y los getters funcionan correctamente
 */
void SM_SyncMe_Dropbox_Test::jsonCase13()
{
    SM_Dropbox_Json json;
    json.setInt("testInt", 10);
    QVERIFY2(json.getInt("testInt") == 10, "setInt de JSON es incorrecto");

    json.setUInt("testUInt", 10);
    QVERIFY2(json.getUInt("testUInt") == 10, "setUInt de JSON es incorrecto");

    json.setDouble("testDouble", 10.0);
    QVERIFY2(json.getDouble("testDouble") == 10.0, "setDouble de JSON es incorrecto");

    json.setBool("testBool", true);
    QVERIFY2(json.getBool("testBool"), "setBool de JSON es incorrecto");

    json.setString("testString", "10");
    QVERIFY2(json.getString("testString").compare("10"), "setString de JSON es incorrecto");

    QDateTime time = QDateTime::currentDateTime();
    json.setTimestamp("testTimestamp", time);
    QVERIFY2(json.getTimestamp("testTimestamp").daysTo(time) == 0, "setTimestamp de JSON es incorrecto");
}

/**
 * Este test verifica que las [ ] se parsean correctamente en JSON
 */
void SM_SyncMe_Dropbox_Test::jsonCase14()
{
    SM_Dropbox_Json json("{\"string\": \"[asdf]abcd\"}");
    QVERIFY2(json.isValid(), "El JSON no ha podido ser parseado");
    QVERIFY2(json.getString("string").compare("[asdf]abcd") == 0, "Las [ ] no se parsean correctamente");
}

/**
 * Este test verifica que las { } se parsean correctamente en JSON
 */
void SM_SyncMe_Dropbox_Test::jsonCase15()
{
    SM_Dropbox_Json json("{\"string\": \"{asdf}abcd\"}");
    QVERIFY2(json.isValid(), "El JSON no ha podido ser parseado");
    QVERIFY2(json.getString("string").compare("{asdf}abcd") == 0,
         QString("Las { } no se parsean correctamente [%1]").arg(json.getString("string")).toStdString().c_str());
}

/**
 * Este test conecta a Dropbox y envia un esqueleto de peticion para comprobar
 * la conexion con texto plano. La peticion no se procesa obviamente...
 * <b>Debes autorizar a SyncMe en tu cuenta de Dropbox para poder utilizar este test!</b>
 */
void SM_SyncMe_Dropbox_Test::dropboxCase1()
{
    SM_QDropbox dropbox(APP_KEY, APP_SECRET);
    QVERIFY2(connectDropbox(&dropbox, SM_QDropbox::Plaintext), "Error de conexion");
    SM_Dropbox_Account accInf = dropbox.requestAccountInfoAndWait();
    QVERIFY2(dropbox.error() == SM_QDropbox::NoError, "Error en la peticion");
    return;
}

/**
 * Este test conecta a Dropbox y prueba la API de llamadas delta.
 *
 * <b>Debes autorizar a SyncMe en tu cuenta de Dropbox para poder utilizar este test!</b>
 */
void SM_SyncMe_Dropbox_Test::dropboxCase2()
{
    QTextStream strout(stdout);
    SM_QDropbox dropbox(APP_KEY, APP_SECRET);
    QVERIFY2(connectDropbox(&dropbox, SM_QDropbox::Plaintext), "Error en la conexion");

    QString cursor = "";
    bool hasMore = true;
    SM_Dropbox_FileInfoMap file_cache;

    strout << "Pidiendo delta...\n";
    do
    {
        SM_Dropbox_DeltaResponse r = dropbox.requestDeltaAndWait(cursor, "");
        cursor = r.getNextCursor();
        hasMore = r.hasMore();

        const SM_Dropbox_DeltaEntryMap entries = r.getEntries();
        for(SM_Dropbox_DeltaEntryMap::const_iterator i = entries.begin(); i != entries.end(); i++)
        {
            if(i.value().isNull())
            {
                file_cache.remove(i.key());
            }
            else
            {
                strout << "insertando fichero " << i.key() << "\n";
                file_cache.insert(i.key(), i.value());
            }
        }

    } while (hasMore);
    strout << "cursor del siguiente apuntando a: " << cursor << "\n";
    for(SM_Dropbox_FileInfoMap::const_iterator i = file_cache.begin(); i != file_cache.end(); i++)
    {
        strout << "fichero " << i.key() << " modificado por ultima vez " << i.value()->clientModified().toString() << "\n";
    }

    return;
}

/**
 * Pedir autorizacion al usuario.
 */
void SM_SyncMe_Dropbox_Test::authorizeApplication(SM_QDropbox* d)
{
    QTextStream strout(stdout);
    QTextStream strin(stdin);

    strout << "##########################################" << endl;
    strout << "# Debes dar acceso a los Tests de SyncMe #" << endl;
    strout << "# a tu cuenta de Dropbox!                #" << endl;
    strout << "#                                        #" << endl;
    strout << "# Ve a la siguiente URL para hacerlo     #" << endl;
    strout << "##########################################" << endl << endl;

    strout << "URL: " << d->authorizeLink().toString() << endl;
    QDesktopServices::openUrl(d->authorizeLink());
    strout << "Esperando hasta que autorices a SyncMe!";
    strout << endl;

    while(d->requestAccessTokenAndWait() == false)
    {
        QThread::msleep(1000);
    }
}

/**
 * Conecta un SM_QDropbox al servicio de Dropbox
 * @param d Objeto SM_QDropbox a conectar
 * @param m Metodo de autenticacion
 * @return <code>true</code> en caso de ser satisfactorio
 */
bool SM_SyncMe_Dropbox_Test::connectDropbox(SM_QDropbox *d, SM_QDropbox::OAuthMethod m)
{
    QFile tokenFile("tokens");

    if(tokenFile.exists()) // Reutilizar tokens anteriores
    {
        if(tokenFile.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QTextStream instream(&tokenFile);
            QString token = instream.readLine().trimmed();
            QString secret = instream.readLine().trimmed();
            if(!token.isEmpty() && !secret.isEmpty())
            {
                d->setToken(token);
                d->setTokenSecret(secret);
                tokenFile.close();
                return true;
            }
        }
        tokenFile.close();
    }

    // Adquirir nuevo token
    if(!d->requestTokenAndWait())
    {
        qCritical() << "Error al pedir un token";
        return false;
    }

    d->setAuthMethod(m);
    if(!d->requestAccessTokenAndWait())
    {
        int i = 0;
        for(;i<3; ++i) // Probamos 3 veces
        {
            if(d->error() != SM_QDropbox::TokenExpired)
                break;
            authorizeApplication(d);
        }

       if(i>3)
       {
           qCritical() <<  "Demasiados intentos para la autorizacion";
           return false;
       }

        if(d->error() != SM_QDropbox::NoError)
        {
           qCritical() << "Error: " << d->error() << " - " << d->errorString() << endl;
           return false;
        }
    }

    if(!tokenFile.open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text))
        return true;

    QTextStream outstream(&tokenFile);
    outstream << d->token() << endl;
    outstream << d->tokenSecret() << endl;
    tokenFile.close();
    return true;
}

QTEST_MAIN(SM_SyncMe_Dropbox_Test)
