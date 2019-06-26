#-------------------------------------------------
#
# Project created by QtCreator 2018-04-17T11:01:14
#
#-------------------------------------------------

QT       += core gui sql network
QT       += positioning

CONFIG   += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "WEB-SBI V08"
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_FILE = websbi.rc

SOURCES += \
        main.cpp \
        multishipcontroller.cpp \
        calculateshiplocation.cpp \
        amqpclient/communicator/communicator.cpp \
        amqpclient/shsclient/amqpserversettings.cpp \
        amqpclient/shsclient/shsclient.cpp \
        DbServerSettings/dbserversettings.cpp \
        displayreceivedaisdata.cpp \
        mainwindow.cpp \
        multishipaisdata.cpp \
        routearray.cpp \
        usersdetails.cpp \
        userdata.cpp \
        userdatabase.cpp \
        sensorinterface.cpp \
        dbreader.cpp \
        filteruserdata.cpp \
        dbwriter.cpp \
        connectionstatus.cpp

HEADERS += \
        multishipcontroller.h \
        calculateshiplocation.h \
        amqpclient/receiver.h \
        amqpclient/transmitter.h \
        amqpclient/communicator/communicator.h \
        amqpclient/shsclient/amqpserversettings.h \
        amqpclient/shsclient/shsclient.h \
        DbServerSettings/dbserversettings.h \
        displayreceivedaisdata.h \
        mainwindow.h \
        multishipaisdata.h \
        routearray.h \
        usersdetails.h \
        userdata.h \
        userdatabase.h \
        dbserversettings.h \
        sensorinterface.h \
        dbreader.h \
        filteruserdata.h \
        dbwriter.h \
        connectionstatus.h

FORMS += \
        multishipcontroller.ui \
        calculateshiplocation.ui \
        amqpclient/shsclient/amqpserversettings.ui \
        DbServerSettings/dbserversettings.ui \
        displayreceivedaisdata.ui \
        mainwindow.ui \
        multishipaisdata.ui \
        routearray.ui \
        usersdetails.ui \
        userdata.ui \
        userdatabase.ui \
        dbserversettings.ui \
        sensorinterface.ui \
        connectionstatus.ui

INCLUDEPATH += $$PWD/headers/
include(amqpclient/qamqp/qamqp.pri)

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/Qwt-6.1.3/lib/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/Qwt-6.1.3/lib/ -lqwtd
INCLUDEPATH +=  $$PWD/Qwt-6.1.3/include

LIBS += -L$$PWD/AISDeocder/lib/ -lAISDecoderLib
INCLUDEPATH += $$PWD/AISDeocder/include

DISTFILES += \
    websbi.rc
