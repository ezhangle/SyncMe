#include "../../../Headers/GDrive/Parsers/SM_XMLParser.h"

SM_XMLParser::SM_XMLParser():
    xmlHandler(new SM_XMLHandler)
{
}

SM_XMLParser::~SM_XMLParser()
{
}

bool SM_XMLParser::startElement(const QString &qName, const QXmlAttributes &attribs)
{
    //return xmlHandler->startElement(namespaceURI, localName, qName, attribs);
    return xmlHandler->startElement(qName, attribs);
}

bool SM_XMLParser::endElement()
{
    //return xmlHandler->endElement(namespaceURI, localName, qName);
    return xmlHandler->endElement();
}

bool SM_XMLParser::characters(const QString &str)
{
    return xmlHandler->characters(str);
}

bool SM_XMLParser::fatalError(const QXmlParseException &exception)
{
    return xmlHandler->fatalError(exception);
}

SM_XMLHandler* SM_XMLParser::getXMLHandler(void) const
{
    return xmlHandler.data();
}
