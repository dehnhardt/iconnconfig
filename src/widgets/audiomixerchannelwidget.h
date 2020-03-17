#ifndef AUDIOMIXERCHANNEL_H
#define AUDIOMIXERCHANNEL_H

#include "../sysex/retmixerinputcontrol.h"
#include "../sysex/retmixeroutputcontrol.h"
#include "../sysex/retsetmixerinputparm.h"
#include "audiochannelfeaturewidget.h"
#include "controls/iconncalc.h"

#include <QMenu>
#include <QToolButton>

class AudioMixerChannelWidget : public AudioChannelFeatureWidget {
  public:
	AudioMixerChannelWidget(Device *device, unsigned int portId,
							unsigned int channelNumber,
							ChannelDirection channelDirection,
							QWidget *parent = nullptr);

	void setMixerInputControl(
		std::shared_ptr<RetMixerInputControl> retMixerInputControl);
	void setMixerOutputControl(
		std::shared_ptr<RetMixerOutputControl> retMixerOutputControl);

  private:
	QToolButton *m_pBtnSelectInput = nullptr;
	QMenu *m_pInputMenu = nullptr;
	Device *m_pDevice = nullptr;
	std::shared_ptr<ScaleCalc> m_pChannelCalc = nullptr;

	unsigned int m_iPortId = 0;
	unsigned int m_iChannelNumber = 0;
	ChannelDirection m_channelDirection = ChannelDirection::CD_NONE;
	std::shared_ptr<RetSetMixerInputParm> m_pMixerInputParm = nullptr;
	std::shared_ptr<RetMixerInputControl> m_pMixerInputControl = nullptr;
	std::shared_ptr<RetMixerOutputControl> m_pMixerOutputControl = nullptr;

  private: // methods
	void createInputMenu();
	void setInput(unsigned int audioSourcePortId,
				  unsigned int audioSourceChannelNumber);
	void setInputName(unsigned int audioSourcePortId,
					  std::string audioSourcePortName,
					  unsigned int audioSourceChannelNumber,
					  std::string audioSourceChannelName,
					  AudioPortType audioPortType);

  private slots:
	void changeInput(unsigned int audioSourcePortId,
					 std::string audioSourcePortName,
					 unsigned int audioSourceChannelNumber,
					 std::string audioSourceChannelName,
					 AudioPortType audioPortType);
};

#endif // AUDIOMIXERCHANNEL_H
