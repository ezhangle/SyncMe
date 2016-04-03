#ifndef SHARE_H
#define SHARE_H

#include "../../Network/SM_NetworkManager.h"
#include "../../GUI/SM_Items.h"

class SM_Share : public SM_NetworkManager
{
    Q_OBJECT
public:
    SM_Share(QObject *parent = 0);

public:
    void file();
};

#endif // SHARE_H
