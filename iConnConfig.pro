#-------------------------------------------------
#
# Project created by QtCreator 2017-03-10T21:28:32
#
#-------------------------------------------------


QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iconnconfig
TEMPLATE = app

CONFIG += c++14
QMAKE_CXXFLAGS += -std=c++14

# option for building a debian package
build_deb {
    DESTDIR = build
    INSTALLS += target
    INSTALLS.CONFIG += executable
    target.path += $$PREFIX/bin
    desktop.files += resources/iconnconfig.desktop
    desktop.path += $$PREFIX/share/applications
    INSTALLS += desktop
    icons.files += resources/iconnconfig.png
    icons.files += resources/iconnconfig.xpm
    icons.path += $$PREFIX/share/icons
    INSTALLS += icons
    message(creating makefile for deb-package - target_path: $$target.path )
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# RTMidi defines
unix:!macx: DEFINES += __LINUX_ALSA__
macx: DEFINES += __MACOSX_CORE__
win32: DEFINES += __WINDOWS_MM__

#DEFINES += __MIO_DEBUG__

#DEFINES += __RTMIDI_DEBUG__
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS = tr/iconnconfig_de_DE.ts \
               tr/iconnconfig_fr_FR.ts \
               tr/iconnconfig_en_EN.ts

SOURCES +=\
        src/RtMidi.cpp \
    src/main.cpp \
    src/device.cpp \
    src/sysex/getaudiochannelname.cpp \
    src/sysex/getmixerinputcontrol.cpp \
    src/sysex/getmixerinputcontrolvalue.cpp \
    src/sysex/getmixermetervalue.cpp \
    src/sysex/getmixeroutputcontrol.cpp \
    src/sysex/getmixeroutputcontrolvalue.cpp \
    src/sysex/getmixeroutputparm.cpp \
    src/sysex/midi.cpp \
    src/devicedetection.cpp \
    src/config/configuration.cpp \
    src/devicedetectionprocessor.cpp \
    src/sysex/retmixerinputcontrol.cpp \
    src/sysex/retmixermetervalue.cpp \
    src/sysex/retmixeroutputcontrol.cpp \
    src/sysex/retsetaudiochannelname.cpp \
    src/sysex/retsetmixerinputcontrolvalue.cpp \
    src/sysex/retsetmixeroutputcontrolvalue.cpp \
    src/sysex/retsetmixeroutputparm.cpp \
    src/sysex/sysexmessage.cpp \
    src/deviceselectiontablemodel.cpp \
    src/widgets/audiomixerchannelwidget.cpp \
    src/widgets/audiomixerinputchannelwidget.cpp \
    src/widgets/audiomixeroutputchannelwidget.cpp \
    src/widgets/controls/iconncalc.cpp \
    src/widgets/controls/pkmixerbutton.cpp \
    src/widgets/deviceinfowidget.cpp \
    src/widgets/portswidget.cpp \
    src/widgets/multiinfowidget.cpp \
    src/widgets/centralwidget.cpp \
    src/sysex/getinfolist.cpp \
    src/widgets/infotablewidget.cpp \
    src/sysex/retsetinfo.cpp \
    src/sysex/getinfo.cpp \
    src/sysex/retcommandlist.cpp \
    src/sysex/getdevice.cpp \
    src/sysex/retinfolist.cpp \
    src/sysex/getcommandlist.cpp \
    src/sysex/ack.cpp \
    src/sysex/getethernetportinfo.cpp \
    src/widgets/ethernetinfowidget.cpp \
    src/sysex/retsetethernetportinfo.cpp \
    src/sysex/getmidiinfo.cpp \
    src/sysex/retsetmidiportinfo.cpp \
    src/sysex/retsetmidiinfo.cpp \
    src/widgets/multiinfolistentry.cpp \
    src/widgets/controls/portbutton.cpp \
    src/widgets/portroutingwidget.cpp \
    src/sysex/getmidiportinfo.cpp \
    src/sysex/getmidiportroute.cpp \
    src/sysex/retsetmidiportroute.cpp \
    src/sysex/saverestore.cpp \
    src/sysex/getsaverestorelist.cpp \
    src/sysex/retsaverestorelist.cpp \
    src/widgets/controls/ipaddressinput.cpp \
    src/sysex/communicationexception.cpp \
    src/sysex/protocolexception.cpp \
    src/aboutdialog.cpp \
    src/iconnconfigmain.cpp \
    src/sysex/getgizmocount.cpp \
    src/sysex/retgizmocount.cpp \
    src/sysex/getmidiportfilter.cpp \
    src/sysex/retsetmidiportfilter.cpp \
    src/widgets/portfilterwidget.cpp \
    src/widgets/controls/midicontrollerselect.cpp \
    src/widgets/portfiltersettingswidget.cpp \
    src/widgets/controls/midicontrollercombodelegate.cpp \
    src/widgets/portinfowidget.cpp \
    src/widgets/portremapsettingswidget.cpp \
    src/widgets/portremapwidget.cpp \
    src/sysex/retsetmidiportremap.cpp \
    src/sysex/getmidiportremap.cpp \
    src/widgets/controls/midichannelcombodelegate.cpp \
    src/widgets/globalaudioconfigurationwidget.cpp \
    src/widgets/audioportswidget.cpp \
    src/sysex/getaudioportparm.cpp \
    src/sysex/retsetaudioportparm.cpp \
    src/sysex/getaudioglobalparm.cpp \
    src/sysex/retsetaudioglobalparm.cpp \
    src/widgets/audioportparmwidget.cpp \
    src/sysex/getaudiocontrolparm.cpp \
    src/sysex/getaudiocontroldetail.cpp \
    src/sysex/retsetaudiocontroldetail.cpp \
    src/sysex/getaudiocontroldetailvalue.cpp \
    src/sysex/retsetaudiocontroldetailvalue.cpp \
    src/sysex/retsetaudiocontrolparm.cpp \
    src/sysex/portsysexmessage.cpp \
    src/widgets/audiocontrolparmwidget.cpp \
    src/widgets/audiocontrolparmselectorwidget.cpp \
    src/widgets/audiocontrolparmclocksourcewidget.cpp \
    src/widgets/audiocontrolparmfeatureswidget.cpp \
    src/widgets/audiocontroldetailfeaturewidget.cpp \
    src/widgets/audiochannelfeaturewidget.cpp \
    src/widgets/controls/pkslider.cpp \
    src/sysex/getaudioportmetervalue.cpp \
    src/sysex/retaudioportmetervalue.cpp \
    src/sysex/getaudiodeviceparm.cpp \
    src/sysex/retsetaudiodeviceparm.cpp \
    src/sysex/getaudiopatchbayparm.cpp \
    src/sysex/retsetaudiopatchbayparm.cpp \
    src/widgets/audiomixerwidget.cpp \
    src/widgets/audiowidget.cpp \
    src/widgets/audioroutingwidget.cpp \
    src/sysex/getmixerparm.cpp \
    src/sysex/retsetmixerparm.cpp \
    src/sysex/retsetmixerportparm.cpp \
    src/sysex/getmixerportparm.cpp \
    src/widgets/mixerportwidget.cpp \
    src/sysex/retsetmixerinputparm.cpp \
    src/sysex/getmixerinputparm.cpp \
    src/widgets/views/hierarchicalheaderview.cpp

INCLUDEPATH += /usr/include/libusb-1.0/

HEADERS  += \
    src/RtMidi.h \
    src/device.h \
    src/sysex/getaudiochannelname.h \
    src/sysex/getmixerinputcontrol.h \
    src/sysex/getmixerinputcontrolvalue.h \
    src/sysex/getmixermetervalue.h \
    src/sysex/getmixeroutputcontrol.h \
    src/sysex/getmixeroutputcontrolvalue.h \
    src/sysex/getmixeroutputparm.h \
    src/sysex/midi.h \
    src/devicedetection.h \
    src/config/configuration.h \
    src/devicedetectionprocessor.h \
    src/sysex/retmixerinputcontrol.h \
    src/sysex/retmixermetervalue.h \
    src/sysex/retmixeroutputcontrol.h \
    src/sysex/retsetaudiochannelname.h \
    src/sysex/retsetmixerinputcontrolvalue.h \
    src/sysex/retsetmixeroutputcontrolvalue.h \
    src/sysex/retsetmixeroutputparm.h \
    src/sysex/sysexmessage.h \
    src/deviceselectiontablemodel.h \
    src/events/events.h \
    src/device.h \
    src/widgets/audiomixerchannelwidget.h \
    src/widgets/audiomixerinputchannelwidget.h \
    src/widgets/audiomixeroutputchannelwidget.h \
    src/widgets/controls/iconncalc.h \
    src/widgets/controls/pkmixerbutton.h \
    src/widgets/deviceinfowidget.h \
    src/widgets/portswidget.h \
    src/widgets/multiinfowidget.h \
    src/widgets/centralwidget.h \
    src/sysex/getinfolist.h \
    src/widgets/infotablewidget.h \
    src/sysex/retsetinfo.h \
    src/sysex/getinfo.h \
    src/sysex/retcommandlist.h \
    src/sysex/getdevice.h \
    src/sysex/retinfolist.h \
    src/sysex/getcommandlist.h \
    src/sysex/ack.h \
    src/sysex/getethernetportinfo.h \
    src/widgets/ethernetinfowidget.h \
    src/sysex/retsetethernetportinfo.h \
    src/sysex/getmidiinfo.h \
    src/sysex/retsetmidiportinfo.h \
    src/sysex/retsetmidiinfo.h \
    src/widgets/multiinfolistentry.h \
    src/widgets/controls/portbutton.h \
    src/widgets/portroutingwidget.h \
    src/sysex/getmidiportinfo.h \
    src/widgets/portdisplayhelper.h \
    src/sysex/getmidiportroute.h \
    src/sysex/retsetmidiportroute.h \
    src/sysex/saverestore.h \
    src/sysex/getsaverestorelist.h \
    src/sysex/retsaverestorelist.h \
    src/widgets/controls/ipaddressinput.h \
    src/sysex/communicationexception.h \
    src/sysex/protocolexception.h \
    src/config/qsettings-xml.h \
    src/definitions.h \
    src/aboutdialog.h \
    src/iconnconfigmain.h \
    src/sysex/getgizmocount.h \
    src/sysex/retgizmocount.h \
    src/sysex/getmidiportfilter.h \
    src/sysex/retsetmidiportfilter.h \
    src/widgets/portfilterwidget.h \
    src/widgets/controls/midicontrollerselect.h \
    src/widgets/portfiltersettingswidget.h \
    src/widgets/controls/midicontrollercombodelegate.h \
    src/widgets/portinfowidget.h \
    src/widgets/portremapsettingswidget.h \
    src/widgets/portremapwidget.h \
    src/sysex/retsetmidiportremap.h \
    src/sysex/getmidiportremap.h \
    src/widgets/controls/midichannelcombodelegate.h \
    src/widgets/globalaudioconfigurationwidget.h \
    src/widgets/audioportswidget.h \
    src/sysex/getaudioportparm.h \
    src/sysex/retsetaudioportparm.h \
    src/sysex/getaudioglobalparm.h \
    src/sysex/retsetaudioglobalparm.h \
    src/widgets/audioportparmwidget.h \
    src/sysex/getaudiocontrolparm.h \
    src/sysex/getaudiocontroldetail.h \
    src/sysex/retsetaudiocontroldetail.h \
    src/sysex/getaudiocontroldetailvalue.h \
    src/sysex/retsetaudiocontroldetailvalue.h \
    src/sysex/retsetaudiocontrolparm.h \
    src/sysex/portsysexmessage.h \
    src/widgets/audiocontrolparmwidget.h \
    src/widgets/audiocontrolparmselectorwidget.h \
    src/widgets/audiocontrolparmclocksourcewidget.h \
    src/widgets/audiocontrolparmfeatureswidget.h \
    src/widgets/audiocontroldetailfeaturewidget.h \
    src/widgets/audiochannelfeaturewidget.h \
    src/widgets/controls/pkslider.h \
    src/sysex/getaudioportmetervalue.h \
    src/sysex/retaudioportmetervalue.h \
    src/sysex/getaudiodeviceparm.h \
    src/sysex/retsetaudiodeviceparm.h \
    src/sysex/getaudiopatchbayparm.h \
    src/sysex/retsetaudiopatchbayparm.h \
    src/widgets/audiomixerwidget.h \
    src/widgets/audiowidget.h \
    src/widgets/audioroutingwidget.h \
    src/sysex/getmixerparm.h \
    src/sysex/retsetmixerparm.h \
    src/sysex/retsetmixerportparm.h \
    src/sysex/getmixerportparm.h \
    src/widgets/mixerportwidget.h \
    src/sysex/retsetmixerinputparm.h \
    src/sysex/getmixerinputparm.h \
    src/widgets/views/hierarchicalheaderview.h

FORMS    += \
    src/devicedetection.ui \
    src/widgets/multiinfowidget.ui \
    src/aboutdialog.ui \
    src/iconnconfigmain.ui \
    src/widgets/portfiltersettingswidget.ui \
    src/widgets/portinfowidget.ui \
    src/widgets/portremapsettingswidget.ui \
    src/widgets/globalaudioconfiguration.ui \
    src/widgets/audiocontrolparmwidget.ui \
    src/widgets/audiochannelfeaturewidget.ui \
    src/widgets/audioportparmwidget.ui \
    src/widgets/audiomixerwidget.ui \
    src/widgets/audioroutingwidget.ui

unix:!macx: LIBS += -lasound \
                    -lpthread \
                    -lusb-1.0

RESOURCES += \
    translations.qrc \
    resources.qrc

DISTFILES += \
    README.md \
    RELEASENOTES.md \
    BUILD.md \
    AUTHOR.md \
    resources/iconnconfig.desktop \
    resources/iconnconfig.png \
    resources/iconnconfig.xpm \
    tr/iconnconfig_de_DE.ts \
    tr/iconnconfig_de_DE.qm \
    tr/iconnconfig_fr_FR.ts \
    tr/iconnconfig_fr_FR.qm \
    tr/iconnconfig_en_EN.ts \
    tr/iconnconfig_en_EN.qm \
    .astylerc

# Some controls are in a separate project und needs to be included here

exists(../PKControls/pklevelmeter.pri){
    include(../PKControls/pklevelmeter.pri)
} else {
    error( The sources for pk controls are missing. \
    Please chechout from here 'https://codeberg.org/dehnhardt/PKControls.git' \
    in the same root directory as iConnConfig)
}

exists(../PKControls/pkdial.pri){
    include(../PKControls/pkdial.pri)
} else {
    error( The sources for pk controls are missing. \
    Please chechout from here 'https://codeberg.org/dehnhardt/PKControls.git' \
    in the same root directory as iConnConfig)
}
