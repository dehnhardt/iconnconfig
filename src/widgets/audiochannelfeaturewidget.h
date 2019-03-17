#ifndef AUDIOCHANNELFEATUREWIDGET_H
#define AUDIOCHANNELFEATUREWIDGET_H

#include "../sysex/retsetaudiocontroldetail.h"

#include <QFrame>

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
	RetSetAudioControlDetail *m_pAudioControlDetail = nullptr;
	void setRetSetAudioControlDetail(
		RetSetAudioControlDetail *retSetAudioControlDetail);
};

#endif // AUDIOCHANNELFEATUREWIDGET_H
