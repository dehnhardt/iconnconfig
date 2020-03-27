#ifndef AUDIOMIXERINPUTCHANNELWIDGET_H
#define AUDIOMIXERINPUTCHANNELWIDGET_H

#include "../sysex/retmixerinputcontrol.h"
#include "../sysex/retmixeroutputcontrol.h"
#include "../sysex/retsetmixerinputcontrolvalue.h"
#include "../sysex/retsetmixerinputparm.h"
#include "../sysex/retsetmixeroutputparm.h"

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
										  ChannelDirection channelDirection,
										  QWidget *parent = nullptr);
	explicit AudioMixerInputChannelWidget(QWidget *parent = nullptr);

	~AudioMixerInputChannelWidget() override;

	void setMixerInputControl(
		std::shared_ptr<RetMixerInputControl> retMixerInputControl);

	void setMaster(bool isMaster, QString channel2Name) override;
	void refreshStatus() override;

  protected:
	void initControls();

  private:
	QToolButton *m_pBtnSelectConnection = nullptr;
	QMenu *m_pConnectionMenu = nullptr;
	std::shared_ptr<ScaleCalc> m_pChannelCalc = nullptr;
	QTimer *m_pUpdateTimer = nullptr;

	std::shared_ptr<RetSetMixerInputParm> m_pMixerInputParm = nullptr;
	std::shared_ptr<RetMixerInputControl> m_pMixerInputControl = nullptr;

	std::shared_ptr<RetSetMixerInputControlValue>
		m_pRetSetMixerInputControlValue = nullptr;

  private: // methods
	void createInputMenu();
	void setInput(unsigned int audioSourcePortId,
				  unsigned int audioSourceChannelNumber);
	void setInputName(unsigned int audioSourcePortId,
					  std::string audioSourcePortName,
					  unsigned int audioSourceChannelNumber,
					  std::string audioSourceChannelName,
					  AudioPortType audioPortType);

	void queryInputValues();
	void setInputValues(std::shared_ptr<RetSetMixerInputControlValue>
							retSetMixerInputControlValue);
  private slots:
	virtual void changeLinkStatus(bool enabled) override;
	void changeInput(unsigned int audioSourcePortId,
					 std::string audioSourcePortName,
					 unsigned int audioSourceChannelNumber,
					 std::string audioSourceChannelName,
					 AudioPortType audioPortType);
};

#endif // AUDIOMIXERINPUTCHANNELWIDGET_H
