#include <QLocale>

#include "../../Headers/Dropbox/SM_dropbox_json.h"

SM_Dropbox_Json::SM_Dropbox_Json(QObject *parent) :
    QObject(parent)
{
    _init();
}

SM_Dropbox_Json::SM_Dropbox_Json(QString strJson, QObject *parent) :
    QObject(parent)
{
    _init();
    parseString(strJson);
}

SM_Dropbox_Json::SM_Dropbox_Json(const SM_Dropbox_Json &other) :
    QObject(other.parent())
{
    _init();
    parseString(other.strContent());
}

SM_Dropbox_Json::~SM_Dropbox_Json()
{
    emptyList();
}

void SM_Dropbox_Json::_init()
{
    valid          = false;
    _anonymousArray = false;
}

void SM_Dropbox_Json::parseString(QString strJson)
{
#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "parse string = " << strJson << endl;
#endif

    // clear all existing data
    emptyList();

    // basically a json is valid until it is invalidated
    valid = true;

    if(!strJson.startsWith("{") ||
            !strJson.endsWith("}"))
    {
#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "string does not start with { " << endl;
#endif

        if(strJson.startsWith("[") && strJson.endsWith("]"))
        {
#ifdef SM_QTDROPBOX_DEBUG
            qDebug() << "JSON is anonymous array" << endl;
#endif
            _anonymousArray = true;
            // fix json to be parseable by the algorithm below
            strJson = "{\"_anonArray\":"+strJson+"}";
        }
        else
        {
            valid = false;
            return;
        }
    }

    QString buffer   = "";
    QString key      = "";
    QString value    = "";

    bool isKey       = true;
    bool insertValue = false;
    bool isJson      = false;
    bool isArray     = false;
    bool openQuotes  = false;


    for(int i=0; i<strJson.size(); ++i)
    {
        switch(strJson.at(i).toLatin1())
        {
        case '"':
            if(!isKey)
            {
                buffer += "\"";
                openQuotes = !openQuotes;
            }
            continue;
            break;
        case ' ':
            if(!isKey)
                buffer += " ";
            continue;
            break;
        case '}':
        if(openQuotes)
          buffer += "}";
            continue;
            break;
        case ':':
            if(!isKey)
            {
                buffer += ":";
                continue;
            }
#ifdef SM_QTDROPBOX_DEBUG
            qDebug() << "key = " << buffer << endl;
#endif

            key    = buffer.trimmed();
            buffer = "";
            isKey  = false;
            break;
        case ',':
            if(openQuotes)
            {
                buffer += ',';
                continue;
            }
#ifdef SM_QTDROPBOX_DEBUG
            qDebug() << "value = " << buffer << endl;
#endif
            value       = buffer.trimmed();
            buffer      = "";
            isKey       = true;
            insertValue = true;
            break;
        case '{':
            if(i == 0)
                continue;
        if(!openQuotes)
              isJson  = true;
            buffer += '{';
            break;
        case '[':
        if(!openQuotes)
              isArray = true;
            buffer += '[';
            break;
        default:
            buffer += strJson.at(i);
            break;
        }

        if(isJson)
        {
            // create new json object with data until }
            sm_dropbox_json_entry e;
            int offset = parseSubJson(strJson, i, &e);
            valueMap[key] = e;
            key = "";

            // ignore next ,
            i = offset+1;
            buffer      = "";
            isJson      = false;
            insertValue = false;
            isKey       = true;
            //continue;
        }

         if(isArray)
         {
             // value is an array value -> parse array
             bool inString    = false;
             bool arrayEnd    = false;
             int arrayDepth = 0;
             int j = i+1;
             buffer = "[";
             for(;!arrayEnd && j<strJson.size();++j)
             {
                 QChar arrC = strJson.at(j);
                 switch(arrC.toLatin1())
                 {
                 case '"':
                     inString = !inString;
                     buffer += arrC;
                     break;
                 case '[':
                     buffer += "[";
                     arrayDepth += 1;
                     break;
                 case ']':
                     buffer += "]";
                     if(!inString)
                     {
                         if(arrayDepth == 0)
                         {
                            arrayEnd = true;
                         }
                         else
                         {
                            arrayDepth -= 1;
                         }
                     }
                     break;
                 case '\\':
                     if(strJson.at(j+1) == '"') // escaped double quote
                     {
                         buffer += "\"";
                         j++;
                     }
                     else
                         buffer += "\\";
                     break;
                 default:
                     buffer += arrC;
                     break;
                 }
             }

             // save array value string (buffer)
             value       = buffer;
             buffer      = "";
             i           = j;
             insertValue = true;
             isArray     = false;
             isKey       = true; // next element is started
         }

        if(insertValue)
        {
#ifdef SM_QTDROPBOX_DEBUG
            qDebug() << "insert value " << key << " with content = " << value.trimmed() << " and type = " << interpretType(value.trimmed()) << endl;
#endif
            sm_dropbox_json_entry e;
            QString *valuePointer = new QString(value.trimmed());
            e.value.value = valuePointer;
            e.type        = interpretType(value.trimmed());
            valueMap[key] = e;

            key   = "";
            value = "";
            insertValue = false;
        }
    }

    // there's some key left
    if(key.compare(""))
    {
#ifdef SM_QTDROPBOX_DEBUG
            qDebug() << "rest value = " << buffer << endl;
#endif
        // but no value in the buffer -> json is invalid because a value is missing
        if(!buffer.compare(""))
        {
            valid = false;
        }
        else
        {
            // if there's a value left we store it
            sm_dropbox_json_entry e;
            e.value.value = new QString(buffer.trimmed());
            e.type        = interpretType(buffer.trimmed());
            valueMap[key] = e;
        }

    }

    return;
}

