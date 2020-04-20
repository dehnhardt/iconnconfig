#ifndef AUDIOMIXERINPUTCHANNELWIDGET_H
#define AUDIOMIXERINPUTCHANNELWIDGET_H

#include "../sysex/retmixerinputcontrol.h"
#include "../sysex/retsetmixerinputcontrolvalue.h"
#include "../sysex/retsetmixerinputparm.h"

#include "audiochannelfeaturewidget.h"
#include "controls/iconncalc.h"

#include "audiomixerchannelwidget.h"

#include <QMenu>
#include <QObject>
#include <QToolButton>

namespace Ui {
class AudioChannelFeatureWidget;
}

class AudioMixerInputChannelWidget : public AudioMixerChannelWidget {
	Q_OBJECT

  public:
	explicit AudioMixerInputChannelWidget(Device *device, unsigned int portId,
										  unsigned int channelNumber,
										  pk::ChannelDirection channelDirection,
										  QWidget *parent = nullptr);
	explicit AudioMixerInputChannelWidget(QWidget *parent = nullptr);

	~AudioMixerInputChannelWidget() override;

	void setMixerInputControl(
		std::shared_ptr<RetMixerInputControl> retMixerInputControl);

	void setMaster(bool isMaster, QString channel2Name) override;
	void refreshInput();
	void refreshStatus() override;

	void
	setConnectedOutputChannel(const AudioChannelId &iConnectedOutputChannel);

  private:
	std::shared_ptr<ScaleCalc> m_pChannelCalc = nullptr;

	std::shared_ptr<RetSetMixerInputParm> m_pMixerInputParm = nullptr;
	std::shared_ptr<RetMixerInputControl> m_pMixerInputControl = nullptr;

	std::shared_ptr<RetSetMixerInputControlValue>
		m_pRetSetMixerInputControlValue = nullptr;

	AudioPortId m_iSourcePortId = 0;
	AudioChannelId m_iSourceChannelId = 0;
	AudioChannelId m_iConnectedOutputChannel = 0;

  private: // methods
	void createInputMenu();
	void setInput(unsigned int audioSourcePortId,
				  unsigned int audioSourceChannelNumber);
	void setInputName(unsigned int audioSourcePortId,
					  std::string audioSourcePortName,
					  unsigned int audioSourceChannelNumber,
					  std::string audioSourceChannelName,
					  pk::AudioPortType audioPortType);

	void queryInputValues();
	void setInputValues(std::shared_ptr<RetSetMixerInputControlValue>
							retSetMixerInputControlValue);

  private slots:
	void audioChannelValueChanged() override;
	virtual void changeLinkStatus(bool enabled) override;
	void changeInput(unsigned int audioSourcePortId,
					 std::string audioSourcePortName,
					 unsigned int audioSourceChannelNumber,
					 std::string audioSourceChannelName,
					 pk::AudioPortType audioPortType);
};

#endif // AUDIOMIXERINPUTCHANNELWIDGET_H
