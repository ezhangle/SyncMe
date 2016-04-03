#include "SM_GDrive_MainWindow.h"
#include "ui_mainwindow.h"
#include "core/SM_DriveEngine.h"
#include "share/SM_Debug.h"
#include "settings/SM_SettingsManager.h"
#include "gui/forms/SM_AuthDialog.h"
#include "network/operations/SM_OperationsManager.h"
#include <QTextCodec>
#include <QKeyEvent>
#include <QToolButton>

SM_GDrive_MainWindow::SM_GDrive_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    auth(NULL)
{
    SUi::inst()->setupUi(this);
}

SM_GDrive_MainWindow::~SM_GDrive_MainWindow()
{
    SDriveEngine::freeInst();
    SUi::freeInst();
    SOperationsManager::freeInst();
}

void SM_GDrive_MainWindow::init(void)
{
    // Aviso inicial
    SM_CommonTools::msg(tr("<img src=\":/ico/syncmeLogo\"><br><br>Esto es un prototipo de <b>SyncMe</b> para sincronizarte con <u>Google Drive</u>.<br><br>Si tienes interés en ayudar con el desarrollo no dudes en hacerlo.<br><br><u>Repo en GitHub:</u> <a href=\"https://github.com/AdrianBZG/SyncMe\">Enlace</a><br><u>Colaboradores:</u> <a href=\"https://github.com/AdrianBZG/SyncMe/graphs/contributors\">Enlace</a>"), "SyncMe: Información");

    SDriveEngine::inst(this)->init();
    SDriveEngine::inst()->getCheckUI()->slotCheckWorkDir(false);

    setConnections();

    SDriveEngine::inst()->getFilePanel(ELeft)->getFileView()->installEventFilter(this);
    SDriveEngine::inst()->getFilePanel(ERight)->getFileView()->installEventFilter(this);

    SDriveEngine::inst()->getFilePanel(ELeft)->getFileView()->header()->resizeSection(0, 250);
    SDriveEngine::inst()->getFilePanel(ERight)->getFileView()->header()->resizeSection(0, 250);
}