void SM_Dropbox_Json::clear()
{
    emptyList();
    return;
}

bool SM_Dropbox_Json::isValid()
{
    return valid;
}

bool SM_Dropbox_Json::hasKey(QString key)
{
    return valueMap.contains(key);
}

SM_Dropbox_Json::DataType SM_Dropbox_Json::type(QString key)
{
    if(!valueMap.contains(key))
        return UnknownType;

    sm_dropbox_json_entry_type t;
    t = valueMap.value(key).type;

    switch(t)
    {
    case SM_DROPBOX_JSON_NUM:
        return NumberType;
    case SM_DROPBOX_JSON_TYPE_STR:
        return StringType;
    case SM_DROPBOX_JSON_TYPE_JSON:
        return JsonType;
    case SM_DROPBOX_JSON_TYPE_ARRAY:
        return ArrayType;
    case SM_DROPBOX_JSON_TYPE_FLOAT:
        return FloatType;
    case SM_DROPBOX_JSON_TYPE_BOOL:
        return BoolType;
    case SM_DROPBOX_JSON_TYPE_UINT:
        return UnsignedIntType;
    default:
        return UnknownType;
    }
    return UnknownType;
}

qint64 SM_Dropbox_Json::getInt(QString key, bool force)
{
    if(!valueMap.contains(key))
        return 0;

    sm_dropbox_json_entry e;
    e = valueMap.value(key);

    if(!force && e.type != SM_DROPBOX_JSON_NUM)
        return 0;

    return e.value.value->toInt();
}

void SM_Dropbox_Json::setInt(QString key, qint64 value)
{
    if(valueMap.contains(key)){
        valueMap[key].value.value->setNum(value);
    }else{
        sm_dropbox_json_entry e;
        QString *valuePointer = new QString();
        valuePointer->setNum(value);
        e.value.value = valuePointer;
        e.type        = SM_DROPBOX_JSON_NUM;
        valueMap[key] = e;
    }
}

quint64 SM_Dropbox_Json::getUInt(QString key, bool force)
{
    if(!valueMap.contains(key))
        return 0;

    sm_dropbox_json_entry e;
    e = valueMap.value(key);

    if(!force && e.type != SM_DROPBOX_JSON_TYPE_UINT)
        return 0;

    return e.value.value->toUInt();
}

void SM_Dropbox_Json::setUInt(QString key, quint64 value)
{
    if(valueMap.contains(key)){
        valueMap[key].value.value->setNum(value);
    }else{
        sm_dropbox_json_entry e;
        QString *valuePointer = new QString();
        valuePointer->setNum(value);
        e.value.value = valuePointer;
        e.type        = SM_DROPBOX_JSON_TYPE_UINT;
        valueMap[key] = e;
    }
}

QString SM_Dropbox_Json::getString(QString key, bool force)
{
    if(!valueMap.contains(key))
        return "";

    sm_dropbox_json_entry e;
    e = valueMap.value(key);

    if(!force && e.type != SM_DROPBOX_JSON_TYPE_STR)
        return "";

    QString value = e.value.value->mid(1, e.value.value->size()-2);
    return value;
}

void SM_Dropbox_Json::setString(QString key, QString value)
{
    if(valueMap.contains(key)){
        *(valueMap[key].value.value) = value;
    }else{
        sm_dropbox_json_entry e;
        QString *valuePointer = new QString(value);
        e.value.value = valuePointer;
        e.type        = SM_DROPBOX_JSON_TYPE_STR;
        valueMap[key] = e;
    }
}

SM_Dropbox_Json* SM_Dropbox_Json::getJson(QString key)
{
    if(!valueMap.contains(key))
        return NULL;

    sm_dropbox_json_entry e;
    e = valueMap.value(key);

    if(e.type != SM_DROPBOX_JSON_TYPE_JSON)
        return NULL;


    return e.value.json;
}

