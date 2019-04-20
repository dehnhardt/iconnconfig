#ifndef AUDIOMIXERCHANNEL_H
#define AUDIOMIXERCHANNEL_H

#include "audiochannelfeaturewidget.h"

#include <QLabel>
#include <QMenu>

class AudioMixerChannelWidget : public AudioChannelFeatureWidget {
  public:
	AudioMixerChannelWidget(Device *device, QWidget *parent = nullptr);

  private:
	QLabel *m_pSelectedInput = nullptr;
	QMenu *m_pInputMenu = nullptr;
	Device *m_pDevice = nullptr;

	void createInputMenu();

  private slots:
	void openInputPortsMenu(const QPoint &);
};

#endif // AUDIOMIXERCHANNEL_H
