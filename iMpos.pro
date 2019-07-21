#-------------------------------------------------
#
# Project created by QtCreator 2019-07-10T17:23:26
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iMpos
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        ConnectionSettingDialog/connectionsettingdialog.cpp \
        DataBases/databases.cpp \
        DataBases/databasesettings.cpp \
        DataBases/options.cpp \
        LoggingCategories/loggingcategories.cpp \
        LoginDialog/logindialog.cpp \
        SettingsDialog/settingsdialog.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        ConnectionSettingDialog/connectionsettingdialog.h \
        DataBases/databases.h \
        DataBases/databasesettings.h \
        DataBases/options.h \
        LoggingCategories/loggingcategories.h \
        LoginDialog/logindialog.h \
        SettingsDialog/settingsdialog.h \
        mainwindow.h

FORMS += \
        ConnectionSettingDialog/connectionsettingdialog.ui \
        LoginDialog/logindialog.ui \
        SettingsDialog/settingsdialog.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    resource.qrc
