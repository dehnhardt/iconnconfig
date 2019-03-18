#include "audiocontrolparmfeatureswidget.h"
#include "audiocontroldetailfeaturewidget.h"

#include <QLabel>
#include <QScrollArea>

AudioControlParmFeaturesWidget::AudioControlParmFeaturesWidget(
	Device *device, QVector<RetSetAudioControlParm *> *features,
	QWidget *parent)
	: QWidget(parent), m_pDevice(device), m_pFeatures(features) {
	createLayout();
	addFeatures();
}

void AudioControlParmFeaturesWidget::createLayout() {
	QVBoxLayout *baseLayout = new QVBoxLayout();
	QWidget *w = new QWidget();
	m_pLayout = new QGridLayout();

	setLayout(baseLayout);

	QScrollArea *scrollarea = new QScrollArea();
	scrollarea->setWidgetResizable(true);
	scrollarea->setWidget(w);
	w->setLayout(m_pLayout);
	baseLayout->addWidget(scrollarea);
}

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
	w->setSizePolicy(QSizePolicy::MinimumExpanding,
					 QSizePolicy::MinimumExpanding);
	int cn = static_cast<int>(retSetAudioControlParm->getControllerNumber());
	m_pLayout->addWidget(
		new QLabel(tr(retSetAudioControlParm->getControllerName().c_str())), 0,
		cn);
	m_pLayout->addWidget(w, 1, cn);
}
