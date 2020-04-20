#include "audiomixerwidget.h"
#include "../iconnconfigmain.h"
#include "../sysex/getaudiopatchbayparm.h"
#include "../sysex/getmixerinputcontrol.h"
#include "../sysex/getmixeroutputcontrol.h"
#include "../sysex/getmixerparm.h"
#include "../sysex/getmixerportparm.h"
#include "../sysex/retmixerinputcontrol.h"
#include "../sysex/retmixeroutputcontrol.h"
#include "../sysex/retsetaudioglobalparm.h"
#include "../sysex/retsetaudiopatchbayparm.h"
#include "../sysex/retsetmixerparm.h"
#include "audiomixerinputchannelwidget.h"
#include "audiomixeroutputchannelwidget.h"
#include "centralwidget.h"
#include "mixerportwidget.h"
#include "portdisplayhelper.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <map>

AudioMixerWidget::AudioMixerWidget(Device *device, QWidget *parent)
	: QWidget(parent), m_pDevice(device) {
	m_iNumberOfAudioPorts =
		m_pDevice->getAudioGlobalParm()->getNumberOfAudioPorts();
	createLayout();
	createConnection();
}

AudioMixerWidget::~AudioMixerWidget() {}

void AudioMixerWidget::createLayout() {
	QVBoxLayout *widgetLayout = new QVBoxLayout();
	QVBoxLayout *mixerLayout = new QVBoxLayout();
	QHBoxLayout *mixerConfigLayout = new QHBoxLayout();
	this->m_pMixerPartsTab = new QTabWidget(this);

	QLabel *lblMixerConfig = new QLabel(tr("Mixer Config"));
	m_pCboMixerConfig = new QComboBox();
	m_pCboMixerConfig->setEnabled(false);
	m_pSAMixer = new QScrollArea();
	m_pSAMixer->setWidgetResizable(true);

	m_pWdgMixer = new QWidget();
	mixerLayout->addWidget(this->m_pMixerPartsTab);
	m_pWdgMixer->setLayout(mixerLayout);
	m_pSAMixer->setWidget(m_pWdgMixer);

	mixerConfigLayout->addWidget(lblMixerConfig);
	mixerConfigLayout->addWidget(m_pCboMixerConfig);
	mixerConfigLayout->addStretch();
	widgetLayout->addLayout(mixerConfigLayout);
	widgetLayout->addWidget(m_pSAMixer);
	setLayout(widgetLayout);
}

void AudioMixerWidget::createConnection() {
	CentralWidget *w = MioMain::getMainWin()->getCentralDeviceWidget();
	connect(w, &CentralWidget::changeAudioConfig, this,
			&AudioMixerWidget::audioConfigurationChanged);
	connect(m_pCboMixerConfig,
			QOverload<int>::of(&QComboBox::currentIndexChanged),
			[=](int index) {
				mixerConfigurationChanged(static_cast<unsigned int>(index) + 1);
			});
}

void AudioMixerWidget::createMixer() {
	std::map<unsigned int, AudioPortMixerBlock>::iterator it;
	std::map<unsigned int, AudioPortMixerBlock> mixerBlocs =
		m_pRetSetMixerPortParm->getAudioPortMixerBlocks();
	for (it = mixerBlocs.begin(); it != mixerBlocs.end(); ++it) {
	}
}

void AudioMixerWidget::audioConfigurationChanged(
	unsigned int audioConfigurationNumber) {
	std::unique_ptr<GetMixerParm> getMixerParm =
		std::make_unique<GetMixerParm>(m_pDevice);
	getMixerParm->setAudioConfigurationNumber(audioConfigurationNumber);
	m_pRetSetMixerParm =
		std::dynamic_pointer_cast<RetSetMixerParm>(getMixerParm->querySmart());
	m_pCboMixerConfig->clear();
	std::vector<AudioConfigurationMixerBlock> mb =
		m_pRetSetMixerParm->getACMixerBlocks();
	std::vector<AudioConfigurationMixerBlock>::iterator it;
	for (it = mb.begin(); it != mb.end(); ++it) {
		m_pCboMixerConfig->addItem(
			QString::number(it->mixerConfigurationNumber));
	}
	m_pCboMixerConfig->setCurrentText(QString::number(
		m_pRetSetMixerParm->getActiveMixerConfigurationBlock()));
}

