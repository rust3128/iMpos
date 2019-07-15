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
        DataBases/connectiondialog.cpp \
        DataBases/databases.cpp \
        DataBases/databasesettings.cpp \
        DataBases/editconndialog.cpp \
        DataBases/optionsdata.cpp \
        LoggingCategories/loggingcategories.cpp \
        LoginDialog/logindialog.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        DataBases/connectiondialog.h \
        DataBases/databases.h \
        DataBases/databasesettings.h \
        DataBases/editconndialog.h \
        DataBases/optionsdata.h \
        LoggingCategories/loggingcategories.h \
        LoginDialog/logindialog.h \
        mainwindow.h

FORMS += \
        DataBases/connectiondialog.ui \
        DataBases/editconndialog.ui \
        LoginDialog/logindialog.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
