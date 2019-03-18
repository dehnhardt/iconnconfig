#include "audiocontroldetailfeaturewidget.h"
#include "../sysex/getaudiocontroldetail.h"
#include "../sysex/retsetaudiocontroldetail.h"
#include "audiochannelfeaturewidget.h"

AudioControlDetailFeatureWidget::AudioControlDetailFeatureWidget(
	RetSetAudioControlParm *retSetAudioControlParm, Device *device,
	QWidget *parent)
	: QWidget(parent), m_pRetSetAudioControlParm(retSetAudioControlParm),
	  m_pDevice(device) {
	m_iPortId = m_pRetSetAudioControlParm->getPortId();
	m_iControllerNumber = m_pRetSetAudioControlParm->getControllerNumber();
	m_pLayout = new QHBoxLayout();
	setLayout(m_pLayout);
	getDetails();
}

void AudioControlDetailFeatureWidget::getDetails() {
	GetAudioControlDetail *getAudioControlDetail =
		new GetAudioControlDetail(m_pDevice);
	getAudioControlDetail->setPortId(m_iPortId);
	getAudioControlDetail->setControllerNumber(m_iControllerNumber);
	getAudioControlDetail->setDebug(true);
	for (unsigned int i = 1; i < 128; i++) {
		getAudioControlDetail->setDetailNumber(i);
		RetSetAudioControlDetail *retSetAudioControlDetail =
			dynamic_cast<RetSetAudioControlDetail *>(
				getAudioControlDetail->query());
		if (!retSetAudioControlDetail)
			break;
		else {
			AudioChannelFeatureWidget *audiochannelFeatureWidget =
				new AudioChannelFeatureWidget(retSetAudioControlDetail);
			m_pLayout->addWidget(audiochannelFeatureWidget);
		}
	}
}
