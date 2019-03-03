#include "audioportswidget.h"
#include "../sysex/getaudioportparm.h"
#include "../sysex/retcommandlist.h"
#include "../sysex/retsetaudioglobalparm.h"
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
		getAudioPortParm->setDebug(true);
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
	std::cout << "create AudioPortsWidget" << std::endl;
	RetSetAudioPortParm *audioPortParm =
		static_cast<RetSetAudioPortParm *>(entry->message);
	// int portNumber = static_cast<int>(audioPortParm->getPortId());
	QTabWidget *audioPortTabWidget = new QTabWidget(this);
	AudioPortParmWidget *audioPortParmWidget =
		new AudioPortParmWidget(audioPortParm);
	// connect(device->getAudioGlobalParm(),&RetSetAudioGlobalParm::ch
	// audioPortParmWidget, &AudioPortParmWidget::audioConfigurationChanged )
	//	connect(portInfoWidget, &PortInfoWidget::changePortName, entry,
	// &MultiInfoListEntry::changeName);
	audioPortTabWidget->addTab(audioPortParmWidget, "Audio-Port Info");
	/*	if (device->getCommands()->isCommandSupported(
				Command::GET_MIDI_PORT_ROUTE)) {
					PortRoutingWidget *w = new PortRoutingWidget(
						device, portNumber, this->parentWidget());
					QScrollArea *a = new QScrollArea(portTabWidget);
					a->setWidget(w);
					portTabWidget->addTab(a, tr("MIDI-Port Routing"));
				}
				if (device->getCommands()->isCommandSupported(
						Command::GET_MIDI_PORT_FILTER)) {
					PortFilterWidget *portFilterWidget = new PortFilterWidget(
						device, portNumber, this->parentWidget());
					portTabWidget->addTab(portFilterWidget,
										  tr("MIDI-Port Filter"));
				}
				if (device->getCommands()->isCommandSupported(
						Command::GET_MIDI_PORT_REMAP)) {
					PortRemapWidget *portRemapWidget = new PortRemapWidget(
						device, portNumber, this->parentWidget());
					portTabWidget->addTab(portRemapWidget,
										  tr("MIDI-Port Remap"));
				}
				return portTabWidget;
				*/
	return audioPortTabWidget;
}