void AudioMixerWidget::mixerConfigurationChanged(
	__attribute__((unused)) unsigned int mixerConfigurationNumber) {
	// this might work here... qDeleteAll(yourWidget->children());
	std::unique_ptr<GetMixerPortParm> getMixerPortParm =
		std::make_unique<GetMixerPortParm>(m_pDevice);
	m_pRetSetMixerPortParm = std::dynamic_pointer_cast<RetSetMixerPortParm>(
		getMixerPortParm->querySmart());
	std::map<unsigned int, AudioPortMixerBlock> apmb =
		m_pRetSetMixerPortParm->getAudioPortMixerBlocks();

	std::vector<std::shared_ptr<RetSetAudioChannelName>> outputChannels =
		m_pDevice->getOutputChannels();
	std::vector<std::shared_ptr<RetSetAudioChannelName>> intputChannels =
		m_pDevice->getInputChannels();

	std::map<unsigned int, AudioPortMixerBlock>::iterator it;
	for (it = apmb.begin(); it != apmb.end(); ++it) {
		unsigned int portId = it->first;
		AudioPortMixerBlock mb = it->second;
		if (mb.numberOfMixerOutputs > 0) {
			for (AudioChannelId audioOutputChannelId = 1;
				 audioOutputChannelId < mb.numberOfMixerOutputs;) {
				std::unique_ptr<GetMixerOutputControl> getOutputControl =
					std::make_unique<GetMixerOutputControl>(m_pDevice);
				getOutputControl->setPortId(portId);
				std::shared_ptr<RetMixerOutputControl> retMixerOutputControl =
					std::dynamic_pointer_cast<RetMixerOutputControl>(
						getOutputControl->querySmart());
				MixerPortWidget *mpw = new MixerPortWidget(portId, m_pDevice);
				mpw->setOutputOffset(audioOutputChannelId);
				QString portName;
				if (mb.numberOfMixerInputs > 0) {
					mpw->setNumberOfInputChannels(mb.numberOfMixerInputs);
					std::unique_ptr<GetMixerInputControl> getInputControl =
						std::make_unique<GetMixerInputControl>(m_pDevice);
					getInputControl->setPortId(portId);
					std::shared_ptr<RetMixerInputControl> retMixerInputControl =
						std::dynamic_pointer_cast<RetMixerInputControl>(
							getInputControl->querySmart());
					for (AudioChannelId audioInputChannelId = 1;
						 audioInputChannelId <= mb.numberOfMixerInputs;
						 audioInputChannelId++) {
						AudioMixerInputChannelWidget *amcw =
							new AudioMixerInputChannelWidget(
								m_pDevice, portId, audioInputChannelId,
								pk::ChannelDirection::CD_INPUT);
						amcw->setMixerInputControl(retMixerInputControl);
						amcw->setConnectedOutputChannel(audioOutputChannelId);
						connect(
							mpw, &MixerPortWidget::inMeterValueChanged, amcw,
							&AudioMixerInputChannelWidget::changeMeterVolume);
						mpw->addMixerPanel(amcw, pk::ChannelDirection::CD_INPUT,
										   audioInputChannelId);
					}
				}
				mpw->setNumberOfOutputChannels(2);
				for (unsigned int j = 0; j < 2; j++, audioOutputChannelId++) {
					AudioMixerOutputChannelWidget *amcw =
						new AudioMixerOutputChannelWidget(
							m_pDevice, portId, audioOutputChannelId,
							pk::ChannelDirection::CD_OUTPUT);
					amcw->setMixerOutputControl(retMixerOutputControl);
					connect(mpw, &MixerPortWidget::outMeterValueChanged, amcw,
							&AudioMixerOutputChannelWidget::changeMeterVolume);
					mpw->addMixerPanel(amcw, pk::ChannelDirection::CD_OUTPUT,
									   audioOutputChannelId);
					if (j == 0) {
						// Use the name of the first output from evey output
						// pair as the panel name
						AudioPortChannelId audioPortChannelId =
							channelIndex(portId, AudioPortClass::MIXER_PORT,
										 audioOutputChannelId);
						try {
							portName = m_pDevice->getAudioMixerOutputChannels()
										   ->at(audioPortChannelId)
										   ->getMixerName()
										   .c_str();
						} catch (...) {
							portName = tr("unknown");
						}
					}
				}
				mpw->addStretch();
				m_pMixerPartsTab->addTab(
					mpw,
					PortDisplayHelper::getAudioPortIcon(
						m_pDevice->getAudioPort(portId)->getAudioPortType()),
					tr("Port ") + QString::number(portId) + " / " + portName);
				connect(mpw, &MixerPortWidget::channelConnectionChanged, this,
						&AudioMixerWidget::changeChannelConnection);
			}
		}
	}
}

void AudioMixerWidget::changeChannelConnection() {
	emit channelConnectionChanged();
}
