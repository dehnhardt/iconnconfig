#include "audiocontrolparmwidget.h"
#include "audiocontrolparmfeatureswidget.h"
#include "ui_audiocontrolparmwidget.h"

#include <QLabel>
#include <QLayout>

AudioControlParmWidget::AudioControlParmWidget(Device *device, int portId,
											   QWidget *parent)
	: QWidget(parent), ui(new Ui::AudioControlParmWidget), m_pDevice(device),
	  m_iPortId(portId) {
	ui->setupUi(this);
	m_pAudioControlParms =
		new QMap<AudioControllerType, QVector<RetSetAudioControlParm *> *>();
	loadAudioControlParms();
	addAudioControllerSections();
}

AudioControlParmWidget::~AudioControlParmWidget() { delete ui; }

void AudioControlParmWidget::loadAudioControlParms() {
	this->m_pGetAudioControlParm = new GetAudioControlParm(m_pDevice);
	m_pGetAudioControlParm->setDebug(false);
	ui->m_pLVAudioControlParms->setHidden(!m_pGetAudioControlParm->getDebug());
	m_pGetAudioControlParm->setPortId(static_cast<unsigned int>(m_iPortId));
	for (unsigned int controllerNumber = 1; controllerNumber < 128;
		 controllerNumber++) {
		m_pGetAudioControlParm->setControllerNumber(controllerNumber);
		RetSetAudioControlParm *retSetAudioControlParm =
			dynamic_cast<RetSetAudioControlParm *>(
				m_pGetAudioControlParm->query());
		if (retSetAudioControlParm == nullptr)
			break;
		if (m_pGetAudioControlParm->getDebug())
			ui->m_pLVAudioControlParms->addItem(
				retSetAudioControlParm->getControllerName().c_str());
		addAudioControlParm(retSetAudioControlParm);
	}
}

void AudioControlParmWidget::addAudioControlParm(
	RetSetAudioControlParm *retSetAudioControlParm) {
	AudioControllerType controllerType =
		retSetAudioControlParm->getControllerType();
	QVector<RetSetAudioControlParm *> *v =
		m_pAudioControlParms->value(controllerType);
	if (!v) {
		v = new QVector<RetSetAudioControlParm *>();
		m_pAudioControlParms->insert(controllerType, v);
	}
	v->push_back(retSetAudioControlParm);
}

void AudioControlParmWidget::addAudioControllerSections() {
	QMap<AudioControllerType, QVector<RetSetAudioControlParm *> *>::iterator i;
	for (i = m_pAudioControlParms->begin(); i != m_pAudioControlParms->end();
		 ++i) {
		AudioControllerType type = i.key();
		switch (type) {
		case CT_FEATUERE: {
			AudioControlParmFeaturesWidget *featureWidget =
				new AudioControlParmFeaturesWidget(m_pDevice, i.value());
			this->layout()->addWidget(featureWidget);
		} break;
		default:
			break;
		}
	}
}
