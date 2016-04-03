#include "../../../Headers/GDrive/Parsers/SM_XMLHandler.h"
#include "../../../Headers/GDrive/Share/SM_CommonTools.h"
#include "../../../Headers/GDrive/Share/SM_Debug.h"

SM_XMLHandler::SM_XMLHandler():
    itemInfo(new SM_Items),
    resDownloadedCount(0),
    isResDownloding(false)
{
    for(int i = ETitle; i < ETagsCount; ++i)
    {
        tags[i] = false;
    }
}

SM_XMLHandler::~SM_XMLHandler()
{
}

bool SM_XMLHandler::startElement(const QString &qName, const QXmlAttributes &attribs)
{
    return handleReply(qName, attribs);
}

bool SM_XMLHandler::endElement()
{
    return true;
}

bool SM_XMLHandler::characters(const QString &str)
{
    if(tags[EAuthor]) handleAuthorTag(str, tags[EEntry]);
    if(tags[ETitle]) handleTitleTag(str);
    if(tags[ESize]) handleSizeTag(str);
    if(tags[EUpdated]) handleUpdatedTag(str);

    return true;
}

void SM_XMLHandler::handleAuthorTag(const QString &str, bool entryTag)
{
    int index;

    if(entryTag)
    {
        index = itemInfo->getItems().count() - 1;

        QString author(str);

        if(itemInfo->getAccountOwner() == author)
        {
            author = QString("me");
        }

        itemInfo->setDataOwner(author, index);
    }
    else
    {
        itemInfo->setAccountOwner(str);
    }

    tags[EAuthor] = false;
    tags[EEntry] = false;
}

void SM_XMLHandler::handleTitleTag(const QString &str)
{
    itemData.name = str;
    tags[ETitle] = false;
}

void SM_XMLHandler::handleSizeTag(const QString &str)
{
    itemInfo->setFileSize(str, itemInfo->getItems().count() - 1);
    tags[ESize] = false;
}

void SM_XMLHandler::handleUpdatedTag(const QString &str)
{
    itemData.dateTime = str;
    tags[EUpdated] = false;
}

SM_Items* SM_XMLHandler::getItemInfo(void) const
{
    return itemInfo.data();
}

bool SM_XMLHandler::fatalError(const QXmlParseException &exception)
{
    DEBUG << "Error fatal: " << exception.message();
    return true;
}

bool SM_XMLHandler::handleReply(const QString &qName, const QXmlAttributes &attribs)
{
    setTag(qName, true);

    if(qName == CONTENT)
    {
        itemData.fileType = FYLE_TYPE_ATTRIBUTE_TAG(attribs.value);
        itemData.downloadLink = FYLE_TYPE_SRC_ATTRIBUTE_TAG(attribs.value);
    }

    if(HIERARCHY_ATTRIBUTE_TAG(attribs.value) == PARENT_TAG) itemData.parent = HIERARCHY_VALUE_TAG(attribs.value);
    if(HIERARCHY_ATTRIBUTE_TAG(attribs.value) == UPLOAD_TAG) itemData.uploadLink = HIERARCHY_VALUE_TAG(attribs.value);
    if(HIERARCHY_ATTRIBUTE_TAG(attribs.value) == ICON_TAG) saveResData(attribs);

    if(HIERARCHY_ATTRIBUTE_TAG(attribs.value) == SELF_TAG)
    {
        itemData.self = HIERARCHY_VALUE_TAG(attribs.value);
        itemData.type = getItemType(itemData.self);
        itemInfo->pushBack(itemData);

        setDefaults();
    }

    return true;
}

QString SM_XMLHandler::getItemType(const QString &selfURL)
{
    QString typeStr(FILE_TYPE_STR);
    QString  str(selfURL);
    QStringList strList = str.split("/");

    str = strList[strList.count() - 1];

    if(str.indexOf(FOLDER_TYPE_STR) != -1)
    {
        typeStr = FOLDER_TYPE_STR;
    }

    return typeStr;
}

void SM_XMLHandler::setTag(const QString &qName, bool state)
{
    if(qName == ENTRY_TAG) tags[EEntry] = state;
    if(qName == AUTHOR_TAG) tags[EAuthor]= state;
    if(qName == FOLDER_TITLE_TAG) tags[ETitle]= state;
    if(qName == FILE_SIZE_TAG) tags[ESize] = state;
    if(qName == UPDATED_FILE_TAG) tags[EUpdated] = state;
}

void SM_XMLHandler::slotResDownloaded()
{
    if(++resDownloadedCount == resManagers.count())
    {
        for(int i = 0; i < resManagers.count();++i)
        {
            delete resManagers[i];
        }

        resManagers.clear();
        resDownloadedCount = 0;

        emit signalAllResDownloaded();
    }
}

void SM_XMLHandler::saveResData(const QXmlAttributes &attribs)
{
    if(!SM_CommonTools::fileFromURLExists(HIERARCHY_VALUE_TAG(attribs.value)))
    {
        isResDownloding = true;

        resManagers.push_back(new SM_ResManager);
        resManagers.last()->cash(HIERARCHY_VALUE_TAG(attribs.value));

        connect(resManagers.last(), SIGNAL(signalResDownloaded()), this, SLOT(slotResDownloaded()));
    }

    itemData.iconPath = SM_CommonTools::getFileNameFromURL(HIERARCHY_VALUE_TAG(attribs.value));
}

void SM_XMLHandler::setDefaults(void)
{
    itemData.fileSize = QString("---");
    itemData.dateTime = QString();
    itemData.parent = ROOT_TAG;
}

bool SM_XMLHandler::resDownloadingNow(void) const
{
    return isResDownloding;
}
