#include "audioportswidget.h"
#include "../sysex/getaudiodeviceparm.h"
#include "../sysex/getaudioportparm.h"
#include "../sysex/retcommandlist.h"
#include "../sysex/retsetaudiodeviceparm.h"
#include "../sysex/retsetaudioglobalparm.h"
#include "audiocontrolparmwidget.h"
#include "audioportparmwidget.h"
#include "portdisplayhelper.h"

#include <QLabel>
#include <QListWidgetItem>
#include <QScrollArea>

AudioPortsWidget::AudioPortsWidget(MioMain *parent, Device *device,
								   QString windowTitle)
	: MultiInfoWidget(parent, device, windowTitle) {
	infoSections = new std::vector<MultiInfoListEntry *>();
	m_pAudioPortParms = new AudioPortStructure();
	if (device->getCommands()->isCommandSupported(
			Command::GET_AUDIO_PORT_PARM)) {
		retrieveAudioPorts();
		getAudioPortSections();
	}
}

AudioPortsWidget::~AudioPortsWidget() {

	AudioPortStructure::iterator it;
	for (it = m_pAudioPortParms->begin(); it != m_pAudioPortParms->end();
		 ++it) {
		std::vector<std::shared_ptr<RetSetAudioPortParm>> *audioPortParms =
			it->second;
		audioPortParms->clear();
	}
	m_pAudioPortParms->clear();
	delete m_pAudioPortParms;
}

void AudioPortsWidget::getAudioPorts(
	std::vector<std::shared_ptr<RetSetAudioPortParm>> *audioPortParms) {
	std::vector<std::shared_ptr<RetSetAudioPortParm>>::iterator it;
	for (it = audioPortParms->begin(); it != audioPortParms->end(); ++it) {
		std::shared_ptr<RetSetAudioPortParm> audioPortParm = *it;
		MultiInfoListEntry *entry = new MultiInfoListEntry(
			MultiInfoListEntry::PORT_ROUTING, audioPortParm->getPortName());
		entry->icon = PortDisplayHelper::getAudioPortIcon(
			audioPortParm->getAudioPortType());
		entry->message = audioPortParm;
		entry->enabled = true;
		infoSections->push_back(entry);
	}
}

void AudioPortsWidget::getAudioPortSections() {
	AudioPortStructure::iterator it;
	for (it = m_pAudioPortParms->begin(); it != m_pAudioPortParms->end();
		 ++it) {
		int section = it->first;
		AudioPortType audioPortType = static_cast<AudioPortType>(section);
		std::string portTypeName =
			PortDisplayHelper::getAudioPortTypeName(audioPortType);
		infoSections->push_back(
			new MultiInfoListEntry(MultiInfoListEntry::SECTION, portTypeName));
		std::vector<std::shared_ptr<RetSetAudioPortParm>> *audioPortParms =
			it->second;
		getAudioPorts(audioPortParms);
	}
}

void AudioPortsWidget::retrieveAudioPorts() {
	unsigned int numberOfAudioPorts =
		device->getGlobalAudioParam()->getNumberOfAudioPorts();
	GetAudioPortParm *getAudioPortParm = new GetAudioPortParm(device);
	getAudioPortParm->setDebug(false);
	for (unsigned int i = 1; i <= numberOfAudioPorts; i++) {
		std::vector<std::shared_ptr<RetSetAudioPortParm>> *audioPorts = nullptr;
		getAudioPortParm->setPortId(i);
		std::shared_ptr<RetSetAudioPortParm> retSetAudioPortParm =
			std::dynamic_pointer_cast<RetSetAudioPortParm>(
				getAudioPortParm->querySmart());
		int audioPortType = retSetAudioPortParm->getAudioPortType();
		try {
			audioPorts = m_pAudioPortParms->at(audioPortType);
		} catch (const std::out_of_range __attribute__((unused)) & oor) {
			audioPorts =
				new std::vector<std::shared_ptr<RetSetAudioPortParm>>();
			m_pAudioPortParms->insert(
				std::pair<int,
						  std::vector<std::shared_ptr<RetSetAudioPortParm>> *>(
					audioPortType, audioPorts));
		}
		audioPorts->push_back(retSetAudioPortParm);
	}
}

QWidget *AudioPortsWidget::createWidget(MultiInfoListEntry *entry) {
	std::shared_ptr<RetSetAudioPortParm> audioPortParm =
		std::dynamic_pointer_cast<RetSetAudioPortParm>(entry->message);
	unsigned int portId = audioPortParm->getPortId();
	QTabWidget *audioPortTabWidget = new QTabWidget(this);
	QWidget *layoutWidget = new QWidget();
	QVBoxLayout *layout = new QVBoxLayout();
	AudioPortParmWidget *audioPortParmWidget =
		new AudioPortParmWidget(audioPortParm);
	connect(audioPortParmWidget, &AudioPortParmWidget::changePortName, entry,
			&MultiInfoListEntry::changeName);

	layout->addWidget(audioPortParmWidget);
	if (device->getCommands()->isCommandSupported(
			Command::GET_AUDIO_DEVICE_PARM)) {
		std::unique_ptr<GetAudioDeviceParm> getAudioControlParm =
			std::make_unique<GetAudioDeviceParm>(device);
		getAudioControlParm->setPortId(portId);
		std::shared_ptr<RetSetAudioDeviceParm> retSetAudioDeviceParm =
			std::dynamic_pointer_cast<RetSetAudioDeviceParm>(
				getAudioControlParm->querySmart());
		audioPortParmWidget->setAudioDeviceParm(retSetAudioDeviceParm);
		if (retSetAudioDeviceParm->getMaxControllers() > 0) {
			AudioControlParmWidget *audioControlParmWidget =
				new AudioControlParmWidget(device, portId,
										   retSetAudioDeviceParm);
			QSizePolicy sp;
			sp.setHorizontalPolicy(QSizePolicy::Expanding);
			sp.setVerticalPolicy(QSizePolicy::Expanding);
			sp.setVerticalStretch(2);
			audioControlParmWidget->setSizePolicy(sp);
			layout->addWidget(audioControlParmWidget);
		} else {
			layout->addStretch();
		}
	}
	layoutWidget->setLayout(layout);
	audioPortTabWidget->addTab(layoutWidget, "Audio-Port Info");
	return audioPortTabWidget;
}
