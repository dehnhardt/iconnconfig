#include "audiocontrolparmfeatureswidget.h"
#include "audiocontroldetailfeaturewidget.h"

#include <QLabel>

AudioControlParmFeaturesWidget::AudioControlParmFeaturesWidget(
	Device *device, QVector<RetSetAudioControlParm *> *features,
	QWidget *parent)
	: QTabWidget(parent), m_pDevice(device), m_pFeatures(features) {
	createLayout();
	addFeatures();
}

void AudioControlParmFeaturesWidget::createLayout() {}

void AudioControlParmFeaturesWidget::addFeatures() {
	QVector<RetSetAudioControlParm *>::iterator i;
	for (i = m_pFeatures->begin(); i != m_pFeatures->end(); ++i) {
		createFeatureWidget(*i);
	}
}

void AudioControlParmFeaturesWidget::createFeatureWidget(
	RetSetAudioControlParm *retSetAudioControlParm) {
	AudioControlDetailFeatureWidget *w =
		new AudioControlDetailFeatureWidget(retSetAudioControlParm, m_pDevice);
	addTab(w, tr(retSetAudioControlParm->getControllerName().c_str()));
}
