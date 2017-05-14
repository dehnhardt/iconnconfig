#-------------------------------------------------
#
# Project created by QtCreator 2017-03-10T21:28:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MioConfig
TEMPLATE = app

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# RTMidi defines
unix:!macx: DEFINES += __LINUX_ALSA__
macx: DEFINES += __MACOSX_CORE__
win32: DEFINES += __WINDOWS_MM__

DEFINES += __MIO_SIMULATE__
DEFINES += __MIO_DEBUG__

DEFINES += __RTMIDI_DEBUG__
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS = tr/mioconfig_de_DE.ts

SOURCES +=\
        src/RtMidi.cpp \
    src/main.cpp \
    src/miomain.cpp \
    src/device.cpp \
    src/sysex/midi.cpp \
    src/devicedetection.cpp \
    src/config/configuration.cpp \
    src/devicedetectionprocessor.cpp \
    src/sysex/sysexmessage.cpp \
    src/deviceselectiontablemodel.cpp \
    src/widgets/portsettingwidget.cpp \
    src/widgets/deviceinfowidget.cpp \
    src/widgets/portswidget.cpp \
    src/sysex/getcommands.cpp \
    src/widgets/multiinfowidget.cpp \
    src/widgets/centralwidget.cpp \
    src/sysex/getinfolist.cpp \
    src/widgets/infotablewidget.cpp \
    src/sysex/retsetinfo.cpp \
    src/sysex/getinfo.cpp \
    src/sysex/retcommandlist.cpp \
    src/sysex/getdevice.cpp \
    src/sysex/retinfolist.cpp

INCLUDEPATH += /usr/include/libusb-1.0/

HEADERS  += src/miomain.h \
    src/RtMidi.h \
    src/device.h \
    src/sysex/midi.h \
    src/devicedetection.h \
    src/config/configuration.h \
    src/devicedetectionprocessor.h \
    src/sysex/sysexmessage.h \
    src/deviceselectiontablemodel.h \
    src/events/events.h \
    src/device.h \
    src/widgets/portsettingwidget.h \
    src/widgets/deviceinfowidget.h \
    src/widgets/portswidget.h \
    src/sysex/getcommands.h \
    src/widgets/multiinfowidget.h \
    src/widgets/centralwidget.h \
    src/sysex/getinfolist.h \
    src/widgets/infotablewidget.h \
    src/sysex/retsetinfo.h \
    src/sysex/getinfo.h \
    src/sysex/retcommandlist.h \
    src/sysex/getdevice.h \
    src/sysex/retinfolist.h

FORMS    += src/miomain.ui \
    src/devicedetection.ui \
    src/widgets/multiinfowidget.ui

unix:!macx: LIBS += -lasound \
                    -lpthread \
                    -lusb-1.0

RESOURCES += \
    translations.qrc

DISTFILES += \
    tr/mioconfig_de_DE.qm \
    tr/mioconfig_de_DE.ts
