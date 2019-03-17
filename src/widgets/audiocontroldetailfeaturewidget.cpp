#include "audiocontroldetailfeaturewidget.h"
#include "../sysex/getaudiocontroldetail.h"
#include "../sysex/retsetaudiocontroldetail.h"
#include "audiochannelfeaturewidget.h"

#include <QScrollArea>

AudioControlDetailFeatureWidget::AudioControlDetailFeatureWidget(
	RetSetAudioControlParm *retSetAudioControlParm, Device *device,
	QWidget *parent)
	: QWidget(parent), m_pRetSetAudioControlParm(retSetAudioControlParm),
	  m_pDevice(device) {
	m_iPortId = m_pRetSetAudioControlParm->getPortId();
	m_iControllerNumber = m_pRetSetAudioControlParm->getControllerNumber();
	m_pLayout = new QVBoxLayout();
	setLayout(m_pLayout);
	setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding));
	getDetails();
}

void AudioControlDetailFeatureWidget::getDetails() {
	GetAudioControlDetail *getAudioControlDetail =
		new GetAudioControlDetail(m_pDevice);
	getAudioControlDetail->setPortId(m_iPortId);
	getAudioControlDetail->setControllerNumber(m_iControllerNumber);
	getAudioControlDetail->setDebug(true);
	QVBoxLayout *layout = new QVBoxLayout();
	QScrollArea *scrollArea = new QScrollArea();
	m_pLayout->addWidget(scrollArea);
	QWidget *w = new QWidget();
	w->setLayout(layout);
	// scrollArea->setLayout(m_pLayout);
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
			layout->addWidget(audiochannelFeatureWidget);
		}
	}
	scrollArea->setWidget(w);
	// layout->addStretch(2);
}
