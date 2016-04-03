#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>
#include "network/SM_Auth.h"

namespace Ui {
class AuthDialog;
}

class SM_AuthDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SM_AuthDialog(QWidget *parent = 0);
    ~SM_AuthDialog();

signals:
    void signalTokens(const QString &accessToken, const QString &refreshToken);

private slots:
    void slotUrlChanged(const QUrl& url);
    void slotAuthResponse(const QString &accessToken, const QString &refreshToken);

private:
    void init(void);
    
private:
    Ui::AuthDialog *ui;
    QScopedPointer<SM_Auth> auth;
};

#endif // AUTHDIALOG_H
