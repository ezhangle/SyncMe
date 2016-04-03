#ifndef FILESUI_H
#define FILESUI_H

#include "core/SM_DriveEngine.h"
#include "share/SM_Enums.h"
#include <QLabel>

class SM_ContentUI: public QObject
{
    Q_OBJECT
public: 
    enum EPath
    {
        EForward,
        EBackward
    };

    explicit SM_ContentUI(QObject *parent = 0);

    friend class SM_FilesTransferUI;
    friend class FoldersUI;
    friend class OperationsUI;
    friend class SM_DriveEngine;

public:
    QLabel* getPanelLabel(SM_GUI_EPanels panel) const;

private slots:
    void slotLeftViewClicked(const QModelIndex&);
    void slotRightViewClicked(const QModelIndex&);
    void slotLeftPanelItemDoubleClicked(QTreeWidgetItem *item, int column);
    void slotRightPanelItemDoubleClicked(QTreeWidgetItem *item, int column);
    void slotUpdateFileList();

private:
    int getDiscLength(SM_GUI_EPanels panel) const;
    void setPanelDisplayingPath(const QString &name, EPath path, SM_GUI_EPanels panel);
    void showFilesOnPanel(const QString &name, SM_GUI_EPanels panel);
    void setCurrentPanelState(SM_GUI_EPanels panel, const QString &url);
    void performShowFiles(const QString &query, const QString &name, EPath path, SM_GUI_EPanels panel);
    bool isFolder(void);
    SM_Children children;
};

#endif // FILESUI_H
