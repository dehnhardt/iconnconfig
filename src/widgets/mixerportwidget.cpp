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
	// m_pVolumeTimer->setSingleShot(true);
	connect(m_pVolumeTimer, &QTimer::timeout, this,
			&MixerPortWidget::timerElapsed);
}

void MixerPortWidget::setName(QString name) { m_pPortNameLabel->setText(name); }

void MixerPortWidget::addMixerPanel(QWidget *mixerPanel,
									unsigned int mixerChannelNumber) {
	m_MapAttachedChannels[mixerChannelNumber] = mixerPanel;
	m_pMixerPanelLayout->addWidget(mixerPanel);
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
	/*++m_iCurrentMeterQuery;
	if (m_iCurrentMeterQuery > m_INumberOfOutputChannels)
		m_iCurrentMeterQuery = 1;*/
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
	m_pVolumeTimer->start(20);
	QWidget::showEvent(event);
}

void MixerPortWidget::hideEvent(QHideEvent *event) {
	std::cout << "stop" << std::endl;
	m_pVolumeTimer->stop();
	QWidget::hideEvent(event);
}
