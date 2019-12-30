#include "audiowidget.h"
#include "../sysex/retsetaudioglobalparm.h"
#include "audiomixerwidget.h"
#include "audioroutingwidget.h"

AudioWidget::AudioWidget(MioMain *parent, Device *device, QString windowTitle)
	: MultiInfoWidget(parent, device, windowTitle) {
	m_pInfoSections = new std::vector<MultiInfoListEntry *>();
	MultiInfoListEntry *entry = new MultiInfoListEntry(
		MultiInfoListEntry::AUDIO_ROUTING, tr("Audio Routing").toStdString());
	m_pInfoSections->push_back(entry);
	entry = new MultiInfoListEntry(MultiInfoListEntry::AUDIO_MIXER,
								   tr("Audio Mixer").toStdString());
	entry->enabled = true;
	m_pInfoSections->push_back(entry);
}

AudioWidget::~AudioWidget() {}

QWidget *AudioWidget::createWidget(MultiInfoListEntry *entry) {
	if (entry->entryCode == MultiInfoListEntry::AUDIO_MIXER) {
		AudioMixerWidget *audioMixerWidget = new AudioMixerWidget(m_pDevice);
		audioMixerWidget->audioConfigurationChanged(
			m_pDevice->getAudioGlobalParm()
				->getNumberOfActiveAudioConfiguration());
		return audioMixerWidget;
	}
	if (entry->entryCode == MultiInfoListEntry::AUDIO_ROUTING)
		return new AudioRoutingWidget(m_pDevice);

	return nullptr;
}
