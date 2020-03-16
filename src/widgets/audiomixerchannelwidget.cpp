#include "audiomixerchannelwidget.h"
#include "../sysex/getmixerinputparm.h"
#include "../sysex/retsetaudiochannelname.h"
#include "portdisplayhelper.h"
#include "ui_audiochannelfeaturewidget.h"

#include <QMenu>
#include <QVBoxLayout>
#include <sstream>

AudioMixerChannelWidget::AudioMixerChannelWidget(
	Device *device, unsigned int portId, unsigned int channelNumber,
	ChannelDirection channelDirection, QWidget *parent)
	: AudioChannelFeatureWidget(parent), m_pDevice(device), m_iPortId(portId),
	  m_iChannelNumber(channelNumber), m_channelDirection(channelDirection) {
	initControls();
	QVBoxLayout *l =
		static_cast<QVBoxLayout *>(ui->m_pFrmUpperMixerPanel->layout());
	if (m_channelDirection == ChannelDirection::CD_INPUT) {
		std::unique_ptr<GetMixerInputParm> getMixerInputParm =
			std::make_unique<GetMixerInputParm>(device);
		getMixerInputParm->setPortId(m_iPortId);
		getMixerInputParm->setMixerInputNumber(m_iChannelNumber);
		m_pMixerInputParm = std::dynamic_pointer_cast<RetSetMixerInputParm>(
			getMixerInputParm->querySmart());
		if (l) {
			m_pBtnSelectInput = new QToolButton();
			m_pBtnSelectInput->setCheckable(false);
			l->addWidget(new QLabel(tr("In")));
			l->addWidget(m_pBtnSelectInput);
			createInputMenu();
			m_pBtnSelectInput->setMenu(m_pInputMenu);
			m_pBtnSelectInput->setPopupMode(
				QToolButton::ToolButtonPopupMode::MenuButtonPopup);
			m_pBtnSelectInput->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
			setInput(m_pMixerInputParm->getAudioSourcePortId(),
					 m_pMixerInputParm->getAudioSourceChannelNumber());
		}
	}
	ui->m_pSlideVolume->setDebug(true);
}

void AudioMixerChannelWidget::setMixerInputControl(
	std::shared_ptr<RetMixerInputControl> retMixerInputControl) {
	this->m_pMixerInputControl = retMixerInputControl;

	// ui->m_pSlideVolume->setScaleType(PKSlider::ScaleType::DECIBEL);
	ui->m_pSlideVolume->setTickInterval(
		m_pMixerInputControl->getVolumeResolution());
	// ui->m_pSlideVolume->setResulution(256.0f);
	ui->m_pSlideVolume->setMinimum(
		m_pMixerInputControl->getMinimumVolumeValue());
	ui->m_pSlideVolume->setMaximum(
		m_pMixerInputControl->getMaximumVolumeValue());

	ui->m_pTbMute->setVisible(m_pMixerInputControl->getHasMuteControl());
	ui->m_pTbSolo->setVisible(m_pMixerInputControl->getHasSoloControl());
	ui->m_pTbPfl->setVisible(m_pMixerInputControl->getHasSoloPFLControl());
	ui->m_pTbInvert->setVisible(m_pMixerInputControl->getHasInvertControl());

	ui->m_pTbMute->setEnabled(m_pMixerInputControl->getMuteControlEditable());
	ui->m_pTbSolo->setEnabled(m_pMixerInputControl->getSoloControlEditable());
	ui->m_pTbPfl->setEnabled(m_pMixerInputControl->getSoloPFLControlEditable());
	ui->m_pTbInvert->setEnabled(
		m_pMixerInputControl->getInvertControlEditable());
}

