#ifndef PORTDISPLAYHELPER_H
#define PORTDISPLAYHELPER_H

#include "../sysex/retsetaudioportparm.h"
#include "../sysex/retsetmidiportinfo.h"

#include <QIcon>
#include <QObject>

class PortDisplayHelper : public QObject {
	Q_OBJECT

  public:
	PortDisplayHelper() : QObject() {}

	static std::string getMidiPortTypeName(MidiPortType midiPortType) {
		std::string midiPortTypeName;
		switch (midiPortType) {
		case NONE:
			break;
		case DIN:
			midiPortTypeName = "DIN-Port";
			break;
		case USB_DEVICE:
			midiPortTypeName = "USB-Device-Port";
			break;
		case USB_HOST:
			midiPortTypeName = "USB-Host-Port";
			break;
		case ETHERNET:
			midiPortTypeName = "Ethernet-Port";
			break;
		}
		return midiPortTypeName;
	}
	static QIcon getPortIcon(MidiPortType midiPortType) {
		QIcon icon;
		switch (midiPortType) {
		case DIN:
			icon = QIcon(":/pixmaps/midijack");
			break;
		case ETHERNET:
			icon = QIcon(":/pixmaps/ethernetjack");
			break;
		case USB_DEVICE:
			icon = QIcon(":/pixmaps/usbjack");
			break;
		case USB_HOST:
			icon = QIcon(":/pixmaps/usbhostjack");
			break;
		default:
			break;
		}
		return icon;
	}

	static std::string getAudioPortTypeName(pk::AudioPortType audioPortType) {
		std::string midiPortTypeName;
		switch (audioPortType) {
		case pk::AudioPortType::APT_NONE:
			break;
		case pk::AudioPortType::APT_ANALOGUE:
			midiPortTypeName = "Analog-Port";
			break;
		case pk::AudioPortType::APT_USB_DEVICE:
			midiPortTypeName = "USB-Device-Port";
			break;
		case pk::AudioPortType::APT_USB_HOST:
			midiPortTypeName = "USB-Host-Port";
			break;
		case pk::AudioPortType::APT_ETHERNET:
			midiPortTypeName = "Ethernet-Port";
			break;
		}
		return midiPortTypeName;
	}
	static QIcon getAudioPortIcon(pk::AudioPortType audioPortType) {
		QIcon icon;
		switch (audioPortType) {
		case pk::AudioPortType::APT_ANALOGUE:
			icon = QIcon(":/pixmaps/xlrjack");
			break;
		case pk::AudioPortType::APT_ETHERNET:
			icon = QIcon(":/pixmaps/ethernetjack");
			break;
		case pk::AudioPortType::APT_USB_DEVICE:
			icon = QIcon(":/pixmaps/usbjack");
			break;
		case pk::AudioPortType::APT_USB_HOST:
			icon = QIcon(":/pixmaps/usbhostjack");
			break;
		default:
			break;
		}
		return icon;
	}
};

#endif // PORTDISPLAYHELPER_H
