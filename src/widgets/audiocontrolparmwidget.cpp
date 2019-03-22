#include "audiocontrolparmwidget.h"
#include "audiocontrolparmfeatureswidget.h"
#include "ui_audiocontrolparmwidget.h"

#include <QLabel>
#include <QLayout>
#include <QScrollArea>

AudioControlParmWidget::AudioControlParmWidget(Device *device,
											   unsigned int portId,
											   QWidget *parent)
	: QWidget(parent), m_pDevice(device), m_iPortId(portId) {
	m_pAudioControlParms =
		new QMap<AudioControllerType, QVector<RetSetAudioControlParm *> *>();
	createLayout();
	loadAudioControlParms();
	addAudioControllerSections();
}

AudioControlParmWidget::~AudioControlParmWidget() {}

void AudioControlParmWidget::createLayout() {
	m_pLayout = new QVBoxLayout();
	setLayout(m_pLayout);
	m_pFeatureTabWidget = new QTabWidget();
	m_pLayout->addWidget(m_pFeatureTabWidget);
}

void AudioControlParmWidget::loadAudioControlParms() {
	this->m_pGetAudioControlParm = new GetAudioControlParm(m_pDevice);
	m_pGetAudioControlParm->setPortId(static_cast<unsigned int>(m_iPortId));
	for (unsigned int controllerNumber = 1; controllerNumber < 128;
		 controllerNumber++) {
		m_pGetAudioControlParm->setControllerNumber(controllerNumber);
		RetSetAudioControlParm *retSetAudioControlParm =
			dynamic_cast<RetSetAudioControlParm *>(
				m_pGetAudioControlParm->query());
		if (retSetAudioControlParm == nullptr)
			break;
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
			this->m_pFeatureTabWidget->addTab(featureWidget,
											  getFeatureName(type));
		} break;
		default:
			break;
		}
	}
}

QString AudioControlParmWidget::getFeatureName(
	AudioControllerType audioControllerType) {
	switch (audioControllerType) {
	case CT_FEATUERE:
		return tr("Features");
	case CT_SELECTOR:
		return tr("Selector");
	case CT_CLOCK_SOURCE:
		return tr("ClockSource");
	case CT_NONE:
		return tr("Unknown");
	}
	return "";
}
