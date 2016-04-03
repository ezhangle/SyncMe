#ifndef CHECK_H
#define CHECK_H

#include <QObject>

class SM_CheckUI: public QObject
{
    Q_OBJECT
public:
     explicit SM_CheckUI(QObject *parent = 0);

public:
    bool checkReg(void);

public slots:
    bool slotCheckWorkDir(bool showDlg = true);
};

#endif // CHECK_H
