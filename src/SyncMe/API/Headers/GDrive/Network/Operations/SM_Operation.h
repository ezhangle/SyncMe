#ifndef OPERATION_H
#define OPERATION_H

#include "../../Network/SM_Queries.h"

class SM_Operation
{
public:
    SM_Operation();

public:
    void updatePanelContent(bool opposite);

protected:
      SM_Queries queries;
};

#endif // OPERATION_H
