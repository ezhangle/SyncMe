#ifndef DRIVEENGINE_H
#define DRIVEENGINE_H

#include <QTreeWidgetItem>
#include "../Network/SM_DownloadFileManager.h"
#include "../Network/SM_UploadFileManager.h"
#include "../Network/SM_ContentManager.h"
#include "../GUI/Forms/SM_GDrive_MainWindow.h"
//#include "ui_mainwindow.h"
#include "../GUI/SM_FilesTransferUI.h"
#include "../GUI/SM_ContentUI.h"
#include "../GUI/SM_CheckUI.h"
#include "../Share/SM_Enums.h"
#include "../GUI/Forms/SM_FilePanel.h"

class SM_FilesTransferUI;
class SM_ContentUI;

class SM_DriveEngine : public QObject
{
    Q_OBJECT
public:
    explicit SM_DriveEngine(QWidget *p = 0);
    ~SM_DriveEngine();

    friend class SM_FilesTransferUI;
    friend class SM_ContentUI;

public:   
    SM_CheckUI *getCheckUI(void) const;
    SM_ContentManager *getContentMngr(bool opposite = false) const;
    SM_FilesTransferUI *getfilesTransferUI(void) const;
    SM_ContentUI *getfilesUI(void) const;
    QWidget *getParent(void) const;
    void init(void);
    SM_FilePanel *getFilePanel(SM_GUI_EPanels panel) const;
    void updatePanel(int panelId, bool initLoad);

private slots:
    void slotFirstPanelIsLoaded(void);
    void slotAccountChanged(int panelNum, const QString& accountName);

private:
    void reset(void);   

private:
    QWidget *parent;
    QScopedPointer<SM_CheckUI> checkUI;
    QScopedPointer<SM_DownloadFileManager> downloadMngr;
    QScopedPointer<SM_ContentManager> contentMngr[EPanelsCount];
    QScopedPointer<SM_FilesTransferUI> filesTransferUI;
    QScopedPointer<SM_ContentUI> filesUI;
    QScopedPointer<SM_UploadFileManager> uploadFileMngr;
    SM_FilePanel *filesViews[EPanelsCount];
};

typedef SM_TSingleton_Item<SM_DriveEngine> SDriveEngine;

#endif // DRIVEENGINE_H