void SM_Dropbox_Json::setJson(QString key, SM_Dropbox_Json value)
{
    if(valueMap.contains(key)){
        *(valueMap[key].value.json) = value;
    }else{
        sm_dropbox_json_entry e;
        SM_Dropbox_Json *valuePointer = new SM_Dropbox_Json(value);
        e.value.json = valuePointer;
        e.type        = SM_DROPBOX_JSON_TYPE_JSON;
        valueMap[key] = e;
    }
}

double SM_Dropbox_Json::getDouble(QString key, bool force)
{
    if(!valueMap.contains(key))
        return 0.0f;

    sm_dropbox_json_entry e;
    e = valueMap.value(key);

    if(!force && e.type != SM_DROPBOX_JSON_TYPE_FLOAT)
        return 0.0f;

    return e.value.value->toDouble();
}

void SM_Dropbox_Json::setDouble(QString key, double value)
{
    if(valueMap.contains(key)){
        valueMap[key].value.value->setNum(value);
    }else{
        sm_dropbox_json_entry e;
        QString *valuePointer = new QString();
        valuePointer->setNum(value);
        e.value.value = valuePointer;
        e.type        = SM_DROPBOX_JSON_TYPE_FLOAT;
        valueMap[key] = e;
    }
}

bool SM_Dropbox_Json::getBool(QString key, bool force)
{
    if(!valueMap.contains(key))
        return false;

    sm_dropbox_json_entry e;
    e = valueMap.value(key);

    if(!force && e.type != SM_DROPBOX_JSON_TYPE_BOOL)
        return false;

    if(!e.value.value->compare("false"))
        return false;

    return true;
}

void SM_Dropbox_Json::setBool(QString key, bool value)
{
    if(valueMap.contains(key)){
        *(valueMap[key].value.value) = value ? "true" : "false";
    }else{
        sm_dropbox_json_entry e;
        QString *valuePointer = new QString(value ? "true" : "false");
        e.value.value = valuePointer;
        e.type        = SM_DROPBOX_JSON_TYPE_BOOL;
        valueMap[key] = e;
    }
}

QDateTime SM_Dropbox_Json::getTimestamp(QString key, bool force)
{
    if(!valueMap.contains(key))
        return QDateTime();

    sm_dropbox_json_entry e;
    e = valueMap.value(key);

    if(!force && e.type != SM_DROPBOX_JSON_TYPE_STR)
        return QDateTime();

    const QString dtFormat = "dd MMM yyyy HH:mm:ss";

    QDateTime res = QLocale(QLocale::English).toDateTime(e.value.value->mid(6, dtFormat.size()), dtFormat);
    res.setTimeSpec(Qt::UTC);

    return res;
}

void SM_Dropbox_Json::setTimestamp(QString key, QDateTime value)
{
    const QString dtFormat = "ddd, dd MMM yyyy hh:mm:ss '+0000'";

    value = value.toUTC();

    if(valueMap.contains(key)){
        *(valueMap[key].value.value) = value.toString(dtFormat);
    }else{
        sm_dropbox_json_entry e;
        QString *valuePointer = new QString(QLocale{QLocale::English}.toString(value, dtFormat));
        e.value.value = valuePointer;
        e.value.value = valuePointer;
        e.type        = SM_DROPBOX_JSON_TYPE_STR;
        valueMap[key] = e;
    }
}

QString SM_Dropbox_Json::strContent() const
{
    if(valueMap.size() == 0)
        return "";

    QString content = "{";
    QList<QString> keys = valueMap.keys();
    for(int i=0; i<keys.size(); ++i)
    {
        QString value = "";
        sm_dropbox_json_entry e = valueMap.value(keys.at(i));

        if(e.type != SM_DROPBOX_JSON_TYPE_JSON)
            value = *e.value.value;
        else
            value = e.value.json->strContent();

        content.append(QString("\"%1\": %2").arg(keys.at(i)).arg(value));
        if(i != keys.size()-1)
            content.append(", ");
    }
    content.append("}");
    return content;
}

void SM_Dropbox_Json::emptyList()
{
    QList<QString> keys = valueMap.keys();
    for(qint32 i=0; i<keys.size(); ++i)
    {
        sm_dropbox_json_entry e = valueMap.value(keys.at(i));
        if(e.type == SM_DROPBOX_JSON_TYPE_JSON)
            delete e.value.json;
        else
            delete e.value.value;
        valueMap.remove(keys.at(i));
    }
    valueMap.clear();
    return;
}

