#-------------------------------------------------
#
# Prototipo de SyncMe usando el motor de Google Drive
#
#-------------------------------------------------

QT += core gui webkit network xml
PKGCONFIG += openssl # Libreria OpenSSL de desarrollo funcionando en Linux (En windows requiere poner el DLL manualmente en el directorio del .exe)
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

QMAKE_CXXFLAGS += -O3
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
QMAKE_LFLAGS += -fprofile-arcs -ftest-coverage

LIBS += \
	-lgcov

# Icono en Windows
#ifdef Q_OS_WIN32
RC_FILE = syncme.rc
#endif

# Icono en MAC OS X
#ifdef Q_OS_MAC
ICON = Resources/SyncMe.icns
#endif

# Icono en Symbian
#ifdef Q_OS_SYMBIAN
ICON = Resources/SyncMe.svg
#endif


TARGET = SyncMe
TEMPLATE = app

SOURCES += API/Sources/GDrive/Core/main.cpp \
    API/Sources/GDrive/Core/SM_DriveEngine.cpp \
    API/Sources/GDrive/GUI/Controls/SM_ComboBox.cpp \
    API/Sources/GDrive/GUI/Controls/SM_Spacer.cpp \
    API/Sources/GDrive/GUI/Controls/SM_ToolBar.cpp \
    API/Sources/GDrive/GUI/Forms/SM_AuthDialog.cpp \
    API/Sources/GDrive/GUI/Forms/SM_CreateFolderDialog.cpp \
    API/Sources/GDrive/GUI/Forms/SM_FilePanel.cpp \
    API/Sources/GDrive/GUI/Forms/SM_GDrive_MainWindow.cpp \
    API/Sources/GDrive/GUI/Forms/SM_ProgressBarDialog.cpp \
    API/Sources/GDrive/GUI/Tools/SM_PainterHelper.cpp \
    API/Sources/GDrive/GUI/SM_AccountsManager.cpp \
    API/Sources/GDrive/GUI/SM_CheckUI.cpp \
    API/Sources/GDrive/GUI/SM_ContentUI.cpp \
    API/Sources/GDrive/GUI/SM_FilesTransferUI.cpp \
    API/Sources/GDrive/GUI/SM_Items.cpp \
    API/Sources/GDrive/GUI/Forms/SM_SettingsDialog.cpp \
    API/Sources/GDrive/GUI/Forms/SM_ShareDialog.cpp \
    API/Sources/GDrive/Network/Operations/SM_Children.cpp \
    API/Sources/GDrive/Network/Operations/SM_Copy.cpp \
    API/Sources/GDrive/Network/Operations/SM_Create.cpp \
    API/Sources/GDrive/Network/Operations/SM_Delete.cpp \
    API/Sources/GDrive/Network/Operations/SM_Download.cpp \
    API/Sources/GDrive/Network/Operations/SM_Move.cpp \
    API/Sources/GDrive/Network/Operations/SM_Operation.cpp \
    API/Sources/GDrive/Network/Operations/SM_OperationsManager.cpp \
    API/Sources/GDrive/Network/Operations/SM_Rename.cpp \
    API/Sources/GDrive/Network/Operations/SM_Share.cpp \
    API/Sources/GDrive/Network/Operations/SM_Upload.cpp \
    API/Sources/GDrive/Network/SM_Auth.cpp \
    API/Sources/GDrive/Network/SM_ContentManager.cpp \
    API/Sources/GDrive/Network/SM_DownloadFileManager.cpp \
    API/Sources/GDrive/Network/SM_NetworkManager.cpp \
    API/Sources/GDrive/Network/SM_Queries.cpp \
    API/Sources/GDrive/Network/SM_ResManager.cpp \
    API/Sources/GDrive/Network/SM_UploadFileManager.cpp \
    API/Sources/GDrive/Parsers/SM_JSONParser.cpp \
    API/Sources/GDrive/Parsers/SM_XMLHandler.cpp \
    API/Sources/GDrive/Parsers/SM_XMLParser.cpp \
    API/Sources/GDrive/Settings/SM_AccountInfo.cpp \
    API/Sources/GDrive/Settings/SM_SettingsManager.cpp \
    API/Sources/GDrive/Share/SM_CommonTools.cpp

