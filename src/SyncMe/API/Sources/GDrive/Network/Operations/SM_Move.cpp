#include "SM_Move.h"

SM_Move::SM_Move(QObject *parent) :
    QObject(parent)
{
}

void SM_Move::item(const SM_Items::Data &source, const QString &destFolderUrl)
{
    copy.file(source, destFolderUrl);
    connect(&copy, SIGNAL(fileCopied(SM_Items::Data&)), this, SLOT(slotFileCopied(SM_Items::Data&)));
}

void SM_Move::slotFileCopied(SM_Items::Data &source)
{
    del.item(source);
}