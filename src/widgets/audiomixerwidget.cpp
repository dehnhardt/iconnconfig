#include "audiomixerwidget.h"
#include "../iconnconfigmain.h"
#include "../sysex/getmixerinputcontrol.h"
#include "../sysex/getmixerparm.h"
#include "../sysex/getmixerportparm.h"
#include "../sysex/retmixerinputcontrol.h"
#include "../sysex/retsetaudioglobalparm.h"
#include "../sysex/retsetmixerparm.h"
#include "audiomixerchannelwidget.h"
#include "centralwidget.h"
#include "mixerportwidget.h"

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
	m_pLAInputMixerParts = new QHBoxLayout();
	m_pLAOutputMixerParts = new QHBoxLayout();

	QLabel *lblMixerConfig = new QLabel(tr("Mixer Config"));
	m_pCboMixerConfig = new QComboBox();
	m_pSAMixer = new QScrollArea();
	m_pSAMixer->setWidgetResizable(true);

	m_pWdgMixer = new QWidget();
	mixerLayout->addLayout(m_pLAInputMixerParts);
	mixerLayout->addLayout(m_pLAOutputMixerParts);
	m_pWdgMixer->setLayout(mixerLayout);
	m_pSAMixer->setWidget(m_pWdgMixer);

	mixerConfigLayout->addWidget(lblMixerConfig);
	mixerConfigLayout->addWidget(m_pCboMixerConfig);
	mixerConfigLayout->addStretch();
	widgetLayout->addLayout(mixerConfigLayout);
	widgetLayout->addWidget(m_pSAMixer);
	// widgetLayout->addWidget(m_pWdgMixer);
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
	std::map<unsigned int, AudioPortMixerBlock>::iterator it;
	for (it = apmb.begin(); it != apmb.end(); ++it) {
		unsigned int portId = it->first;
		AudioPortMixerBlock mb = it->second;
		if (mb.numberOfMixerInputs > 0) {
			MixerPortWidget *mpw = new MixerPortWidget(portId);
			mpw->setName(tr("Input Port ") + QString::number(portId));
			std::unique_ptr<GetMixerInputControl> getInputControl =
				std::make_unique<GetMixerInputControl>(m_pDevice);
			getInputControl->setPortId(portId);
			std::shared_ptr<RetMixerInputControl> retMixerInputControl =
				std::dynamic_pointer_cast<RetMixerInputControl>(
					getInputControl->querySmart());
			m_pLAInputMixerParts->addWidget(mpw);
			for (unsigned int i = 1; i <= mb.numberOfMixerInputs; i++) {
				AudioMixerChannelWidget *amcw = new AudioMixerChannelWidget(
					m_pDevice, portId, i, ChannelDirection::CD_INPUT);
				amcw->setMixerInputControl(retMixerInputControl);
				mpw->addMixerPanel(amcw, i);
			}
		}
		if (mb.numberOfMixerOutputs > 0) {
			MixerPortWidget *mpw = new MixerPortWidget(portId);
			mpw->setName(tr("Output Port ") + QString::number(portId));
			m_pLAOutputMixerParts->addWidget(mpw);
			for (unsigned int i = 1; i <= mb.numberOfMixerOutputs; i++) {
				AudioMixerChannelWidget *amcw = new AudioMixerChannelWidget(
					m_pDevice, portId, i, ChannelDirection::CD_OUTPUT);
				mpw->addMixerPanel(amcw, i);
			}
		}
	}
}