void SM_GDrive_MainWindow::setConnections(void)
{
    connect(SUi::inst()->actionLogin, SIGNAL(triggered()), this, SLOT(slotAuthDialog()));
    connect(SUi::inst()->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(SUi::inst()->actionDownload, SIGNAL(triggered()), SDriveEngine::inst()->getfilesTransferUI(), SLOT(slotDownload()));
    connect(SUi::inst()->actionUpload, SIGNAL(triggered()), SDriveEngine::inst()->getfilesTransferUI(), SLOT(slotUpload()));
    connect(SUi::inst()->actionSettings, SIGNAL(triggered()), SDriveEngine::inst()->getCheckUI(), SLOT(slotCheckWorkDir()));   
    connect(SUi::inst()->actionDelete, SIGNAL(triggered()), SOperationsManager::inst(), SLOT(slotDeleteItem()));
    connect(SUi::inst()->actionNewFolder, SIGNAL(triggered()), SOperationsManager::inst(), SLOT(slotNewFolder()));
    connect(SUi::inst()->copyButton, SIGNAL(clicked()), SOperationsManager::inst(), SLOT(slotCopyFile()));
    connect(SUi::inst()->moveButton, SIGNAL(clicked()), SOperationsManager::inst(), SLOT(slotMoveFile()));
    connect(SUi::inst()->newFolderButton, SIGNAL(clicked()), SOperationsManager::inst(), SLOT(slotNewFolder()));
    connect(SUi::inst()->deleteButton, SIGNAL(clicked()), SOperationsManager::inst(), SLOT(slotDeleteItem()));
    connect(SUi::inst()->renameButton, SIGNAL(clicked()), SOperationsManager::inst(), SLOT(slotRenameItem()));
    connect(SUi::inst()->shareButton, SIGNAL(clicked()), SOperationsManager::inst(), SLOT(slotShareFile()));
    //
    connect(SUi::inst()->actionColabora_con_el_desarrollo, SIGNAL(triggered()), this, SLOT(slotColaboraDialog()));
    //
    connect(SDriveEngine::inst()->getFilePanel(ELeft)->getFileView(), SIGNAL(clicked(const QModelIndex&)), SDriveEngine::inst()->getfilesUI(), SLOT(slotLeftViewClicked(const QModelIndex&)));
    connect(SDriveEngine::inst()->getFilePanel(ERight)->getFileView(), SIGNAL(clicked(const QModelIndex&)), SDriveEngine::inst()->getfilesUI(), SLOT(slotRightViewClicked(const QModelIndex&)));
    connect(SDriveEngine::inst()->getFilePanel(ELeft)->getFileView(), SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SDriveEngine::inst()->getfilesUI(), SLOT(slotLeftPanelItemDoubleClicked(QTreeWidgetItem*, int)));
    connect(SDriveEngine::inst()->getFilePanel(ERight)->getFileView(), SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SDriveEngine::inst()->getfilesUI(), SLOT(slotRightPanelItemDoubleClicked(QTreeWidgetItem*, int)));
    connect(SDriveEngine::inst()->getContentMngr()->self(), SIGNAL(signalAccessTokenRequired()), this, SLOT(slotAccessTokenRequired()));
    connect(SDriveEngine::inst()->getContentMngr(true)->self(), SIGNAL(signalAccessTokenRequired()), this, SLOT(slotAccessTokenRequired()));
    connect(SDriveEngine::inst()->getContentMngr(), SIGNAL(signalFirstPanelIsLoaded()), SDriveEngine::inst(), SLOT(slotFirstPanelIsLoaded()));
    connect(SOperationsManager::inst(), SIGNAL(signalAccountInfoReadyToUse()), this, SLOT(slotAccountInfoReadyToUse()));
}

void SM_GDrive_MainWindow::slotAuthDialog(void)
{
    SM_AuthDialog authDialog(SUi::inst()->centralWidget);
    connect(&authDialog, SIGNAL(signalTokens(const QString&, const QString&)), this, SLOT(slotTokens(const QString&, const QString&)));
    authDialog.exec();
}

void SM_GDrive_MainWindow::slotColaboraDialog(void)
{
    SM_CommonTools::msg(tr("<img src=\":/ico/collaborationImage\"><br><br>Para colaborar en el proyecto de <b>SyncMe</b> solo tienes que proponer tus sugerencias, posibles cambios...etc utilizando los Issues, y añadir tus propias mejoras utilizando Pull Request.<br><br><u>Repo en GitHub:</u> <a href=\"https://github.com/AdrianBZG/SyncMe\">Enlace</a><br><u>Issues:</u> <a href=\"https://github.com/AdrianBZG/SyncMe/issues\">Enlace</a><br><u>Pull Requests:</u> <a href=\"https://github.com/AdrianBZG/SyncMe/pulls\">Enlace</a>"), "SyncMe: Colabora con el desarrollo");
}

void SM_GDrive_MainWindow::slotTokens(const QString &accessToken, const QString &refreshToken)
{
    SOperationsManager::inst()->setAccountInfo(accessToken, refreshToken);
}

void SM_GDrive_MainWindow::slotAccountInfoReadyToUse(void)
{
    SDriveEngine::inst()->updatePanel(LEFT_PANEL_VALUE, true);
}

void SM_GDrive_MainWindow::slotAccessTokenRequired(void)
{
    SM_SettingsManager settingsManager;

    if(auth) delete auth;
    auth = new SM_Auth;

    auth->getAccessToken(settingsManager.clientId(), settingsManager.clientSecret(), settingsManager.refreshToken());
    connect(auth, SIGNAL(signalAuthResponse(const QString&)), this, SLOT(slotAuthResponse(const QString&)));
}

void SM_GDrive_MainWindow::slotAuthResponse(const QString &accessToken)
{
    SOperationsManager::inst()->setAccountInfo(accessToken);

    auth->deleteLater();
    auth = NULL;
}