#ifndef CHILDREN_H
#define CHILDREN_H

#include "network/SM_NetworkManager.h"
#include "SM_Operation.h"
#include "gui/SM_Items.h"

class SM_Children : public SM_NetworkManager,  public SM_Operation
{
    Q_OBJECT
public:    
    SM_Children(QObject *parent = 0);

protected slots:
     void slotReplyFinished();

public:
     void fetch();

};

#endif // CHILDREN_H