HEADERS += \
    API/Headers/GDrive/Core/SM_DriveEngine.h \
    API/Headers/GDrive/GUI/Controls/SM_ComboBox.h \
    API/Headers/GDrive/GUI/Controls/SM_Spacer.h \
    API/Headers/GDrive/GUI/Controls/SM_ToolBar.h \
    API/Headers/GDrive/GUI/Forms/SM_AuthDialog.h \
    API/Headers/GDrive/GUI/Forms/SM_CreateFolderDialog.h \
    API/Headers/GDrive/GUI/Forms/SM_FilePanel.h \
    API/Headers/GDrive/GUI/Forms/SM_GDrive_MainWindow.h \
    API/Headers/GDrive/GUI/Forms/SM_ProgressBarDialog.h \
    API/Headers/GDrive/GUI/Forms/SM_SettingsDialog.h \
    API/Headers/GDrive/GUI/Forms/SM_ShareDialog.h \
    API/Headers/GDrive/GUI/Tools/SM_PainterHelper.h \
    API/Headers/GDrive/GUI/SM_AccountsManager.h \
    API/Headers/GDrive/GUI/SM_CheckUI.h \
    API/Headers/GDrive/GUI/SM_ContentUI.h \
    API/Headers/GDrive/GUI/SM_FilesTransferUI.h \
    API/Headers/GDrive/GUI/SM_Items.h \
    API/Headers/GDrive/Network/Operations/SM_Children.h \
    API/Headers/GDrive/Network/Operations/SM_Copy.h \
    API/Headers/GDrive/Network/Operations/SM_Create.h \
    API/Headers/GDrive/Network/Operations/SM_Delete.h \
    API/Headers/GDrive/Network/Operations/SM_Download.h \
    API/Headers/GDrive/Network/Operations/SM_Move.h \
    API/Headers/GDrive/Network/Operations/SM_Operation.h \
    API/Headers/GDrive/Network/Operations/SM_OperationsManager.h \
    API/Headers/GDrive/Network/Operations/SM_Rename.h \
    API/Headers/GDrive/Network/Operations/SM_Share.h \
    API/Headers/GDrive/Network/Operations/SM_Upload.h \
    API/Headers/GDrive/Network/SM_Auth.h \
    API/Headers/GDrive/Network/SM_ContentManager.h \
    API/Headers/GDrive/Network/SM_NetworkManager.h \
    API/Headers/GDrive/Network/SM_Queries.h \
    API/Headers/GDrive/Network/SM_ResManager.h \
    API/Headers/GDrive/Network/SM_UploadFileManager.h \
    API/Headers/GDrive/Network/SM_DownloadFileManager.h \
    API/Headers/GDrive/Parsers/SM_JSONParser.h \
    API/Headers/GDrive/Parsers/SM_XMLHandler.h \
    API/Headers/GDrive/Parsers/SM_XMLParser.h \
    API/Headers/GDrive/Settings/SM_AccountInfo.h \
    API/Headers/GDrive/Settings/SM_SettingsManager.h \
    API/Headers/GDrive/Share/SM_CommonTools.h \
    API/Headers/GDrive/Share/SM_Debug.h \
    API/Headers/GDrive/Share/SM_Defs.h \
    API/Headers/GDrive/Share/SM_Enums.h \
    API/Headers/GDrive/Share/SM_TSingleton_Item.h

FORMS += \
    API/Headers/GDrive/GUI/Forms/SM_AuthDialog.ui \
    API/Headers/GDrive/GUI/Forms/SM_CreateFolderDialog.ui \
    API/Headers/GDrive/GUI/Forms/SM_FilePanel.ui \
    API/Headers/GDrive/GUI/Forms/SM_GDrive_MainWindow.ui \
    API/Headers/GDrive/GUI/Forms/SM_ProgressBarDialog.ui \
    API/Headers/GDrive/GUI/Forms/SM_SettingsDialog.ui \
    API/Headers/GDrive/GUI/Forms/SM_ShareDialog.ui

RESOURCES += resource.qrc

DISTFILES += \
    syncme.rc
