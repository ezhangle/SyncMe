#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include <QXmlSimpleReader>
#include <QHeaderView>
#include "../Parsers/SM_XMLParser.h"
#include "SM_NetworkManager.h"
#include "Operations/SM_OperationsManager.h"

class SM_ContentManager : public SM_NetworkManager
{
    Q_OBJECT
public:
    explicit SM_ContentManager(QObject *parent = 0);
    virtual ~SM_ContentManager();

public:
    void get(const QString &url);
    SM_XMLParser* getParser(void) const;
    void clear(void);
    QString getParentFolderUrl(void) const;
    SM_Items::Data getUpperLevelFolderInfo(void) const;
    QTreeWidget* getPanel(void) const;
    QString back(void);
    void setPanel(QTreeWidget *p, int pn);
    void sort();
    SM_Items::Data getCurrentFileInfo(void);
    QStringList getPathesURLs(void) const;
    void setPathesURLs(const QStringList &pathesURLsStrList);

private:
    void show(void);
    bool parseReply(const QString &str);
    void setItems(SM_Items::Data::ESortOrder itemSortOrder, Qt::SortOrder sortOrder);
    void addItem(const SM_Items::Data &itemData);
    QString getDate(const QString &date);
    QString getSize(const QString &size);
    void updateItemsState(void);

signals:
    void signalFirstPanelIsLoaded();

private slots:
        void slotSectionClicked(int logicalIndex);
        virtual void slotReplyFinished(QNetworkReply* reply);
        void slotResDownloaded(void);

private:
    QScopedPointer<SM_XMLParser> parser;
    QList<QTreeWidgetItem*> treeWidgetItems;
    QTreeWidget *panel;
    QStringList pathesURLs;
    SM_Items::Data rootData;
    QList<SM_Items::Data> normalizedItems;
    bool isRoot;
    int panelNum;
};



#endif // CONTENTMANAGER_H
