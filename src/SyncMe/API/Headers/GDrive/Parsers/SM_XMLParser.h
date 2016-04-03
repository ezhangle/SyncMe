#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QXmlDefaultHandler>
#include "SM_XMLHandler.h"

class SM_XMLParser : public QXmlDefaultHandler
{
public:
    SM_XMLParser();
    ~SM_XMLParser();

public:
    bool startElement(const QString &qName, const QXmlAttributes &attribs);
    bool endElement();
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);
    SM_XMLHandler* getXMLHandler(void) const;

private:
    QScopedPointer<SM_XMLHandler> xmlHandler;
};

#endif // XMLPARSER_H
