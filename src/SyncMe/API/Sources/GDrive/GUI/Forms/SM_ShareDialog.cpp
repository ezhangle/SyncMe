#include "SM_ShareDialog.h"
#include "ui_sharedialog.h"
#include <QUrl>
#include <QFile>

SM_ShareDialog::SM_ShareDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShareDialog)
{
    ui->setupUi(this);
}

SM_ShareDialog::~SM_ShareDialog()
{
    delete ui;
}