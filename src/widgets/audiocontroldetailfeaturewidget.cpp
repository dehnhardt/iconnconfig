#include "audiocontroldetailfeaturewidget.h"
#include "../sysex/getaudiocontroldetail.h"
#include "../sysex/retsetaudiocontroldetail.h"
#include "audiochannelfeaturewidget.h"
#include "audiocontrolparmfeatureswidget.h"

int AudioControlDetailFeatureWidget::INPUT = 1;
int AudioControlDetailFeatureWidget::OUTPUT = 1;

AudioControlDetailFeatureWidget::AudioControlDetailFeatureWidget(
	std::shared_ptr<RetSetAudioControlParm> retSetAudioControlParm,
	Device *device, QWidget *parent)
	: QWidget(parent), m_pRetSetAudioControlParm(retSetAudioControlParm),
	  m_pDevice(device) {
	m_pAudioControlParmFeaturesWidget =
		dynamic_cast<AudioControlParmFeaturesWidget *>(parent);
	m_iPortId = m_pRetSetAudioControlParm->getPortId();
	m_iControllerNumber = m_pRetSetAudioControlParm->getControllerNumber();
	createLayout();
	getDetails();
}

void AudioControlDetailFeatureWidget::getDetails() {
	std::unique_ptr<GetAudioControlDetail> getAudioControlDetail =
		std::make_unique<GetAudioControlDetail>(m_pDevice);
	getAudioControlDetail->setPortId(m_iPortId);
	getAudioControlDetail->setControllerNumber(m_iControllerNumber);
	getAudioControlDetail->setDebug(false);
	bool lastLinkStatus = false;
	unsigned int numberOfFeatureChannels =
		m_pRetSetAudioControlParm->getNumberOfFeatuireChannels();
	for (unsigned int i = 1; i <= numberOfFeatureChannels; i++) {
		getAudioControlDetail->setDetailNumber(i);
		std::shared_ptr<RetSetAudioControlDetail> retSetAudioControlDetail =
			std::dynamic_pointer_cast<RetSetAudioControlDetail>(
				getAudioControlDetail->querySmart());
		if (!retSetAudioControlDetail) {
			break;
		} else {
			AudioChannelFeatureWidget *audiochannelFeatureWidget =
				new AudioChannelFeatureWidget(retSetAudioControlDetail);
			m_vFeatureWidgets.push_back(audiochannelFeatureWidget);
			m_pLayout->addWidget(audiochannelFeatureWidget);
			if (retSetAudioControlDetail->hasStereoLinkControl() &&
				retSetAudioControlDetail->getStereoLinkControlEditable()) {
				connect(audiochannelFeatureWidget,
						&AudioChannelFeatureWidget::linkStatusChanged, this,
						&AudioControlDetailFeatureWidget::linkStatusChanged);
				if (i % 2 == 0) {
					if (lastLinkStatus ||
						audiochannelFeatureWidget->getLinkStatus())
						linkStatusChanged(i, true);
				}
				lastLinkStatus = audiochannelFeatureWidget->getLinkStatus();
				switch (retSetAudioControlDetail->getChannelDirection()) {
				case pk::ChannelDirection::CD_INPUT:
					audiochannelFeatureWidget->setChannelId(INPUT);
					INPUT++;
					connect(
						m_pAudioControlParmFeaturesWidget,
						&AudioControlParmFeaturesWidget::inMeterValueChanged,
						audiochannelFeatureWidget,
						&AudioChannelFeatureWidget::changeMeterVolume);
					break;
				case pk::ChannelDirection::CD_OUTPUT:
					audiochannelFeatureWidget->setChannelId(OUTPUT);
					OUTPUT++;
					connect(
						m_pAudioControlParmFeaturesWidget,
						&AudioControlParmFeaturesWidget::outMeterValueChanged,
						audiochannelFeatureWidget,
						&AudioChannelFeatureWidget::changeMeterVolume);
					break;
				default:
					break;
				}
			}
		}
	}
}

void AudioControlDetailFeatureWidget::createLayout() {

	QSizePolicy p;
	p.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
	setSizePolicy(p);
	m_pLayout = new QHBoxLayout();
	m_pLayout->setAlignment(Qt::AlignmentFlag::AlignLeft);
	setLayout(m_pLayout);
}

void AudioControlDetailFeatureWidget::linkStatusChanged(unsigned int detailId,
														bool status) {
	AudioChannelFeatureWidget *master = nullptr;
	AudioChannelFeatureWidget *slave = nullptr;
	if (detailId < 1)
		return;
	unsigned long index = static_cast<unsigned long>(detailId - 1);
	if (index % 2) {
		master = this->m_vFeatureWidgets[index - 1];
		slave = this->m_vFeatureWidgets[index];
		master->changeLinkStatus(detailId - 1, status);
	} else {
		master = this->m_vFeatureWidgets[index];
		slave = this->m_vFeatureWidgets[index + 1];
		slave->changeLinkStatus(detailId + 1, status);
	}
	master->setMaster(status, slave->getChannelName());
	slave->setVisible(!status);
	if (status) {
		connect(master, &AudioChannelFeatureWidget::volumeChanged, slave,
				&AudioChannelFeatureWidget::changeVolume);
		connect(master, &AudioChannelFeatureWidget::trimChanged, slave,
				&AudioChannelFeatureWidget::changeTrim);
		connect(master, &AudioChannelFeatureWidget::phantomPowerStatusChanged,
				slave, &AudioChannelFeatureWidget::changePhantomPowerStatus);
		connect(master, &AudioChannelFeatureWidget::highImpedanceStatusChanged,
				slave, &AudioChannelFeatureWidget::changeHighImpedanceStatus);
		connect(master, &AudioChannelFeatureWidget::muteStatusChanged, slave,
				&AudioChannelFeatureWidget::changeMuteStatus);
	} else {
		disconnect(master, &AudioChannelFeatureWidget::volumeChanged, slave,
				   &AudioChannelFeatureWidget::changeVolume);
		disconnect(master, &AudioChannelFeatureWidget::trimChanged, slave,
				   &AudioChannelFeatureWidget::changeTrim);
		disconnect(master,
				   &AudioChannelFeatureWidget::phantomPowerStatusChanged, slave,
				   &AudioChannelFeatureWidget::changePhantomPowerStatus);
		disconnect(
			master, &AudioChannelFeatureWidget::highImpedanceStatusChanged,
			slave, &AudioChannelFeatureWidget::changeHighImpedanceStatus);
		disconnect(master, &AudioChannelFeatureWidget::muteStatusChanged, slave,
				   &AudioChannelFeatureWidget::changeMuteStatus);
	}
}

void AudioControlDetailFeatureWidget::changeMeterValue(int channel, int value) {
	emit meterValueChanged(channel, value);
}
