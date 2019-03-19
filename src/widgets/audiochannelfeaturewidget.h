#ifndef AUDIOCHANNELFEATUREWIDGET_H
#define AUDIOCHANNELFEATUREWIDGET_H

#include "../sysex/getaudiocontroldetailvalue.h"
#include "../sysex/retsetaudiocontroldetail.h"
#include "../sysex/retsetaudiocontroldetailvalue.h"

#include <QFrame>
#include <QTimer>

namespace Ui {
class AudioChannelFeatureWidget;
}

class AudioChannelFeatureWidget : public QFrame {
	Q_OBJECT

  public:
	explicit AudioChannelFeatureWidget(
		RetSetAudioControlDetail *retSetAudioControlDetail,
		QWidget *parent = nullptr);
	~AudioChannelFeatureWidget();

  private:
	Ui::AudioChannelFeatureWidget *ui;
	QTimer *m_pUpdateTimer = nullptr;

	RetSetAudioControlDetail *m_pAudioControlDetail = nullptr;
	RetSetAudioControlDetailValue *m_pRetSetAudioControlDetailValue = nullptr;

	void setRetSetAudioControlDetail(
		RetSetAudioControlDetail *retSetAudioControlDetail);
	void queryValues();
	void
	setValues(RetSetAudioControlDetailValue *m_pRetSetAudioControlDetailValue);
	void createConnections();

  public slots:
	void audioChannelValueChanged();
};

#endif // AUDIOCHANNELFEATUREWIDGET_H
