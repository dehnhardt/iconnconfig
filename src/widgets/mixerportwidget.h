#ifndef MIXERPORTWIDGET_H
#define MIXERPORTWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

class MixerPortWidget : public QWidget {
	Q_OBJECT
  public:
	explicit MixerPortWidget(unsigned int m_iPortId, QWidget *parent = nullptr);
	void setName(QString name);
	void addMixerPanel(QWidget *mixerPanel, unsigned int mixerChannelNumber);
	void addStretch() { m_pMixerPanelLayout->addStretch(); }

  private: // members
	QLabel *m_pPortNameLabel = nullptr;
	QHBoxLayout *m_pMixerPanelLayout = nullptr;
	unsigned int m_iPortId = 0;
	std::map<unsigned int, QWidget *> m_MapAttachedChannels;

  private: // methods
	void createLayout();

  signals:

  public slots:
};

#endif // MIXERPORTWIDGET_H
