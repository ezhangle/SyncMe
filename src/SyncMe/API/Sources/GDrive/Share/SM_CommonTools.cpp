#include "../../../Headers/GDrive/Share/SM_CommonTools.h"
#include "../../../Headers/GDrive/GUI/Forms/SM_GDrive_MainWindow.h"
#include "../../../Headers/GDrive/Share/SM_Defs.h"
#include "../../../Headers/GDrive/Settings/SM_SettingsManager.h"
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include "../../../Headers/GDrive/Share/SM_Debug.h"

void SM_CommonTools::setHeader(const QString &accessToken, QNetworkRequest &request)
{
    request.setRawHeader("User-Agent", SM_SettingsManager().applicationName().toLatin1());
    request.setRawHeader("GData-Version", "3.0");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(accessToken).toLatin1());
}

void SM_CommonTools::msg(const QString &text, const QString &title, QWidget *parent)
{
    QMessageBox msgBox(parent);

    msgBox.setText(text);
    msgBox.setWindowTitle(title);
    msgBox.exec();
}

int SM_CommonTools::errorMsg(const QString &caption, const QString &text, QWidget *parent)
{
    return QMessageBox::critical(parent, caption, text, QMessageBox::Ok);
}

void SM_CommonTools::logToFile(const QString &fileName, const QByteArray &bytes)
{
    QFile file(fileName);

    file.open(QIODevice::WriteOnly);
    file.write(bytes);
    file.close();
}

QByteArray SM_CommonTools::loadFromFile(const QString &fileName)
{
    QFile file(fileName);

    file.open(QIODevice::ReadOnly);
    QByteArray arr = file.readAll();
    file.close();

    return arr;
}

bool SM_CommonTools::fileFromURLExists(const QString &url)
{
    QFileInfo fi(url);
    QFile file(fi.fileName());

    return file.exists();
}

QString SM_CommonTools::getFileNameFromURL(const QString &url)
{
  QFileInfo fi(url);
  return fi.fileName();
}

QString SM_CommonTools::getIDFromURL(const QString &url)
{
    int backParamNum = 1;

    QStringList queryStrs(url.split("/"));

    if(queryStrs[queryStrs.count() - 1].contains(QRegExp("contents")))
    {
        backParamNum = 2;
    }

    QString lastParam(queryStrs[queryStrs.count() - backParamNum]);

    queryStrs = lastParam.split("%3A");

    return queryStrs[queryStrs.count()  - 1];
}
