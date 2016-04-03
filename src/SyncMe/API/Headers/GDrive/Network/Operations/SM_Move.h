#ifndef MOVE_H
#define MOVE_H

#include <QObject>
#include "../../Network/Operations/SM_Copy.h"
#include "../../Network/Operations/SM_Delete.h"

class SM_Move : public QObject
{
    Q_OBJECT
public:
    SM_Move(QObject* parent = 0);

public:
    void item(const SM_Items::Data &source, const QString &destFolderUrl);

private slots:
    void slotFileCopied(SM_Items::Data &source);

private:
    SM_Copy copy;
    SM_Delete del;
};

#endif // MOVE_H
