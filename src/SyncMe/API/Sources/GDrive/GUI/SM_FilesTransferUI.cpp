#include "../../../Headers/GDrive/GUI/SM_FilesTransferUI.h"
#include "../../../Headers/GDrive/Share/SM_Debug.h"
#include "../../../Headers/GDrive/Settings/SM_SettingsManager.h"
#include <QFileDialog>
#include <QDir>

SM_FilesTransferUI::SM_FilesTransferUI(QObject *parent) :
    QObject(parent)
{
}

void SM_FilesTransferUI::download(void)
{
    if(SDriveEngine::inst()->downloadMngr)
    {
        if(SDriveEngine::inst()->downloadMngr->getState() == SM_NetworkManager::EBusy) return;
    }

    SM_Items::Data item = SDriveEngine::inst()->getContentMngr()->getCurrentFileInfo();

    QString downloadLink(item.downloadLink);

    if(SDriveEngine::inst()->checkUI->slotCheckWorkDir(false))
    {
        QString fileName(SM_SettingsManager().workDir() + QDir::toNativeSeparators("/") + item.name);

        SDriveEngine::inst()->downloadMngr.reset(new SM_DownloadFileManager(SDriveEngine::inst()->parent));
        SDriveEngine::inst()->downloadMngr->startDownload(QUrl(downloadLink), fileName, item.fileType);
    }
    else
    {
        SM_CommonTools::msg(tr("Debes especificar un directorio de trabajo para descargar ficheros"));
    }
}

void SM_FilesTransferUI::upload(void)
{
    if(SDriveEngine::inst()->uploadFileMngr)
    {
        if(SDriveEngine::inst()->uploadFileMngr->getState() == SM_NetworkManager::EBusy) return;
    }

    QString  fileName = QFileDialog::getOpenFileName(SDriveEngine::inst()->parent, tr("Subiendo fichero"), QDir::homePath(), tr("Todos los ficheros(*)"));

    if(!fileName.isEmpty())
    {
        QString uploadLink(SDriveEngine::inst()->getContentMngr()->getUpperLevelFolderInfo().uploadLink + QString("/?convert=false"));

        SDriveEngine::inst()->uploadFileMngr.reset(new SM_UploadFileManager(SDriveEngine::inst()->parent));
        connect(SDriveEngine::inst()->uploadFileMngr.data(), SIGNAL(signalUpdateFileList()), SDriveEngine::inst()->filesUI.data(), SLOT(slotUpdateFileList()));
        SDriveEngine::inst()->uploadFileMngr->startUpload(uploadLink, fileName);
    }
}

void SM_FilesTransferUI::slotDownload(void)
{
    download();
}

void SM_FilesTransferUI::slotUpload(void)
{
    upload();
}
