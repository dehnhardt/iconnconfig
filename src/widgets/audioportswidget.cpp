#include "audioportswidget.h"
#include "../sysex/getaudioportparm.h"
#include "../sysex/retcommandlist.h"
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

AudioPortsWidget::~AudioPortsWidget() {}

void AudioPortsWidget::getAudioPorts(
	std::vector<RetSetAudioPortParm *> *audioPortParms) {
	std::vector<RetSetAudioPortParm *>::iterator it;
	for (it = audioPortParms->begin(); it != audioPortParms->end(); ++it) {
		RetSetAudioPortParm *audioPortParm = *it;
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
		std::vector<RetSetAudioPortParm *> *audioPortParms = it->second;
		getAudioPorts(audioPortParms);
	}
}

void AudioPortsWidget::retrieveAudioPorts() {
	unsigned int numberOfAudioPorts =
		device->getGlobalAudioParam()->getNumberOfAudioPorts();
	for (unsigned int i = 1; i <= numberOfAudioPorts; i++) {
		std::vector<RetSetAudioPortParm *> *audioPorts = nullptr;
		GetAudioPortParm *getAudioPortParm = new GetAudioPortParm(device);
		getAudioPortParm->setDebug(false);
		getAudioPortParm->setPortId(i);
		RetSetAudioPortParm *retSetAudioPortParm =
			dynamic_cast<RetSetAudioPortParm *>(getAudioPortParm->query());
		int audioPortType = retSetAudioPortParm->getAudioPortType();
		try {
			audioPorts = m_pAudioPortParms->at(audioPortType);
		} catch (const std::out_of_range __attribute__((unused)) & oor) {
			audioPorts = new std::vector<RetSetAudioPortParm *>();
			m_pAudioPortParms->insert(
				std::pair<int, std::vector<RetSetAudioPortParm *> *>(
					audioPortType, audioPorts));
		}
		audioPorts->push_back(retSetAudioPortParm);
	}
}

QWidget *AudioPortsWidget::createWidget(MultiInfoListEntry *entry) {
	RetSetAudioPortParm *audioPortParm =
		static_cast<RetSetAudioPortParm *>(entry->message);
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
			Command::GET_AUDIO_CONTROL_PARM)) {
		// check if there is at least one parameter
		GetAudioControlParm *m_pGetAudioControlParm =
			new GetAudioControlParm(device);
		m_pGetAudioControlParm->setDebug(false);
		m_pGetAudioControlParm->setPortId(portId);
		m_pGetAudioControlParm->setControllerNumber(1);
		SysExMessage *m = m_pGetAudioControlParm->query();
		if (m->getCommand() == RET_SET_AUDIO_CONTROL_PARM) {
			AudioControlParmWidget *audioControlParmWidget =
				new AudioControlParmWidget(device, portId);
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
