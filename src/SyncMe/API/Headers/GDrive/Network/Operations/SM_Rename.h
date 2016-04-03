#ifndef RENAME_H
#define RENAME_H

#include "../../Network/SM_NetworkManager.h"
#include "SM_Operation.h"
#include "../../GUI/SM_Items.h"

class SM_Rename : public SM_NetworkManager, public SM_Operation
{
    Q_OBJECT
public:
    SM_Rename(QObject *parent = 0);

protected slots:
    void slotPutFinished(void);

public:
    void item(const SM_Items::Data &source, const QString &newName);
};

#endif // RENAME_H
