#ifndef DELETE_H
#define DELETE_H

#include "../SM_NetworkManager.h"
#include "SM_Operation.h"
#include "../../GUI/SM_Items.h"

class SM_Delete : public SM_NetworkManager, public SM_Operation
{
    Q_OBJECT
public:
    SM_Delete(QObject *parent = 0);
    void item(const SM_Items::Data &source);

protected slots:
    void slotReplyFinished(QNetworkReply*);
};

#endif // DELETE_H
