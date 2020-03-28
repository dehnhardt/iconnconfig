#include "mixerportwidget.h"
#include "../sysex/retmixermetervalue.h"
#include <QVBoxLayout>

MixerPortWidget::MixerPortWidget(unsigned int portId, Device *device,
								 QWidget *parent)
	: QWidget(parent), m_pDevice(device), m_iPortId(portId) {
	createLayout();
	m_pGetMixerMeterValue = std::make_unique<GetMixerMeterValue>(m_pDevice);
	m_pGetMixerMeterValue->setPortId(portId);
	this->m_pVolumeTimer = new QTimer();
	connect(m_pVolumeTimer, &QTimer::timeout, this,
			&MixerPortWidget::timerElapsed);
}

void MixerPortWidget::setName(QString name) { m_pPortNameLabel->setText(name); }

void MixerPortWidget::addMixerPanel(AudioMixerChannelWidget *mixerPanel,
									ChannelDirection portDirection,
									unsigned int mixerChannelNumber) {
	if (portDirection == ChannelDirection::CD_INPUT) {
		m_MapAttachedInputChannels[mixerChannelNumber] = mixerPanel;
	} else if (portDirection == ChannelDirection::CD_OUTPUT) {
		m_MapAttachedOutputChannels[mixerChannelNumber] = mixerPanel;
	}
	m_pMixerPanelLayout->addWidget(mixerPanel);
	connect(mixerPanel, &AudioMixerChannelWidget::linkStatusChanged, this,
			&MixerPortWidget::linkStatusChanged);
}

void MixerPortWidget::setNumberOfInputChannels(
	unsigned int iNumberOfInputChannels) {
	m_iNumberOfInputChannels = iNumberOfInputChannels;
}

void MixerPortWidget::setNumberOfOutputChannels(
	unsigned int INumberOfOutputChannels) {
	m_INumberOfOutputChannels = INumberOfOutputChannels;
	m_iCurrentMeterQuery = 0;
}

void MixerPortWidget::createLayout() {
	QBoxLayout *vBoxLayout = new QVBoxLayout();
	QBoxLayout *hBoxLayout = new QHBoxLayout();
	m_pMixerPanelLayout = new QHBoxLayout();
	m_pPortNameLabel = new QLabel();
	m_pVolButton = new QPushButton("Volumes");
	hBoxLayout->addWidget(m_pPortNameLabel);
	hBoxLayout->addWidget(m_pVolButton);
	hBoxLayout->addStretch();
	vBoxLayout->addLayout(hBoxLayout);
	vBoxLayout->addLayout(m_pMixerPanelLayout);
	vBoxLayout->addStretch();
	setLayout(vBoxLayout);
	connect(m_pVolButton, &QPushButton::released, this,
			&MixerPortWidget::getVolumes);
}

void MixerPortWidget::getVolumes() {
	std::unique_ptr<GetMixerMeterValue> mmv =
		std::make_unique<GetMixerMeterValue>(m_pDevice);
	mmv->setPortId(m_iPortId);
	mmv->setPortDirections(PortDirection::BOTH);
	std::shared_ptr<RetMixerMeterValue> retMixerMeterValue =
		std::dynamic_pointer_cast<RetMixerMeterValue>(mmv->querySmart());
	mmv->setPortId(m_iPortId);
}

void MixerPortWidget::timerElapsed() {
	for (unsigned int i = 1; i <= m_INumberOfOutputChannels; i++) {
		m_pGetMixerMeterValue->setOutputNumber(i);
		std::shared_ptr<RetMixerMeterValue> rapmv =
			std::dynamic_pointer_cast<RetMixerMeterValue>(
				m_pGetMixerMeterValue->querySmart());
		if (rapmv) {
			ChannelVolumes v = rapmv->getVolumes();
			for (auto channelVolume = v.in.begin(); channelVolume != v.in.end();
				 ++channelVolume)
				emit inMeterValueChanged(channelVolume->channel,
										 channelVolume->volume);
			for (auto channelVolume = v.out.begin();
				 channelVolume != v.out.end(); ++channelVolume)
				emit outMeterValueChanged(channelVolume->channel,
										  channelVolume->volume);
			rapmv.reset();
		}
	}
}

void MixerPortWidget::showEvent(QShowEvent *event) {
	std::cout << "start" << std::endl;
	for (auto widget : m_MapAttachedInputChannels) {
		widget.second->refreshStatus();
	}
	m_pVolumeTimer->start(20);
	QWidget::showEvent(event);
}

void MixerPortWidget::hideEvent(QHideEvent *event) {
	std::cout << "stop" << std::endl;
	m_pVolumeTimer->stop();
	QWidget::hideEvent(event);
}

void MixerPortWidget::linkStatusChanged(AudioChannelId mixerChannelId,
										ChannelDirection channelDirection,
										bool status) {
	AudioMixerChannelWidget *master = nullptr;
	AudioMixerChannelWidget *slave = nullptr;
	if (mixerChannelId < 1)
		return;
	if (mixerChannelId % 2) {
		if (channelDirection == ChannelDirection::CD_INPUT) {
			master = this->m_MapAttachedInputChannels[mixerChannelId];
			slave = this->m_MapAttachedInputChannels[mixerChannelId + 1];
		}
		master->changeLinkStatus(status);
	} else {
		if (channelDirection == ChannelDirection::CD_INPUT) {
			master = this->m_MapAttachedInputChannels[mixerChannelId];
			slave = this->m_MapAttachedInputChannels[mixerChannelId - 1];
		}
		slave->changeLinkStatus(status);
	}
	master->setMaster(status, slave->getChannelName());
	slave->setVisible(!status);
	if (status) {
		connect(master, &AudioMixerChannelWidget::volumeChanged, slave,
				&AudioMixerChannelWidget::changeVolume);
		/*connect(master, &AudioMixerChannelWidget::panChanged, slave,
				&AudioMixerChannelWidget::changePan);*/
		connect(master, &AudioMixerChannelWidget::soloStatusChanged, slave,
				&AudioMixerChannelWidget::changeSoloStatus);
		connect(master, &AudioMixerChannelWidget::soloPFLStatusChanged, slave,
				&AudioMixerChannelWidget::changePFLStatus);
		connect(master, &AudioMixerChannelWidget::invertStatusChanged, slave,
				&AudioMixerChannelWidget::changeInvertStatus);
		connect(master, &AudioMixerChannelWidget::muteStatusChanged, slave,
				&AudioMixerChannelWidget::changeMuteStatus);
	} else {
		disconnect(master, &AudioMixerChannelWidget::volumeChanged, slave,
				   &AudioMixerChannelWidget::changeVolume);
		/*disconnect(master, &AudioMixerChannelWidget::panChanged, slave,
				   &AudioMixerChannelWidget::changePan);*/
		disconnect(master, &AudioMixerChannelWidget::soloStatusChanged, slave,
				   &AudioMixerChannelWidget::changeSoloStatus);
		disconnect(master, &AudioMixerChannelWidget::soloPFLStatusChanged,
				   slave, &AudioMixerChannelWidget::changePFLStatus);
		disconnect(master, &AudioMixerChannelWidget::invertStatusChanged, slave,
				   &AudioMixerChannelWidget::changeInvertStatus);
		disconnect(master, &AudioMixerChannelWidget::muteStatusChanged, slave,
				   &AudioMixerChannelWidget::changeMuteStatus);
	}
}
