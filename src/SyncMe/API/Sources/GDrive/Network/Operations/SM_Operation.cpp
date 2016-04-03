#include "SM_Operation.h"
#include "core/SM_DriveEngine.h"
#include "network/SM_ContentManager.h"

SM_Operation::SM_Operation()
{
}

void SM_Operation::updatePanelContent(bool opposite)
{
    SM_ContentManager* cm(SDriveEngine::inst()->getContentMngr(opposite));
    cm->get(cm->getParentFolderUrl());
}