sm_dropbox_json_entry_type SM_Dropbox_Json::interpretType(QString value)
{
    // check for string
    if(value.startsWith("\"") && value.endsWith("\""))
        return SM_DROPBOX_JSON_TYPE_STR;

    // check for integer
    bool ok;
    value.toInt(&ok);
    if(ok)
        return SM_DROPBOX_JSON_NUM;

    // check for uint
    value.toUInt(&ok);
    if(ok)
        return SM_DROPBOX_JSON_TYPE_UINT;

    // check for bool
    if(!value.compare("true") || !value.compare("false"))
        return SM_DROPBOX_JSON_TYPE_BOOL;

    // check for array
    if(value.startsWith("[") && value.endsWith("]"))
        return SM_DROPBOX_JSON_TYPE_ARRAY;

    value.toDouble(&ok);
    if(ok)
        return SM_DROPBOX_JSON_TYPE_FLOAT;

    return SM_DROPBOX_JSON_TYPE_UNKNOWN;
}

SM_Dropbox_Json& SM_Dropbox_Json::operator=(SM_Dropbox_Json& other)
{
    /*!< \todo use toString() */
    parseString(other.strContent());
    return *this;
}

QStringList SM_Dropbox_Json::getArray(QString key, bool force)
{
    QStringList list;
    if(!valueMap.contains(key))
        return list;

    sm_dropbox_json_entry e;
    e = valueMap.value(key);

    if(!force && e.type != SM_DROPBOX_JSON_TYPE_ARRAY)
        return list;

    QString arrayStr = e.value.value->mid(1, e.value.value->length()-2);
    QString buffer = "";
    bool inString = false;
    int  inJson   = 0;
    int  inArray  = 0;
    for(int i=0; i<arrayStr.size(); ++i)
    {
        QChar c = arrayStr.at(i);
        if( ((c != ',' && c != ' ') || inString || inJson || inArray) &&
            (c != '"' || inJson > 0 || inArray > 0))
            buffer += c;
        switch(c.toLatin1())
        {
        case '"':
            if(i > 0 && arrayStr.at(i-1).toLatin1() == '\\')
            {
                buffer += c;
                break;
            }
            else
                inString = !inString;
            break;
        case '{':
            inJson++;
            break;
        case '}':
            inJson--;
            break;
        case '[':
            inArray++;
            break;
        case ']':
            inArray--;
            break;
        case ',':
            if(inJson == 0 && inArray == 0 && !inString)
            {
                list.append(buffer);
                buffer = "";
            }
            break;
        }
    }

    if(!buffer.isEmpty())
        list.append(buffer);

    return list;
}

int SM_Dropbox_Json::parseSubJson(QString strJson, int start, sm_dropbox_json_entry *jsonEntry)
{
    int openBrackets = 1;
    QString buffer = "";
    SM_Dropbox_Json* jsonValue = NULL;

    int j;
    for(j=start+1; openBrackets > 0 && j < strJson.size(); ++j)
    {
        if(strJson.at(j).toLatin1() == '{')
            openBrackets++;
        else if(strJson.at(j).toLatin1() == '}')
            openBrackets--;
    }

    buffer = strJson.mid(start, j-start);
#ifdef SM_QTDROPBOX_DEBUG
    qDebug() << "brackets = " << openBrackets << endl;
    qDebug() << "json data(" << start << ":" << j-start << ") = " << buffer << endl;
#endif
    jsonValue = new SM_Dropbox_Json();
    jsonValue->parseString(buffer);

    // invalid sub json means invalid json
    if(!jsonValue->isValid())
    {
#ifdef SM_QTDROPBOX_DEBUG
        qDebug() << "subjson invalid!" << endl;
#endif
        valid = false;
        return j;
    }

    // insert new
    jsonEntry->value.json = jsonValue;
    jsonEntry->type       = SM_DROPBOX_JSON_TYPE_JSON;
    return j;
}

bool SM_Dropbox_Json::isAnonymousArray()
{
    return _anonymousArray;
}

QStringList SM_Dropbox_Json::getArray()
{
    if(!isAnonymousArray())
        return QStringList();

    return getArray("_anonArray");
}

int SM_Dropbox_Json::compare(const SM_Dropbox_Json& other)
{
    if(valueMap.size() != other.valueMap.size())
        return 1;

    QMap<QString, sm_dropbox_json_entry> yourMap = other.valueMap;

    QList<QString> keys = valueMap.keys();
    for(int i=0; i<keys.size(); ++i)
    {
        QString key = keys.at(i);
        if(!yourMap.contains(key))
            return 1;

        sm_dropbox_json_entry myEntry = valueMap.value(key);
        sm_dropbox_json_entry yourEntry = yourMap.value(key);

        if(myEntry.type != yourEntry.type)
            return 1;

        if(myEntry.type == SM_DROPBOX_JSON_TYPE_JSON)
        {
            if(myEntry.value.json->compare(*yourEntry.value.json) != 0)
                return 1;
        }
        else
        {
            if(myEntry.value.value->compare(yourEntry.value.value) != 0)
                return 1;
        }
    }

    return 0;
}
