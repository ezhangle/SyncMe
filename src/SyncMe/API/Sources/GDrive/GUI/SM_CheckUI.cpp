#include "SM_CheckUI.h"
#include "settings/SM_SettingsManager.h"
#include <QMessageBox>
#include "gui/forms/SM_SettingsDialog.h"
#include "share/SM_Defs.h"
#include "gui/forms/SM_GDrive_MainWindow.h"
#include "ui_mainwindow.h"
#include "core/SM_DriveEngine.h"

SM_CheckUI::SM_CheckUI(QObject *parent) :
    QObject(parent)
{
}

bool SM_CheckUI::checkReg(void)
{
    return false;
}

bool SM_CheckUI::slotCheckWorkDir(bool showDlg)
{   
    SM_SettingsManager settingsManager;

    if(settingsManager.isWorkDirSet() && !showDlg)
    {
        return true;
    }

    bool dirTextNotEmpty = false;

    if(showDlg)
    {
        SM_SettingsDialog dlg(SDriveEngine::inst()->getParent());

        dlg.setDirectoryPath(settingsManager.workDir());

        switch(dlg.exec())
        {
        case QDialog::Accepted:
        {
            if(!dlg.directoryPath().isEmpty() )
            {
                settingsManager.setWorkDir(dlg.directoryPath());
                dirTextNotEmpty = true;
            }

        }
            break;
        }
    }

    return dirTextNotEmpty;
}