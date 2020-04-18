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

MixerPortWidget::~MixerPortWidget() { delete m_pVolumeTimer; }

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
	connect(mixerPanel, &AudioMixerChannelWidget::channelConnectionChanged,
			this, &MixerPortWidget::channelConnectionChanged);
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

void MixerPortWidget::setOutputOffset(unsigned int iOutputOffset) {
	m_iOutputOffset = iOutputOffset;
}

void MixerPortWidget::createLayout() {
	QBoxLayout *vBoxLayout = new QVBoxLayout();
	QBoxLayout *hBoxLayout = new QHBoxLayout();
	m_pMixerPanelLayout = new QHBoxLayout();
	m_pPortNameLabel = new QLabel("", this);
	hBoxLayout->addWidget(m_pPortNameLabel);
	hBoxLayout->addStretch();
	vBoxLayout->addLayout(hBoxLayout);
	vBoxLayout->addLayout(m_pMixerPanelLayout);
	vBoxLayout->addStretch();
	setLayout(vBoxLayout);
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
	for (unsigned int i = 0; i < m_INumberOfOutputChannels; i++) {
		AudioChannelId outputChannelId = m_iOutputOffset + i;
		m_pGetMixerMeterValue->setPortDirections(PortDirection::BOTH);
		m_pGetMixerMeterValue->setOutputNumber(outputChannelId);
		std::shared_ptr<RetMixerMeterValue> rapmv =
			std::dynamic_pointer_cast<RetMixerMeterValue>(
				m_pGetMixerMeterValue->querySmart());
		if (rapmv) {
			ChannelVolumes v = rapmv->getVolumes();
			for (auto channelVolume = v.in.begin(); channelVolume != v.in.end();
				 ++channelVolume)
				if ((channelVolume->channel % 2) == (outputChannelId % 2))
					emit inMeterValueChanged(
						static_cast<unsigned int>(channelVolume->channel),
						channelVolume->volume);
			for (auto channelVolume = v.out.begin();
				 channelVolume != v.out.end(); ++channelVolume) {
				if ((m_iPortId == 1) && channelVolume->volume)
					std::cout << "Output volume for port " << m_iPortId
							  << " channel " << (m_iOutputOffset + i) << " = "
							  << channelVolume->volume << std::endl;
				emit outMeterValueChanged(outputChannelId,
										  channelVolume->volume);
			}
			rapmv.reset();
		}
	}
}

void MixerPortWidget::showEvent(QShowEvent *event) {
	std::cout << "start" << std::endl;
	for (std::pair<int, AudioMixerChannelWidget *> widget :
		 m_MapAttachedInputChannels) {
		widget.second->refreshStatus();
	}
	for (std::pair<int, AudioMixerChannelWidget *> widget :
		 m_MapAttachedOutputChannels) {
		widget.second->refreshStatus();
	}
	m_pVolumeTimer->start(100);
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
		if (channelDirection == ChannelDirection::CD_OUTPUT) {
			master = this->m_MapAttachedOutputChannels[mixerChannelId];
			slave = this->m_MapAttachedOutputChannels[mixerChannelId + 1];
		}
		master->changeLinkStatus(status);
	} else {
		if (channelDirection == ChannelDirection::CD_INPUT) {
			master = this->m_MapAttachedInputChannels[mixerChannelId - 1];
			slave = this->m_MapAttachedInputChannels[mixerChannelId];
		}
		if (channelDirection == ChannelDirection::CD_OUTPUT) {
			master = this->m_MapAttachedOutputChannels[mixerChannelId - 1];
			slave = this->m_MapAttachedOutputChannels[mixerChannelId];
		}
		slave->changeLinkStatus(status);
	}
	master->setMaster(status, slave->getChannelName());
	slave->setSlave(status);
	slave->setVisible(!status);
	if (channelDirection == ChannelDirection::CD_INPUT) {
		if (status) {
			/*connect(master, &AudioMixerChannelWidget::volumeChanged, slave,
					&AudioMixerChannelWidget::changeVolume);
			connect(master, &AudioMixerChannelWidget::panChanged, slave,
					&AudioMixerChannelWidget::changePan);
			connect(master, &AudioMixerChannelWidget::soloStatusChanged, slave,
					&AudioMixerChannelWidget::changeSoloStatus);
			connect(master, &AudioMixerChannelWidget::soloPFLStatusChanged,
					slave, &AudioMixerChannelWidget::changePFLStatus);*/
			connect(master, &AudioMixerChannelWidget::invertStatusChanged,
					slave, &AudioMixerChannelWidget::changeInvertStatus);
			/*connect(master, &AudioMixerChannelWidget::muteStatusChanged,
			   slave, &AudioMixerChannelWidget::changeMuteStatus);*/
		} else {
			/*disconnect(master, &AudioMixerChannelWidget::volumeChanged, slave,
					   &AudioMixerChannelWidget::changeVolume);
			disconnect(master, &AudioMixerChannelWidget::panChanged, slave,
					   &AudioMixerChannelWidget::changePan);
			disconnect(master, &AudioMixerChannelWidget::soloStatusChanged,
					   slave, &AudioMixerChannelWidget::changeSoloStatus);
			disconnect(master, &AudioMixerChannelWidget::soloPFLStatusChanged,
					   slave, &AudioMixerChannelWidget::changePFLStatus);*/
			disconnect(master, &AudioMixerChannelWidget::invertStatusChanged,
					   slave, &AudioMixerChannelWidget::changeInvertStatus);
			/*disconnect(master, &AudioMixerChannelWidget::muteStatusChanged,
					   slave, &AudioMixerChannelWidget::changeMuteStatus);*/
		}
	}
	if (status) {
		QVBoxLayout *l = static_cast<QVBoxLayout *>(
			master->getSecondConnectionFrame()->layout());
		if (l) {
			l->addWidget(slave->getConnectionButton());
			master->getSecondConnectionFrame()->setEnabled(true);
		}
		int balance = master->calculateBalance(master->getPanValue(),
											   slave->getPanValue());
		master->setPanValue(balance);
	} else {
		QLayout *l = slave->getFirstConnectionFrame()->layout();
		if (l) {
			l->addWidget(slave->getConnectionButton());
			master->getSecondConnectionFrame()->setEnabled(false);
		}
		int balance = master->getPanValue();
		master->setPanValue(master->calculatePanL(balance));
		slave->setPanValue(master->calculatePanR(balance));
	}
}

void MixerPortWidget::changeChannelConnection() {
	emit channelConnectionChanged();
}
