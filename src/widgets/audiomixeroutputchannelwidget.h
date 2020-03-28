#ifndef AUDIOMIXEROUTPUTCHANNELWIDGET_H
#define AUDIOMIXEROUTPUTCHANNELWIDGET_H

#include "../sysex/retmixerinputcontrol.h"
#include "../sysex/retmixeroutputcontrol.h"
#include "../sysex/retsetmixerinputcontrolvalue.h"
#include "../sysex/retsetmixerinputparm.h"
#include "../sysex/retsetmixeroutputparm.h"

#include "audiochannelfeaturewidget.h"
#include "audiomixerchannelwidget.h"
#include "controls/iconncalc.h"

#include <QFrame>
#include <QMenu>
#include <QObject>
#include <QToolButton>

namespace Ui {
class AudioChannelFeatureWidget;
}

class AudioMixerOutputChannelWidget : public AudioMixerChannelWidget {
	Q_OBJECT

  public:
	explicit AudioMixerOutputChannelWidget(Device *device, unsigned int portId,
										   unsigned int channelNumber,
										   ChannelDirection channelDirection,
										   QWidget *parent = nullptr);
	explicit AudioMixerOutputChannelWidget(QWidget *parent = nullptr);

	~AudioMixerOutputChannelWidget() override;

	void setMixerOutputControl(
		std::shared_ptr<RetMixerOutputControl> retMixerOutputControl);

	void setMaster(bool isMaster, QString channel2Name) override;
	void refreshStatus() override;

  public slots:
	void changeMeterVolume(unsigned int channel, int value);

  protected:
	void initControls();

  private:
	QToolButton *m_pBtnSelectConnection = nullptr;
	QMenu *m_pConnectionMenu = nullptr;
	std::shared_ptr<ScaleCalc> m_pChannelCalc = nullptr;
	QTimer *m_pUpdateTimer = nullptr;

	std::shared_ptr<RetSetMixerOutputParm> m_pMixerOutputParm = nullptr;
	std::shared_ptr<RetMixerOutputControl> m_pMixerOutputControl = nullptr;

	/*	std::shared_ptr<RetSetMixerOutputControlValue>
			m_pretSetMixerOutputControlValue = nullptr;*/

  private: // methods
	void createOutputMenu();
	void setOutput(std::vector<unsigned int> audioSinkChannelNumbers);
	void setOutputName(unsigned int audioSourcePortId,
					   std::string audioSourcePortName,
					   unsigned int audioSourceChannelNumber,
					   std::string audioSourceChannelName,
					   AudioPortType audioPortType);

	void queryOutputValues();
  private slots:
	void audioChannelValueChanged() override;
	virtual void changeLinkStatus(bool enabled) override;
	void changeOutput(unsigned int audioSourcePortId,
					  std::string audioSourcePortName,
					  unsigned int audioSourceChannelNumber,
					  std::string audioSourceChannelName,
					  AudioPortType audioPortType);
};

#endif // AUDIOMIXEROUTPUTCHANNELWIDGET_H
