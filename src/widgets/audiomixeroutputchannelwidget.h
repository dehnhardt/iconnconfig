#ifndef AUDIOMIXEROUTPUTCHANNELWIDGET_H
#define AUDIOMIXEROUTPUTCHANNELWIDGET_H

#include "../sysex/retmixeroutputcontrol.h"
#include "../sysex/retsetmixeroutputcontrolvalue.h"
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
	void refreshOutput();
	void refreshStatus() override;

  private:
	QToolButton *m_pBtnSelectConnection = nullptr;
	QMenu *m_pConnectionMenu = nullptr;
	std::shared_ptr<ScaleCalc> m_pChannelCalc = nullptr;

	std::shared_ptr<RetSetMixerOutputParm> m_pMixerOutputParm = nullptr;
	std::shared_ptr<RetMixerOutputControl> m_pMixerOutputControl = nullptr;

	std::shared_ptr<RetSetMixerOutputControlValue>
		m_pRetSetMixerOutputControlValue = nullptr;

  private: // methods
	void createOutputMenu();
	void setOutput(std::vector<unsigned int> audioSinkChannelNumbers);
	void setOutputName(unsigned int audioSourcePortId,
					   std::string audioSourcePortName,
					   unsigned int audioSourceChannelNumber,
					   std::string audioSourceChannelName,
					   AudioPortType audioPortType);

	void queryOutputValues();
	void setOutputValues(std::shared_ptr<RetSetMixerOutputControlValue>
							 retSetMixerOutputControlValue);

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
