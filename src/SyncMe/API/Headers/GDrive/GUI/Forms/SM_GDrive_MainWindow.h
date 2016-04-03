#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../../Share/SM_TSingleton_Item.h"
#include "../../Network/SM_Queries.h"
#include "../../Network/SM_Auth.h"

namespace Ui
{
class MainWindow;
}

class SM_DriveEngine;

typedef SM_TSingleton_Item<Ui::MainWindow> SUi;

class SM_GDrive_MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SM_GDrive_MainWindow(QWidget *parent = 0);
    ~SM_GDrive_MainWindow();

public:
    void init(void);

private:
    void setConnections(void);

private slots:
    void slotAccessTokenRequired(void);
    void slotAccountInfoReadyToUse(void);
    void slotAuthDialog(void);
    void slotColaboraDialog(void);
    void slotAuthResponse(const QString &accessToken);
    void slotTokens(const QString &accessToken, const QString &refreshToken);

private:
    SM_Auth *auth;
};

#endif // MAINWINDOW_H
