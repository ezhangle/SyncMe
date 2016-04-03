#include "settings/SM_SettingsManager.h"
#include "SM_ContentUI.h"
#include "share/SM_Debug.h"
#include <QDir>

SM_ContentUI::SM_ContentUI(QObject *parent) :
    QObject(parent)
{
}

void SM_ContentUI::slotLeftViewClicked(const QModelIndex&)
{
    SM_SettingsManager().setCurrentPanel(LEFT_PANEL_VALUE);
}

void SM_ContentUI::slotRightViewClicked(const QModelIndex&)
{
    SM_SettingsManager().setCurrentPanel(RIGHT_PANEL_VALUE);
}

void SM_ContentUI::slotLeftPanelItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    SM_SettingsManager().setCurrentPanel(LEFT_PANEL_VALUE);
    showFilesOnPanel(item->data(0, Qt::DisplayRole).toString(), ELeft);
}

void SM_ContentUI::slotRightPanelItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    SM_SettingsManager().setCurrentPanel(RIGHT_PANEL_VALUE);
    showFilesOnPanel(item->data(0, Qt::DisplayRole).toString(), ERight);
}

void SM_ContentUI::showFilesOnPanel(const QString &name, SM_GUI_EPanels panel)
{
    if(name == PARENT_FOLDER_SIGN)
    {
        performShowFiles(SDriveEngine::inst()->getContentMngr()->back(), name, EBackward, panel);
    }
    else
    {
        if(isFolder())
        {
            QString query(GET_FILES_IN_FOLDER);

            query += SM_CommonTools::getIDFromURL(SDriveEngine::inst()->getContentMngr()->getCurrentFileInfo().self);
            query += (CONTENTS + MAX_RESULTS);

            performShowFiles(query, name, EForward, panel);
        }
    }
}

bool SM_ContentUI::isFolder(void)
{
  return (SDriveEngine::inst()->getContentMngr()->getCurrentFileInfo().type == FOLDER_TYPE_STR);
}

void SM_ContentUI::setCurrentPanelState(SM_GUI_EPanels panel, const QString &url)
{
    SM_SettingsManager settingsManager;

    int panelNum = static_cast <int> (panel);

    settingsManager.setCurrentFolderURL(panelNum, url);

    QString fullPath(getPanelLabel(panel)->text());
    int beginPos = fullPath.indexOf(QDir::toNativeSeparators("/")) + 1;
    int length = fullPath.length() - beginPos;

    settingsManager.setCurrentFolderPath(panelNum, fullPath.mid(beginPos, length));
    settingsManager.setPathesURLs(panelNum, SDriveEngine::inst()->getContentMngr()->getPathesURLs());
}

void SM_ContentUI::performShowFiles(const QString &query, const QString &name, EPath path, SM_GUI_EPanels panel)
{
    setPanelDisplayingPath(name, path, panel);
    SDriveEngine::inst()->getContentMngr()->get(query);
    setCurrentPanelState(panel, query);

    children.fetch();
}

void SM_ContentUI::slotUpdateFileList()
{
    SDriveEngine::inst()->getContentMngr()->get(SDriveEngine::inst()->getContentMngr()->getParentFolderUrl());
}

void SM_ContentUI::setPanelDisplayingPath(const QString &name, EPath path, SM_GUI_EPanels panel)
{
    QLabel* label = getPanelLabel(panel);
    QString pathDividerSign(QDir::toNativeSeparators("/"));
    int labelTextlength = label->text().length();
    int discStrLength = getDiscLength(panel);

    switch(path)
    {
    case EForward:
    {
        QString divider((labelTextlength == discStrLength) ? "" : pathDividerSign);
        label->setText(label->text() += (divider + name));
    }
        break;
    case EBackward:
    {
        int pos = label->text().lastIndexOf(pathDividerSign);
        int removeCount = labelTextlength - (pos + 1) + 1;

        if(pos == (discStrLength - 1)) ++pos;

        label->setText(label->text().remove(pos,  removeCount));
    }
        break;
    }
}

int SM_ContentUI::getDiscLength(SM_GUI_EPanels panel) const
{
    SM_SettingsManager settingsManager;
    QString disc(settingsManager.accountDisc(settingsManager.currentAccount(static_cast <int> (panel))));

    disc += QString(":");
    disc += QDir::toNativeSeparators("/");

    return disc.length();
}

QLabel* SM_ContentUI::getPanelLabel(SM_GUI_EPanels panel) const
{
    return  SDriveEngine::inst()->getFilePanel(panel)->getPathLabel();
}