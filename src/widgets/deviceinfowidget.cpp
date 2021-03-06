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
								   std::shared_ptr<GetInfo> deviceInfo,
								   QString windowTitle)
	: MultiInfoWidget(parent, device, windowTitle), m_pDeviceInfo(deviceInfo) {
	m_pInfoSections = new std::vector<MultiInfoListEntry *>();
	if (device->getCommands()->isCommandSupported(Command::GET_INFO_LIST))
		m_pInfoSections->push_back(
			new MultiInfoListEntry(MultiInfoListEntry::GLOBAL_DEVICE_INFO,
								   tr("Global").toStdString()));
	if (device->getCommands()->isCommandSupported(
			Command::GET_ETHERNET_PORT_INFO)) {
		int networkAdapters = device->getMidiInfo()->getEthernetJacks();
		for (int i = 0; i < networkAdapters; i++) {
			m_pInfoSections->push_back(
				new MultiInfoListEntry(MultiInfoListEntry::NETWORK_INFO,
									   tr("Network").toStdString(), i));
		}
	}
	if (device->hasAudioSupport()) {
		m_pInfoSections->push_back(new MultiInfoListEntry(
			MultiInfoListEntry::SECTION, tr("Audio").toStdString()));
		m_pInfoSections->push_back(new MultiInfoListEntry(
			MultiInfoListEntry::AUDIO_INFO,
			tr("Global Audio Configuration").toStdString()));
	}
}

DeviceInfoWidget::~DeviceInfoWidget() {
	/*if (infoSections != nullptr) {
		infoSections->clear();
		delete infoSections;
	}*/
}

void DeviceInfoWidget::deviceInfoChanged(SysExMessage::DeviceInfoItem item,
										 std::string value) {
	std::cout << "DeviceInfoWidget: deviceInfoChanged " << item << " value "
			  << value << std::endl;
	std::map<SysExMessage::DeviceInfoItem, std::shared_ptr<RetSetInfo>>
		*retSetInfos = this->m_pDeviceInfo->getRetSetInfos();
	if (retSetInfos) {
		std::shared_ptr<RetSetInfo> info = (*retSetInfos)[item];
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
			new GetEthernetPortInfo(this->m_pDevice);
		getEthernetPortInfo->setDebug(false);
		RetSetEthernetPortInfo *retSetEthernetPortInfo =
			static_cast<RetSetEthernetPortInfo *>(getEthernetPortInfo->query());
		retSetEthernetPortInfo->setDebug(false);
		EthernetInfoWidget *w =
			new EthernetInfoWidget(this, retSetEthernetPortInfo);
		a->setWidget(w);
		return a;
	};
	case MultiInfoListEntry::AUDIO_INFO: {
		GlobalAudioConfigurationWidget *globalAudioConfigurationWidget =
			new GlobalAudioConfigurationWidget(m_pDevice->getGlobalAudioParam(),
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
