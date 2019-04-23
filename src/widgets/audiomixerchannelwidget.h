#ifndef AUDIOMIXERCHANNEL_H
#define AUDIOMIXERCHANNEL_H

#include "../sysex/retsetmixerinputparm.h"
#include "audiochannelfeaturewidget.h"

#include <QMenu>
#include <QToolButton>

class AudioMixerChannelWidget : public AudioChannelFeatureWidget {
  public:
	AudioMixerChannelWidget(Device *device, unsigned int portId,
							unsigned int channelNumber,
							ChannelDirection channelDirection,
							QWidget *parent = nullptr);

  private:
	QToolButton *m_pBtnSelectInput = nullptr;
	QMenu *m_pInputMenu = nullptr;
	Device *m_pDevice = nullptr;

	unsigned int m_iPortId = 0;
	unsigned int m_iChannelNumber = 0;
	ChannelDirection m_channelDirection = ChannelDirection::CD_NONE;
	std::shared_ptr<RetSetMixerInputParm> m_pMixerInputParm = nullptr;

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
