#include "audiocontrolparmfeatureswidget.h"
#include "../sysex/retaudioportmetervalue.h"
#include "audiocontroldetailfeaturewidget.h"

#include <QLabel>
#include <QScrollArea>

AudioControlParmFeaturesWidget::AudioControlParmFeaturesWidget(
    Device *device, QVector<std::shared_ptr<RetSetAudioControlParm>> *features,
    QWidget *parent)
    : QWidget(parent), m_pDevice(device), m_pFeatures(features) {
	createLayout();
	this->m_pGetAudioPortMeterValue =
	    std::make_shared<GetAudioPortMeterValue>(m_pDevice);
	// Todo get port id
	m_pGetAudioPortMeterValue->setPortId(3);
	this->m_pVolumeTimer = new QTimer();
	// m_pVolumeTimer->setSingleShot(true);
	connect(m_pVolumeTimer, &QTimer::timeout, this,
	        &AudioControlParmFeaturesWidget::timerElapsed);

	addFeatures();
	// m_pVolumeTimer->start(10);
}

AudioControlParmFeaturesWidget::~AudioControlParmFeaturesWidget() {
	delete m_pVolumeTimer;
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
	QVector<std::shared_ptr<RetSetAudioControlParm>>::iterator i;
	for (i = m_pFeatures->begin(); i != m_pFeatures->end(); ++i) {
		createFeatureWidget(*i);
	}
}

void AudioControlParmFeaturesWidget::createFeatureWidget(
    std::shared_ptr<RetSetAudioControlParm> retSetAudioControlParm) {
	AudioControlDetailFeatureWidget *w = new AudioControlDetailFeatureWidget(
	    retSetAudioControlParm, m_pDevice, this);
	w->setSizePolicy(QSizePolicy::MinimumExpanding,
	                 QSizePolicy::MinimumExpanding);
	int cn = static_cast<int>(retSetAudioControlParm->getControllerNumber());
	m_pLayout->addWidget(
	    new QLabel(tr(retSetAudioControlParm->getControllerName().c_str())), 0,
	    cn);
	m_pLayout->addWidget(w, 1, cn);
}

void AudioControlParmFeaturesWidget::timerElapsed() {
	std::shared_ptr<RetAudioPortMeterValue> rapmv =
	    std::dynamic_pointer_cast<RetAudioPortMeterValue>(
	        m_pGetAudioPortMeterValue->querySmart());
	if (rapmv) {
		ChannelVolumes v = rapmv->getVolumes();
		for (auto channelVolume = v.in.begin(); channelVolume != v.in.end();
		     ++channelVolume)
			emit inMeterValueChanged(channelVolume->channel,
		                             channelVolume->volume);
		for (auto channelVolume = v.out.begin(); channelVolume != v.out.end();
		     ++channelVolume)
			emit outMeterValueChanged(channelVolume->channel,
		                              channelVolume->volume);
		rapmv.reset();
	}
}

void AudioControlParmFeaturesWidget::showEvent(QShowEvent *event) {
	std::cout << "start" << std::endl;
	m_pVolumeTimer->start(20);
	QWidget::showEvent(event);
}

void AudioControlParmFeaturesWidget::hideEvent(QHideEvent *event) {
	std::cout << "stop" << std::endl;
	m_pVolumeTimer->stop();
	QWidget::hideEvent(event);
}
