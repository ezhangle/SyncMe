#include "SM_DriveEngine.h"
#include "share/SM_Debug.h"
#include "settings/SM_SettingsManager.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QSplitter>
#include <QDir>

SM_DriveEngine::SM_DriveEngine(QWidget *p) :
    parent(p)
{
}

SM_DriveEngine::~SM_DriveEngine()
{
}

void SM_DriveEngine::init(void)
{
    reset();

    if(SM_SettingsManager().isAnyAccount())
    {
        updatePanel(LEFT_PANEL_VALUE, true);
    }
}

void SM_DriveEngine::reset(void)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout(SUi::inst()->panelsWidget);
    QSplitter *hSplitter = new QSplitter(Qt::Horizontal, SUi::inst()->panelsWidget);

    filesViews[ELeft] = new SM_FilePanel(ELeft);
    filesViews[ERight] = new SM_FilePanel(ERight);

    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->addWidget(hSplitter);

    hSplitter->setHandleWidth(1);

    hSplitter->addWidget(filesViews[ELeft]);
    hSplitter->addWidget(filesViews[ERight]);

    for(int i = 0; i < EPanelsCount; ++i)
    {
        contentMngr[i].reset(new SM_ContentManager);
        contentMngr[i]->init();
    }

    checkUI.reset(new SM_CheckUI);
    filesTransferUI.reset(new SM_FilesTransferUI);
    filesUI.reset(new SM_ContentUI);

    connect(filesViews[ELeft], SIGNAL(signalAccountChanged(int, const QString&)), SLOT(slotAccountChanged(int, const QString&)));
    connect(filesViews[ERight], SIGNAL(signalAccountChanged(int, const QString&)), SLOT(slotAccountChanged(int, const QString&)));
}

SM_FilePanel* SM_DriveEngine::getFilePanel(SM_GUI_EPanels panel) const
{
    return filesViews[panel];
}

SM_ContentManager* SM_DriveEngine::getContentMngr(bool opposite) const
{
    SM_ContentManager* cm;
    SM_GUI_EPanels currentPanel = static_cast<SM_GUI_EPanels> (SM_SettingsManager().currentPanel());

    if(currentPanel == ELeft)
    {
        if(opposite) cm = contentMngr[ERight].data();
        else cm = contentMngr[ELeft].data();
    }

    if(currentPanel == ERight)
    {
        if(opposite) cm = contentMngr[ELeft].data();
        else cm = contentMngr[ERight].data();
    }

    return cm;
}

void SM_DriveEngine::updatePanel(int panelNum, bool initLoad)
{
    SM_SettingsManager settingsManager;
    SM_GUI_EPanels panelId = static_cast <SM_GUI_EPanels> (panelNum);
    QString disc;

    settingsManager.setInitialLoading(initLoad);
    settingsManager.setCurrentPanel(panelNum);

    disc = settingsManager.accountDisc(settingsManager.currentAccount(panelNum));
    disc += QString(":");
    disc += QDir::toNativeSeparators("/");

    filesUI->getPanelLabel(panelId)->setText(disc + settingsManager.currentFolderPath(panelNum));
    getContentMngr()->setPathesURLs(settingsManager.pathesURLs(panelNum));

    contentMngr[panelNum]->setPanel(filesViews[panelNum]->getFileView(), filesViews[panelNum]->getpanelNum());
    contentMngr[panelNum]->get(settingsManager.currentFolderURL(panelNum));

    getFilePanel(panelId)->fillComboBox(settingsManager.accountsWithLetters(), settingsManager.currentAccount(panelNum));
}

void SM_DriveEngine::slotFirstPanelIsLoaded(void)
{
    updatePanel(RIGHT_PANEL_VALUE, false);
}

void SM_DriveEngine::slotAccountChanged(int panelNum, const QString &accountName)
{
    SM_SettingsManager settingsManager;

    if(settingsManager.currentAccount(panelNum) != accountName)
    {
        settingsManager.setCurrentAccount(panelNum, accountName);
        updatePanel(panelNum, false);
    }
}

SM_CheckUI* SM_DriveEngine::getCheckUI(void) const
{
    return checkUI.data();
}

SM_ContentUI* SM_DriveEngine::getfilesUI(void) const
{
    return filesUI.data();
}

SM_FilesTransferUI* SM_DriveEngine::getfilesTransferUI(void) const
{
    return filesTransferUI.data();
}

QWidget* SM_DriveEngine::getParent(void) const
{
    return parent;
}