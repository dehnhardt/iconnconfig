#include "audiomixerchannelwidget.h"
#include "../sysex/retsetaudiochannelname.h"
#include "portdisplayhelper.h"

#include <QMenu>
#include <QVBoxLayout>

AudioMixerChannelWidget::AudioMixerChannelWidget(Device *device,
												 QWidget *parent)
	: AudioChannelFeatureWidget(parent), m_pDevice(device) {
	QVBoxLayout *l = static_cast<QVBoxLayout *>(layout());
	if (l) {
		m_pSelectedInput = new QLabel("Port 1 >");
		m_pSelectedInput->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(m_pSelectedInput, &QLabel::customContextMenuRequested, this,
				&AudioMixerChannelWidget::openInputPortsMenu);
		l->insertWidget(1, m_pSelectedInput);
	}
	createInputMenu();
}

void AudioMixerChannelWidget::createInputMenu() {
	m_pInputMenu = new QMenu();
	std::shared_ptr<AudioPortStructure> aps =
		m_pDevice->getAudioPortStructure();
	AudioPortStructure::iterator itPortType;
	for (itPortType = aps->begin(); itPortType != aps->end(); ++itPortType) {
		int section = itPortType->first;
		AudioPortType audioPortType = static_cast<AudioPortType>(section);
		std::string portTypeName =
			PortDisplayHelper::getAudioPortTypeName(audioPortType);
		QMenu *audioPortTypeMenu =
			m_pInputMenu->addMenu(tr(portTypeName.c_str()));
		std::vector<std::shared_ptr<RetSetAudioPortParm>>::iterator itPort;
		std::vector<std::shared_ptr<RetSetAudioPortParm>> *ports =
			itPortType->second;
		for (itPort = ports->begin(); itPort != ports->end(); itPort++) {
			std::shared_ptr<RetSetAudioPortParm> app = *itPort;
			unsigned int portId = app->getPortId();
			QMenu *portMenu = audioPortTypeMenu->addMenu(
				QString::number(portId) + ": " + app->getPortName().c_str());
			AudioChannelNames::iterator channelIt;
			AudioChannelNames channelNames = m_pDevice->getAudioChannelNames(
				portId, ChannelDirection::CD_INPUT);
			for (channelIt = channelNames.begin();
				 channelIt != channelNames.end(); ++channelIt) {
				unsigned int channelId = channelIt->first;
				std::shared_ptr<RetSetAudioChannelName> channelName =
					channelIt->second;
				portMenu->addAction(QString::number(channelId) + ": " +
									channelName->getChannelName().c_str());
			}
		}
	}
}

void AudioMixerChannelWidget::openInputPortsMenu(const QPoint &pos) {
	m_pInputMenu->exec(m_pSelectedInput->mapToGlobal(pos));
}
