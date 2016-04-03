#include "../../../../Headers/GDrive/GUI/Forms/SM_SettingsDialog.h"
#include <QFileDialog>
#include <QDir>

SM_SettingsDialog::SM_SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(browseDir()));
}

SM_SettingsDialog::~SM_SettingsDialog()
{
    delete ui;
}

void SM_SettingsDialog::browseDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Especificar directorio"), ui->directoryLineEdit->text());

    if(dir.isEmpty())
    {
        return;
    }

    ui->directoryLineEdit->setText(dir);
}

QString SM_SettingsDialog::directoryPath() const
{
    return QDir::toNativeSeparators(ui->directoryLineEdit->text());
}

void SM_SettingsDialog::setDirectoryPath(const QString &dir)
{
    ui->directoryLineEdit->setText(QDir::toNativeSeparators(dir));
}
