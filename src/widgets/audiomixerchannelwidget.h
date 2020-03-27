#ifndef AUDIOMIXERCHANNELWIDGET_H
#define AUDIOMIXERCHANNELWIDGET_H

#include <QFrame>
#include <QObject>

#include "../device.h"

namespace Ui {
class AudioChannelFeatureWidget;
}

class AudioMixerChannelWidget : public QFrame {
	Q_OBJECT
  public:
	AudioMixerChannelWidget(Device *device, unsigned int portId,
							unsigned int channelNumber,
							ChannelDirection channelDirection, QWidget *parent);

	AudioMixerChannelWidget(QWidget *parent);
	~AudioMixerChannelWidget();

	virtual void setMaster(bool isMaster, QString channel2Name) = 0;
	virtual void refreshStatus() = 0;

  protected:
	Ui::AudioChannelFeatureWidget *ui;
	ChannelDirection m_channelDirection = ChannelDirection::CD_NONE;
	Device *m_pDevice = nullptr;
	bool channelInit = false;
	unsigned int m_iPortId = 0;
	unsigned int m_iChannelNumber = 0;
	bool m_bIsMaster = false;

	void initControls();

  signals:
	void linkStatusChanged(unsigned int m_iChannelNumber, bool status);
	void soloStatusChanged(bool status);
	void soloPFLStatusChanged(bool status);
	void invertStatusChanged(bool status);
	void muteStatusChanged(bool status);
	void volumeChanged(int volume);
	void panChanged(int trim);

  public slots:
	virtual void changeLinkStatus(bool enabled) = 0;
	void changeSoloStatus(bool enabled);
	void changePFLStatus(bool enabled);
	void changeInvertStatus(bool enabled);
	void changeMuteStatus(bool enabled);
	void changeVolume(int volume);
	void changePan(int pan);
	void changeMeterVolume(unsigned int channel, int value);
};

#endif // AUDIOMIXERCHANNELWIDGET_H
