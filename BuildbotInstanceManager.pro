#-------------------------------------------------
#
# Project created by QtCreator 2014-01-05T12:53:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BuildbotInstanceManager
TEMPLATE = app
CONFIG += c++11
RC_FILE = Resources/win_res.rc
ICON = Resources/icon.icns
QMAKE_INFO_PLIST = Resources/Info.plist

unix:!mac { LIBS += -Wl,-rpath=. }

SOURCES += main.cpp\
    BuildbotInstanceManagerGUI.cpp \
    BuildbotInstance.cpp \
    BuildbotInstanceScanner.cpp \
    AppSettings.cpp \
    SettingsDialog.cpp \
    Utility.cpp \
    Buildmaster.cpp \
    Buildslave.cpp \
    BuildbotInstanceObserver.cpp \
    BuildmasterListAdapter.cpp \
    BuildslaveListAdapter.cpp

HEADERS  += \
    BuildbotInstanceManagerGUI.h \
    BuildbotInstance.h \
    BuildbotInstanceScanner.h \
    AppSettings.h \
    SettingsDialog.h \
    Utility.h \
    Buildmaster.h \
    Buildslave.h \
    BuildbotInstanceObserver.h \
    BuildmasterListAdapter.h \
    BuildslaveListAdapter.h

FORMS    += \
    BuildbotInstanceManagerGUI.ui \
    SettingsDialog.ui

RESOURCES += \
    Resources.qrc
