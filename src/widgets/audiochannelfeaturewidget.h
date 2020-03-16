#ifndef AUDIOCHANNELFEATUREWIDGET_H
#define AUDIOCHANNELFEATUREWIDGET_H

#include "../sysex/getaudiocontroldetailvalue.h"
#include "../sysex/retsetaudiocontroldetail.h"
#include "../sysex/retsetaudiocontroldetailvalue.h"

#include "controls/calc/dbcalc.h"

#include <QFrame>
#include <QTimer>

namespace Ui {
class AudioChannelFeatureWidget;
}

class AudioChannelFeatureWidget : public QFrame {
	Q_OBJECT

  public:
	explicit AudioChannelFeatureWidget(
		std::shared_ptr<RetSetAudioControlDetail> retSetAudioControlDetail,
		QWidget *parent = nullptr);
	explicit AudioChannelFeatureWidget(QWidget *parent = nullptr);
	~AudioChannelFeatureWidget();
	void setMaster(bool isMaster, QString channel2Name);
	bool getLinkStatus();
	QString getChannelName();

	int getChannelId() const;
	void setChannelId(int value);

  protected:
	Ui::AudioChannelFeatureWidget *ui;
	QTimer *m_pUpdateTimer = nullptr;
	int m_iDetailId = 0;
	int m_iChannelId = 0;
	bool m_bIsMaster = false;
	std::shared_ptr<DbCalc> m_pSc1;

	std::shared_ptr<RetSetAudioControlDetail> m_pAudioControlDetail;
	std::shared_ptr<RetSetAudioControlDetailValue>
		m_pRetSetAudioControlDetailValue = nullptr;

	void initControls();
	void setRetSetAudioControlDetail(
		std::shared_ptr<RetSetAudioControlDetail> retSetAudioControlDetail);
	void queryValues();
	void setValues(std::shared_ptr<RetSetAudioControlDetailValue>
					   m_pRetSetAudioControlDetailValue);
	void createConnections();

  signals:
	void linkStatusChanged(int detailId, bool status);
	void phantomPowerStatusChanged(bool status);
	void highImpedanceStatusChanged(bool status);
	void muteStatusChanged(bool status);
	void volumeChanged(int volume);
	void trimChanged(int trim);

  private slots:
	void audioChannelValueChanged();

  public slots:
	void changeLinkStatus(unsigned int detailId, bool enabled);
	void changePhantomPowerStatus(bool enabled);
	void changeHighImpedanceStatus(bool enabled);
	void changeMuteStatus(bool enabled);
	void changeVolume(int volume);
	void changeTrim(int trim);
	void setVolume(int volume);
	void changeMeterVolume(int channel, int value);
};

#endif // AUDIOCHANNELFEATUREWIDGET_H
