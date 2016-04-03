#include "../../../../Headers/GDrive/Network/Operations/SM_Operation.h"
#include "../../../../Headers/GDrive/Core/SM_DriveEngine.h"
#include "../../../../Headers/GDrive/Network/SM_ContentManager.h"

SM_Operation::SM_Operation()
{
}

void SM_Operation::updatePanelContent(bool opposite)
{
    SM_ContentManager* cm(SDriveEngine::inst()->getContentMngr(opposite));
    cm->get(cm->getParentFolderUrl());
}
