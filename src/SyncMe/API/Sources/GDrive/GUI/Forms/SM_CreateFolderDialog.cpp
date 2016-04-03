#include "SM_CreateFolderDialog.h"
#include "ui_createfolderdialog.h"

SM_CreateFolderDialog::SM_CreateFolderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateFolderDialog)
{
    ui->setupUi(this);
    ui->nameEdit->setFocus();

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotAccept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(slotReject()));
    connect(this, SIGNAL(finished(int)), this, SLOT(slotFinished(int)));
}

SM_CreateFolderDialog::~SM_CreateFolderDialog()
{
    delete ui;
}

void SM_CreateFolderDialog::slotAccept()
{
    emit signalAccept(ui->nameEdit->text());
}

void SM_CreateFolderDialog::slotReject()
{
    emit signalReject();
}

void SM_CreateFolderDialog::slotFinished(int result)
{
    emit signalFinished(result);
}

void SM_CreateFolderDialog::setNameLabel()
{
    return ui->nameLabel->setText("Crear nueva carpeta:");
}