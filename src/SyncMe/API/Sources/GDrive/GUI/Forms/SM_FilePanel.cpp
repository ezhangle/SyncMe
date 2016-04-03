#include "../../../../Headers/GDrive/GUI/Forms/SM_FilePanel.h"
//#include "ui_filepanel.h"
#include "../../../../Headers/GDrive/Share/SM_Defs.h"
#include "../../../../Headers/GDrive/Share/SM_Debug.h"
#include <QApplication>

SM_FilePanel::SM_FilePanel(int pn, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilePanel),
    panelNum(pn)
{    
    init();
}

void SM_FilePanel::init(void)
{
    ui->setupUi(this);

    ui->fileView->header()->setSectionsClickable(true);

    ui->fileView->header()->resizeSection(0, 250);
    ui->fileView->header()->resizeSection(1, 60);
    ui->fileView->header()->resizeSection(2, 150);

    accountsToolBar = new SM_ToolBar;
    ui->verticalLayout->insertWidget(0, accountsToolBar);

    accountsComboBox = new SM_ComboBox;
    accountsToolBar->addWidget(accountsComboBox);

    connect(accountsComboBox, SIGNAL(activated(const QString&)), SLOT(slotActivated(const QString&)));
}

SM_FilePanel::~SM_FilePanel()
{
    delete ui;
}

void SM_FilePanel::slotActivated(const QString &text)
{
    int beginPos = text.indexOf(ACCOUNT_SEPARATOR_BEGIN) + ACCOUNT_SEPARATOR_BEGIN.length();
    int length = text.lastIndexOf(ACCOUNT_SEPARATOR_END) - beginPos;

    emit signalAccountChanged(panelNum, text.mid(beginPos, length));
}

QTreeWidget* SM_FilePanel::getFileView(void) const
{
    return ui->fileView;
}

QLabel* SM_FilePanel::getPathLabel(void) const
{
    return ui->pathLabel;
}

SM_ToolBar* SM_FilePanel::getAccountsToolBar(void) const
{
    return accountsToolBar;
}

SM_ComboBox* SM_FilePanel::getAccountsComboBox(void) const
{
    return accountsComboBox;
}

void SM_FilePanel::fillComboBox(QMap<QString, QString> accountsMap, const QString &currentAccount)
{
    QStringList keys(accountsMap.keys());

    accountsComboBox->clear();

    for(int i = 0; i < keys.count(); ++i)
    {
        QString discLetter(keys[i]);

        discLetter = discLetter.rightJustified(2,' ');
        discLetter = discLetter.leftJustified(6, ' ');

        accountsComboBox->addItem(discLetter + ACCOUNT_SEPARATOR_BEGIN + accountsMap[keys[i]] + ACCOUNT_SEPARATOR_END);
        accountsComboBox->setItemIcon(i, QIcon(QApplication::style()->standardIcon(QStyle::SP_DriveFDIcon)));

        if(currentAccount == accountsMap[keys[i]] && accountsComboBox->currentIndex() != i)
        {
          accountsComboBox->setCurrentIndex(i);
        }
    }  

    accountsComboBox->setMinimumWidth(80);
}

int SM_FilePanel::getpanelNum(void) const
{
    return panelNum;
}
