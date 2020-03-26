#ifndef AUDIOMIXERCHANNEL_H
#define AUDIOMIXERCHANNEL_H

#include "../sysex/retmixerinputcontrol.h"
#include "../sysex/retmixeroutputcontrol.h"
#include "../sysex/retsetmixerinputcontrolvalue.h"
#include "../sysex/retsetmixerinputparm.h"
#include "../sysex/retsetmixeroutputparm.h"

#include "audiochannelfeaturewidget.h"
#include "controls/iconncalc.h"

#include <QFrame>
#include <QMenu>
#include <QObject>
#include <QToolButton>

namespace Ui {
class AudioChannelFeatureWidget;
}

class AudioMixerChannelWidget : public QFrame {
	Q_OBJECT

  public:
	explicit AudioMixerChannelWidget(Device *device, unsigned int portId,
									 unsigned int channelNumber,
									 ChannelDirection channelDirection,
									 QWidget *parent = nullptr);
	explicit AudioMixerChannelWidget(QWidget *parent = nullptr);

	~AudioMixerChannelWidget();

	void setMixerInputControl(
		std::shared_ptr<RetMixerInputControl> retMixerInputControl);
	void setMixerOutputControl(
		std::shared_ptr<RetMixerOutputControl> retMixerOutputControl);

  public slots:
	void changeMeterVolume(unsigned int channel, int value);

  protected:
	Ui::AudioChannelFeatureWidget *ui;
	void initControls();

  private:
	QToolButton *m_pBtnSelectConnection = nullptr;
	QMenu *m_pConnectionMenu = nullptr;
	Device *m_pDevice = nullptr;
	std::shared_ptr<ScaleCalc> m_pChannelCalc = nullptr;
	QTimer *m_pUpdateTimer = nullptr;

	unsigned int m_iPortId = 0;
	unsigned int m_iChannelNumber = 0;
	bool m_bIsMaster = false;
	ChannelDirection m_channelDirection = ChannelDirection::CD_NONE;
	std::shared_ptr<RetSetMixerInputParm> m_pMixerInputParm = nullptr;
	std::shared_ptr<RetSetMixerOutputParm> m_pMixerOutputParm = nullptr;
	std::shared_ptr<RetMixerInputControl> m_pMixerInputControl = nullptr;
	std::shared_ptr<RetMixerOutputControl> m_pMixerOutputControl = nullptr;

	std::shared_ptr<RetSetMixerInputControlValue>
		m_pretSetMixerInputControlValue = nullptr;
	/*	std::shared_ptr<RetSetMixerOutputControlValue>
			m_pretSetMixerOutputControlValue = nullptr;*/

  private: // methods
	void createInputMenu();
	void setInput(unsigned int audioSourcePortId,
				  unsigned int audioSourceChannelNumber);
	void setInputName(unsigned int audioSourcePortId,
					  std::string audioSourcePortName,
					  unsigned int audioSourceChannelNumber,
					  std::string audioSourceChannelName,
					  AudioPortType audioPortType);

	void createOutputMenu();
	void setOutput(std::vector<unsigned int> audioSinkChannelNumbers);
	void setOutputName(unsigned int audioSourcePortId,
					   std::string audioSourcePortName,
					   unsigned int audioSourceChannelNumber,
					   std::string audioSourceChannelName,
					   AudioPortType audioPortType);

	void queryInputValues();
	void queryOutputValues();
	void setInputValues(std::shared_ptr<RetSetMixerInputControlValue>
							retSetMixerInputControlValue);
  private slots:
	void changeInput(unsigned int audioSourcePortId,
					 std::string audioSourcePortName,
					 unsigned int audioSourceChannelNumber,
					 std::string audioSourceChannelName,
					 AudioPortType audioPortType);

	void changeOutput(unsigned int audioSourcePortId,
					  std::string audioSourcePortName,
					  unsigned int audioSourceChannelNumber,
					  std::string audioSourceChannelName,
					  AudioPortType audioPortType);
  signals:
	void linkStatusChanged(unsigned int m_iChannelNumber, bool status);
	void soloStatusChanged(bool status);
	void soloPFLStatusChanged(bool status);
	void invertStatusChanged(bool status);
	void muteStatusChanged(bool status);
	void volumeChanged(int volume);
	void panChanged(int trim);
};

#endif // AUDIOMIXERCHANNEL_H
