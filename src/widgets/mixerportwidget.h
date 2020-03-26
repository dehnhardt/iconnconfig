#ifndef MIXERPORTWIDGET_H
#define MIXERPORTWIDGET_H

#include "../device.h"
#include "../sysex/getmixermetervalue.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QWidget>

class MixerPortWidget : public QWidget {
	Q_OBJECT
  public:
	explicit MixerPortWidget(unsigned int m_iPortId, Device *device,
							 QWidget *parent = nullptr);
	void setName(QString name);
	void addMixerPanel(QWidget *mixerPanel, unsigned int mixerChannelNumber);
	void addStretch() { m_pMixerPanelLayout->addStretch(); }

	void setNumberOfInputChannels(unsigned int iNumberOfInputChannels);
	void setNumberOfOutputChannels(unsigned int INumberOfOutputChannels);

  private: // members
	Device *m_pDevice = nullptr;
	QLabel *m_pPortNameLabel = nullptr;
	QPushButton *m_pVolButton = nullptr;
	QTimer *m_pVolumeTimer = nullptr;
	QHBoxLayout *m_pMixerPanelLayout = nullptr;
	unsigned int m_iPortId = 0;
	unsigned int m_iXNumber = 0;
	std::unique_ptr<GetMixerMeterValue> m_pGetMixerMeterValue = nullptr;
	std::map<unsigned int, QWidget *> m_MapAttachedChannels;
	unsigned int m_iNumberOfInputChannels = 0;
	unsigned int m_INumberOfOutputChannels = 0;
	unsigned int m_iCurrentMeterQuery = 0;

  private: // methods
	void createLayout();

  private slots:
	void getVolumes();
	void timerElapsed();

	// QWidget interface
  protected:
	void showEvent(QShowEvent *event) override;
	void hideEvent(QHideEvent *event) override;

  signals:
	void inMeterValueChanged(int channel, int value);
	void outMeterValueChanged(int channel, int value);
};

#endif // MIXERPORTWIDGET_H
