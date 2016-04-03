#ifndef XMLHANDLER_H
#define XMLHANDLER_H

#include <QObject>
#include <QXmlAttributes>
#include "gui/SM_Items.h"
#include "share/SM_Defs.h"
#include "network/SM_ResManager.h"

class SM_XMLHandler : public QObject
{
    Q_OBJECT
public:

    enum ETags
    {
        ETitle,
        ESize,
        EUpdated,
        EAuthor,
        EEntry,
        ETagsCount
    };

    SM_XMLHandler();
    ~SM_XMLHandler();

public:
    bool startElement(const QString &qName, const QXmlAttributes &attribs);
    bool endElement();
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);

    SM_Items* getItemInfo(void) const;
    bool resDownloadingNow(void) const;

public slots:
    void slotResDownloaded();

signals:
    void signalAllResDownloaded(void);

private:
    bool handleReply(const QString &qName, const QXmlAttributes &attribs);
    void handleAuthorTag(const QString &str, bool entryTag);
    void handleTitleTag(const QString &str);
    void handleSizeTag(const QString &str);
    void handleUpdatedTag(const QString &str);
    void setDefaults(void);
    void setTag(const QString& qName, bool state);
    QString getItemType(const QString &selfURL);
    void saveResData(const QXmlAttributes &attribs);

private:
    QScopedPointer<SM_Items> itemInfo;
    SM_Items::Data itemData;
    QList<SM_ResManager*> resManagers;
    int resDownloadedCount;
    bool isResDownloding;
    bool tags[ETagsCount];
};

#endif // XMLHANDLER_H