#include "SM_Share.h"
#include "share/SM_CommonTools.h"

SM_Share::SM_Share(QObject *parent) :
    SM_NetworkManager(parent)
{
}

void SM_Share::file()
{
  SM_CommonTools::msg("Esta funcionalidad no está implementada todavía", "SyncMe: Información");
}