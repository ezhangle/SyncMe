#-------------------------------------------------
#
# Definiciones generales y dependencias
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SyncMe
TEMPLATE = app

CONFIG += C++11


SOURCES += main.cpp\
        mainwindow.cpp \
    API/Sources/Dropbox/SM_qdropbox.cpp \
    API/Sources/GDrive/Core/SM_DriveEngine.cpp \
    API/Sources/GDrive/Core/main.cpp \
    SM_AccountsManager.cpp \
    SM_CheckUI.cpp \
    SM_FilesTransferUI.cpp \
    SM_FilesUI.cpp \
    SM_FoldersUI.cpp \
    SM_ItemInfo.cpp \
    SM_OperationsUI.cpp \
    API/Sources/GDrive/Network/SM_Auth.cpp \
    API/Sources/GDrive/Network/SM_ContentManager.cpp \
    API/Sources/GDrive/Network/SM_DownloadManager.cpp \
    API/Sources/GDrive/Network/SM_FilesManager.cpp \
    API/Sources/GDrive/Network/SM_FoldersManager.cpp \
    API/Sources/GDrive/Network/SM_NetworkManager.cpp \
    API/Sources/GDrive/Network/SM_OperationsManager.cpp \
    API/Sources/GDrive/Network/SM_Queries.cpp \
    API/Sources/GDrive/Network/SM_ResManager.cpp \
    API/Sources/GDrive/Network/SM_UploadManager.cpp \
    API/Sources/GDrive/Parsers/SM_JSONParser.cpp \
    API/Sources/GDrive/Parsers/SM_XMLHandler.cpp \
    API/Sources/GDrive/Parsers/SM_XMLParser.cpp \
    API/Sources/GDrive/Settings/SM_AccountInfo.cpp \
    API/Sources/GDrive/Settings/SM_SettingsManager.cpp

HEADERS  += mainwindow.h \
    API/Headers/Dropbox/SM_dropbox.h \
    API/Headers/Dropbox/SM_dropbox_global.h \
    API/Headers/Dropbox/SM_dropbox_account.h \
    API/Headers/Dropbox/SM_dropbox_deltaresponse.h \
    API/Headers/Dropbox/SM_dropbox_file.h \
    API/Headers/Dropbox/SM_dropbox_fileinfo.h \
    API/Headers/Dropbox/SM_dropbox_json.h \
    API/Headers/Dropbox/SM_qdropbox.h \
    API/Headers/GDrive/Core/SM_DriveEngine.h \
    API/Headers/GDrive/Core/SM_EventHandler.h \
    API/Headers/GDrive/GUI/Controls/SM_ComboBox.h \
    API/Headers/GDrive/GUI/Controls/SM_ToolBar.h \
    SM_AdditionalFolderManager.h \
    SM_CheckUI.h \
    SM_FilesTransferUI.h \
    SM_FilesUI.h \
    SM_FoldersUI.h \
    SM_ItemInfo.h \
    SM_OperationsUI.h \
    API/Headers/GDrive/Network/SM_Auth.h \
    API/Headers/GDrive/Network/SM_ContentManager.h \
    API/Headers/GDrive/Network/SM_DownloadManager.h \
    API/Headers/GDrive/Network/SM_FilesManager.h \
    API/Headers/GDrive/Network/SM_FoldersManager.h \
    API/Headers/GDrive/Network/SM_NetworkManager.h \
    API/Headers/GDrive/Network/SM_OperationsManager.h \
    API/Headers/GDrive/Network/SM_Queries.h \
    API/Headers/GDrive/Network/SM_ResManager.h \
    API/Headers/GDrive/Network/SM_UploadManager.h \
    API/Headers/GDrive/Parsers/SM_JSONParser.h \
    API/Headers/GDrive/Parsers/SM_XMLHandler.h \
    API/Headers/GDrive/Parsers/SM_XMLParser.h \
    API/Headers/GDrive/Settings/SM_AccountInfo.h \
    API/Headers/GDrive/Settings/SM_SettingsManager.h \
    API/Headers/GDrive/Share/SM_CommonTools.h

FORMS    += mainwindow.ui \
    API/Headers/GDrive/GUI/Forms/AdditionalFolders.ui \
    API/Headers/GDrive/GUI/Forms/AuthDialog.ui \
    API/Headers/GDrive/GUI/Forms/CreateFolderDialog.ui \
    API/Headers/GDrive/GUI/Forms/DriveTree.ui \
    API/Headers/GDrive/GUI/Forms/FilePanel.ui \
    API/Headers/GDrive/GUI/Forms/MainWindow.ui \
    API/Headers/GDrive/GUI/Forms/ProgressBarDialog.ui \
    API/Headers/GDrive/GUI/Forms/SettingsDialog.ui \
    API/Headers/GDrive/GUI/Forms/ShareDialog.ui

OTHER_FILES += \
    API/Headers/GDrive/Images/PLACEHOLDER
