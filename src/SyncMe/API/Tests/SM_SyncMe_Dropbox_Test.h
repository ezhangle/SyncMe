#ifndef SM_SYNCME_DROPBOX_TEST_H
#define SM_SYNCME_DROPBOX_TEST_H

#include <QtTest>  // Usando pruebas unitarias de Qt5
#include <QDesktopServices>
#include <QThread>
#include "../Headers/Dropbox/SM_dropbox.h"
#include "SM_Dropbox_Keys_Data.h"

class SM_SyncMe_Dropbox_Test : public QObject
{
    Q_OBJECT

public:
   SM_SyncMe_Dropbox_Test();

private Q_SLOTS:

   /* Probando la clase SM_QDropbox */
    void jsonCase1();
    void jsonCase2();
    void jsonCase3();
    void jsonCase4();
    void jsonCase5();
    void jsonCase6();
    void jsonCase7();
    void jsonCase8();
    void jsonCase9();
    void jsonCase10();
    void jsonCase11();
    void jsonCase12();
    void jsonCase13();
    void jsonCase14();
    void jsonCase15();

  /* Probando la clase SM_QDropbox */
    void dropboxCase1();
    void dropboxCase2();

private:
    void authorizeApplication(SM_QDropbox *d);
    bool connectDropbox(SM_QDropbox* d, SM_QDropbox::OAuthMethod m);
};


#endif // SM_SYNCME_DROPBOX_TEST_H
