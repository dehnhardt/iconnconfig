#include "mixerportwidget.h"
#include <QVBoxLayout>

MixerPortWidget::MixerPortWidget(unsigned int portId, QWidget *parent)
	: QWidget(parent), m_iPortId(portId) {
	createLayout();
}

void MixerPortWidget::setName(QString name) { m_pPortNameLabel->setText(name); }

void MixerPortWidget::addMixerPanel(QWidget *mixerPanel,
									unsigned int mixerChannelNumber) {
	m_MapAttachedChannels[mixerChannelNumber] = mixerPanel;
	m_pMixerPanelLayout->addWidget(mixerPanel);
}

void MixerPortWidget::createLayout() {
	QBoxLayout *vBoxLayout = new QVBoxLayout();
	m_pMixerPanelLayout = new QHBoxLayout();
	m_pPortNameLabel = new QLabel();
	vBoxLayout->addWidget(m_pPortNameLabel);
	vBoxLayout->addLayout(m_pMixerPanelLayout);
	setLayout(vBoxLayout);
}
