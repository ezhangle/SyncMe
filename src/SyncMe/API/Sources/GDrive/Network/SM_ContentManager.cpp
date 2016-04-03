#include "SM_ContentManager.h"
#include "share/SM_CommonTools.h"
#include "settings/SM_SettingsManager.h"
#include "core/SM_DriveEngine.h"
#include "share/SM_Debug.h"
#include <QLocale>
#include <QDateTime>
#include <QPair>
#include <QApplication>

SM_ContentManager::SM_ContentManager(QObject *parent):
    SM_NetworkManager(parent),
    panel(NULL)
{
    parser.reset(new SM_XMLParser);
}

SM_ContentManager::~SM_ContentManager()
{
    if(panel && panel->topLevelItemCount() < 1) panel->clear();
    pathesURLs.clear();
}

void SM_ContentManager::get(const QString &url)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    SM_CommonTools::setHeader(SM_SettingsManager().accessToken(), request);
    getRequest(url);
}

void SM_ContentManager::slotReplyFinished(QNetworkReply* reply)
{
    // Para mandar a un log las respuestas que lea del parser
    //CommonTools::logToFile(QString("ParserReply ") + ".txt", replyStr.toLatin1());

    DEBUG << "<===============================================================================================================";
    DEBUG << "replyStr" << replyStr;
    DEBUG << "===============================================================================================================>";

    parseReply(replyStr);

    replyStr.clear();

    if(!parser->getXMLHandler()->resDownloadingNow() && reply->error() != QNetworkReply::AuthenticationRequiredError)
    {
        show();
    }

    QApplication::restoreOverrideCursor();
}

bool SM_ContentManager::parseReply(const QString &str)
{
    QXmlSimpleReader reader;
    QXmlInputSource source;

    parser.reset(new SM_XMLParser);

    connect(parser->getXMLHandler(), SIGNAL(signalAllResDownloaded()),this, SLOT(slotResDownloaded()));

    source.setData(str.toLatin1());

    reader.setContentHandler(parser.data());
    reader.setErrorHandler(parser.data());

    return reader.parse(&source);
}

void SM_ContentManager::slotResDownloaded(void)
{
    show();
}

SM_XMLParser* SM_ContentManager::getParser(void) const
{
    return parser.data();
}

void SM_ContentManager::clear(void)
{
    treeWidgetItems.clear();

    if(treeWidgetItems.empty()) return;

    for(int i = 0; i < treeWidgetItems.count(); ++i)
    {
        delete treeWidgetItems[i];
    }
}

void SM_ContentManager::show(void)
{
    if(!panel) return;

    clear();
    panel->clear();

    updateItemsState();

    if(getRequest().url() != GET_FULL_ROOT_CONTENT)
    {
        isRoot = false;

        treeWidgetItems.push_back(new QTreeWidgetItem(panel));
        treeWidgetItems.last()->setText(0, PARENT_FOLDER_SIGN);
    }
    else
    {
        isRoot = true;
    }

    for(int i = 0; i < normalizedItems.count(); ++i)
    {
        addItem(normalizedItems[i]);
    }

    if(SM_SettingsManager().initialLoading()) emit signalFirstPanelIsLoaded();

    QString url(getRequest().url().toString());

    if(!pathesURLs.contains(url)) pathesURLs.push_back(url);

    connect(panel->header(),SIGNAL(sectionClicked(int)), this, SLOT(slotSectionClicked(int)));
}

void SM_ContentManager::setItems(SM_Items::Data::ESortOrder itemSortOrder, Qt::SortOrder sortOrder)
{
    normalizedItems = parser->getXMLHandler()->getItemInfo()->getItems();

    if(!normalizedItems.isEmpty())
    {
        rootData = normalizedItems.takeFirst();
    }

    parser->getXMLHandler()->getItemInfo()->sort(normalizedItems, itemSortOrder, sortOrder);
}

void SM_ContentManager::addItem(const SM_Items::Data &itemData)
{
    treeWidgetItems.push_back(new QTreeWidgetItem(panel));

    treeWidgetItems.last()->setText(0, itemData.name);
    treeWidgetItems.last()->setIcon(0, QPixmap(itemData.iconPath));
    treeWidgetItems.last()->setText(1, itemData.owner);
    treeWidgetItems.last()->setText(2, getDate(itemData.dateTime));
    treeWidgetItems.last()->setText(3, getSize(itemData.fileSize));
}

QString SM_ContentManager::getDate(const QString &date)
{
    QDateTime fileDateTime = QDateTime::fromString(date, Qt::ISODate);
    return fileDateTime.toLocalTime().toString("ddd, MMM d yyyy, h:mm");
}

QString SM_ContentManager::getSize(const QString &size)
{
    QString formattedSize;

    if(size.toLongLong() > 0)
    {
        formattedSize = QLocale().toString(size.toLongLong());
    }

    return formattedSize;
}

void SM_ContentManager::setPanel(QTreeWidget *p, int pn)
{
    panel = p;
    panelNum = pn;
}

QTreeWidget* SM_ContentManager::getPanel(void) const
{
    return panel;
}

QString SM_ContentManager::getParentFolderUrl(void) const
{
    return pathesURLs.last();
}

SM_Items::Data SM_ContentManager::getUpperLevelFolderInfo(void) const
{
    return  rootData;
}

QString SM_ContentManager::back(void)
{
    QString prevLink;

    if(!pathesURLs.isEmpty())
    {
        pathesURLs.pop_back();
        prevLink = pathesURLs.last();
        pathesURLs.pop_back();
    }

    return prevLink;
}

QStringList SM_ContentManager::getPathesURLs(void) const
{
    return pathesURLs;
}

void SM_ContentManager::setPathesURLs(const QStringList &pathesURLsStrList)
{
    pathesURLs = pathesURLsStrList;
}

void SM_ContentManager::sort()
{
}

SM_Items::Data SM_ContentManager::getCurrentFileInfo(void)
{
    int index;

    if(isRoot) index = panel->currentIndex().row();
    else index = panel->currentIndex().row() - 1;

    if(index < 0) index = 0;

    return  normalizedItems[index];
}

void SM_ContentManager::slotSectionClicked(int logicalIndex)
{
    Q_UNUSED(logicalIndex);
    SM_SettingsManager().savePanelHeaderState(panelNum, panel->header()->saveState());
    show();
}

void SM_ContentManager::updateItemsState(void)
{
    SM_Items::Data::ESortOrder itemSortOrder;
    Qt::SortOrder sortOrder;

    QByteArray values(SM_SettingsManager().restorePanelHeaderState(panelNum));

    if(values.isEmpty())
    {
        itemSortOrder = SM_Items::Data::ETypeName;
        sortOrder = Qt::AscendingOrder;

        panel->header()->setSortIndicator(0, sortOrder);
    }
    else
    {
        panel->header()->restoreState(values);

        itemSortOrder = static_cast<SM_Items::Data::ESortOrder> (panel->header()->sortIndicatorSection());
        sortOrder =  static_cast<Qt::SortOrder> (panel->header()->sortIndicatorOrder());
    }

    setItems(itemSortOrder, sortOrder);
}