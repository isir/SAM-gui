#-------------------------------------------------
#
# Project created by QtCreator 2019-04-11T16:11:11
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Monitoring
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

CONFIG += c++1z

SOURCES += \
        src/ui/mosquittopp/client.cpp \
        src/ui/mosquittopp/connect_factory.cpp \
        src/ui/mosquittopp/connect_helper.cpp \
        src/ui/mosquittopp/library_wrapper.cpp \
        src/ui/mosquittopp/message.cpp \
        src/ui/mosquittopp/subscription.cpp \
        src/ui/mosquittopp/subscription_factory.cpp \
        src/ui/mqtt_client_wrapper.cpp \
        src/ui/mqttconnect.cpp \
        src/main.cpp \
        src/ui/logdisplay.cpp \
        src/ui/mainwindow.cpp \
        src/ui/menudisplay.cpp \
        src/ui/param/param_viewer.cpp \
        src/ui/systemdisplay.cpp \
        src/ui/topicplotter.cpp

HEADERS += \
        src/ui/mosquittopp/client.h \
        src/ui/mosquittopp/connect_factory.h \
        src/ui/mosquittopp/connect_helper.h \
        src/ui/mosquittopp/library_version.h \
        src/ui/mosquittopp/library_wrapper.h \
        src/ui/mosquittopp/message.h \
        src/ui/mosquittopp/subscription.h \
        src/ui/mosquittopp/subscription_factory.h \
        src/ui/mqtt_client_wrapper.h \
        src/ui/mqttconnect.h \
        src/ui/logdisplay.h \
        src/ui/mainwindow.h \
        src/ui/menudisplay.h \
        src/ui/param/param_viewer.h \
        src/ui/systemdisplay.h \
        src/ui/topicplotter.h

FORMS += \
        src/ui/mqttconnect.ui \
        src/ui/logdisplay.ui \
        src/ui/mainwindow.ui \
        src/ui/menudisplay.ui \
        src/ui/param/param_viewer.ui \
        src/ui/systemdisplay.ui \
        src/ui/topicplotter.ui

INCLUDEPATH += "src/"

LIBS += -lmosquitto

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
