#include "deviceinfowidget.h"
#include "../sysex/getethernetportinfo.h"
#include "../sysex/retcommandlist.h"
#include "../sysex/retsetethernetportinfo.h"
#include "../sysex/retsetmidiinfo.h"
#include "ethernetinfowidget.h"
#include "globalaudioconfigurationwidget.h"
#include "infotablewidget.h"
#include "multiinfolistentry.h"

#include <QGridLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QScrollArea>
#include <QTableWidget>

DeviceInfoWidget::DeviceInfoWidget(MioMain *parent, Device *device,
								   GetInfo *deviceInfo, QString windowTitle)
	: MultiInfoWidget(parent, device, windowTitle), m_pDeviceInfo(deviceInfo) {
	infoSections = new std::vector<MultiInfoListEntry *>();
	if (device->getCommands()->isCommandSupported(Command::GET_INFO_LIST))
		infoSections->push_back(
			new MultiInfoListEntry(MultiInfoListEntry::GLOBAL_DEVICE_INFO,
								   tr("Global").toStdString()));
	if (device->getCommands()->isCommandSupported(
			Command::GET_ETHERNET_PORT_INFO)) {
		int networkAdapters = device->getMidiInfo()->getEthernetJacks();
		for (int i = 0; i < networkAdapters; i++) {
			infoSections->push_back(
				new MultiInfoListEntry(MultiInfoListEntry::NETWORK_INFO,
									   tr("Network").toStdString(), i));
		}
	}
	if (device->hasAudioSupport()) {
		infoSections->push_back(new MultiInfoListEntry(
			MultiInfoListEntry::SECTION, tr("Audio").toStdString()));
		infoSections->push_back(new MultiInfoListEntry(
			MultiInfoListEntry::AUDIO_INFO,
			tr("Global Audio Konfiguration").toStdString()));
	}
}

DeviceInfoWidget::~DeviceInfoWidget() {}

void DeviceInfoWidget::deviceInfoChanged(SysExMessage::DeviceInfoItem item,
										 std::string value) {
	std::cout << "DeviceInfoWidget: deviceInfoChanged " << item << " value "
			  << value << std::endl;
	std::map<SysExMessage::DeviceInfoItem, RetSetInfo *> *retSetInfos =
		this->m_pDeviceInfo->getRetSetInfos();
	if (retSetInfos) {
		RetSetInfo *info = (*retSetInfos)[item];
		info->setValue(value);
	}
}

QWidget *DeviceInfoWidget::createWidget(MultiInfoListEntry *entry) {
	switch (entry->entryCode) {
	case MultiInfoListEntry::GLOBAL_DEVICE_INFO: {
		InfoTableWidget *w = new InfoTableWidget(
			this->parentWidget(), this->m_pDeviceInfo->getRetSetInfos());
		connect(
			w,
			SIGNAL(
				deviceInfoChanged(SysExMessage::DeviceInfoItem, std::string)),
			this,
			SLOT(deviceInfoChanged(SysExMessage::DeviceInfoItem, std::string)));
		return w;
	};
	case MultiInfoListEntry::NETWORK_INFO: {
		QScrollArea *a = new QScrollArea();
		GetEthernetPortInfo *getEthernetPortInfo =
			new GetEthernetPortInfo(this->device);
		getEthernetPortInfo->setDebug(true);
		RetSetEthernetPortInfo *retSetEthernetPortInfo =
			static_cast<RetSetEthernetPortInfo *>(getEthernetPortInfo->query());
		retSetEthernetPortInfo->setDebug(true);
		EthernetInfoWidget *w =
			new EthernetInfoWidget(this, retSetEthernetPortInfo);
		a->setWidget(w);
		return a;
	};
	case MultiInfoListEntry::AUDIO_INFO: {
		GlobalAudioConfigurationWidget *globalAudioConfigurationWidget =
			new GlobalAudioConfigurationWidget(device->getGlobalAudioParam(),
											   this);
		return globalAudioConfigurationWidget;
	}
	default: {
		QWidget *w = new QWidget(this->parentWidget());
		QGridLayout *lo = new QGridLayout();
		w->setLayout(lo);
		QLabel *l = new QLabel(w);
		l->setText(QString::fromStdString(entry->name));
		lo->addWidget(l, 0, 0);
		return w;
	};
	}
}
