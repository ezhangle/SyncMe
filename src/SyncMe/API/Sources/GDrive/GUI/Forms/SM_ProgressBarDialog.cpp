#include "../../../../Headers/GDrive/GUI/Forms/SM_ProgressBarDialog.h"
#include "ui_SM_ProgressBarDialog.h"

SM_ProgressBarDialog::SM_ProgressBarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::progressBarDialog)
{
    ui->setupUi(this);
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(slotCancel()));
}

SM_ProgressBarDialog::~SM_ProgressBarDialog()
{
    delete ui;
}

void SM_ProgressBarDialog::slotCancel()
{
    hide();
    emit signalProgressCanceled();
}

void SM_ProgressBarDialog::setMaximum(qint64 max)
{
 ui->progressBar->setMaximum(max);
}

void SM_ProgressBarDialog::setValue(qint64 val)
{
  ui->progressBar->setValue(val);
}

void SM_ProgressBarDialog::setText(const QString& text)
{
  ui->infoLbel->setText(text);
}