void AudioMixerChannelWidget::createInputMenu() {
	QFont menuFont = ui->m_pFrmUpperMixerPanel->font();
	m_pInputMenu = new QMenu();
	m_pInputMenu->setFont(menuFont);
	std::shared_ptr<AudioPortStructure> aps =
		m_pDevice->getAudioPortStructure();
	QAction *qANone = m_pInputMenu->addAction(tr(" - none - "));
	connect(qANone, &QAction::triggered, [=]() {
		std::cout << "Audio port selected" << std::endl;
		changeInput(0, "", 0, "", AudioPortType::APT_NONE);
	});
	AudioPortStructure::iterator itPortType;
	for (itPortType = aps->begin(); itPortType != aps->end(); ++itPortType) {
		int section = itPortType->first;
		AudioPortType audioPortType = static_cast<AudioPortType>(section);
		std::string portTypeName =
			PortDisplayHelper::getAudioPortTypeName(audioPortType);
		QIcon portIcon =
			PortDisplayHelper::getAudioPortIcon(AudioPortType(section));
		QMenu *audioPortTypeMenu =
			m_pInputMenu->addMenu(portIcon, tr(portTypeName.c_str()));
		audioPortTypeMenu->setFont(menuFont);
		std::vector<std::shared_ptr<RetSetAudioPortParm>>::iterator itPort;
		std::vector<std::shared_ptr<RetSetAudioPortParm>> *ports =
			itPortType->second;
		for (itPort = ports->begin(); itPort != ports->end(); itPort++) {
			std::shared_ptr<RetSetAudioPortParm> app = *itPort;
			unsigned int portId = app->getPortId();
			QMenu *portMenu = audioPortTypeMenu->addMenu(
				portIcon,
				QString::number(portId) + ": " + app->getPortName().c_str());
			portMenu->setFont(menuFont);
			AudioChannelNames::iterator channelIt;
			AudioChannelNames channelNames = m_pDevice->getAudioChannelNames(
				portId, ChannelDirection::CD_INPUT);
			for (channelIt = channelNames.begin();
				 channelIt != channelNames.end(); ++channelIt) {
				unsigned int channelId = channelIt->first;
				std::shared_ptr<RetSetAudioChannelName> channelName =
					channelIt->second;
				QAction *a = portMenu->addAction(
					portIcon, QString::number(channelId) + ": " +
								  channelName->getChannelName().c_str());
				connect(a, &QAction::triggered, [=]() {
					std::cout << "Audio port selected" << std::endl;
					changeInput(portId, app->getPortName(), channelId,
								channelName->getChannelName(),
								AudioPortType(section));
				});
			}
		}
	}
}

void AudioMixerChannelWidget::setInput(unsigned int audioSourcePortId,
									   unsigned int audioSourceChannelNumber) {
	if (audioSourcePortId == 0 || audioSourceChannelNumber == 0) {
		setInputName(0, "", 0, "", AudioPortType::APT_NONE);
	}
	std::shared_ptr<AudioPortStructure> aps =
		m_pDevice->getAudioPortStructure();
	AudioPortStructure::iterator itPortType;
	for (itPortType = aps->begin(); itPortType != aps->end(); ++itPortType) {
		int section = itPortType->first;
		std::vector<std::shared_ptr<RetSetAudioPortParm>>::iterator itPort;
		std::vector<std::shared_ptr<RetSetAudioPortParm>> *ports =
			itPortType->second;
		for (itPort = ports->begin(); itPort != ports->end(); itPort++) {
			std::shared_ptr<RetSetAudioPortParm> app = *itPort;
			if (audioSourcePortId == app->getPortId()) {
				AudioChannelNames channelNames =
					m_pDevice->getAudioChannelNames(audioSourcePortId,
													ChannelDirection::CD_INPUT);
				std::shared_ptr<RetSetAudioChannelName> channelName =
					channelNames[audioSourceChannelNumber];
				setInputName(audioSourcePortId, app->getPortName(),
							 audioSourceChannelNumber,
							 channelName->getChannelName(),
							 static_cast<AudioPortType>(section));
				return;
			}
		}
	}
}

void AudioMixerChannelWidget::setInputName(
	unsigned int audioSourcePortId, std::string audioSourcePortName,
	unsigned int audioSourceChannelNumber, std::string audioSourceChannelName,
	AudioPortType audioPortType) {
	if (audioSourcePortId == 0 || audioSourceChannelNumber == 0) {
		m_pBtnSelectInput->setToolTip(tr("none"));
		m_pBtnSelectInput->setText(tr("none"));
		m_pBtnSelectInput->setIcon(QIcon());
		return;
	}
	std::stringstream name;
	name << tr("Port ").toStdString() << std::dec << audioSourcePortId << ": "
		 << audioSourcePortName << " - " << tr("Channel ").toStdString()
		 << audioSourceChannelNumber << ": " << audioSourceChannelName;
	m_pBtnSelectInput->setToolTip(name.str().c_str());
	m_pBtnSelectInput->setText(QString::number(audioSourcePortId) + " - " +
							   QString::number(audioSourceChannelNumber));
	m_pBtnSelectInput->setIcon(
		PortDisplayHelper::getAudioPortIcon(audioPortType));
}

void AudioMixerChannelWidget::changeInput(unsigned int audioSourcePortId,
										  std::string audioSourcePortName,
										  unsigned int audioSourceChannelNumber,
										  std::string audioSourceChannelName,
										  AudioPortType audioPortType) {
	setInputName(audioSourcePortId, audioSourcePortName,
				 audioSourceChannelNumber, audioSourceChannelName,
				 audioPortType);
}
