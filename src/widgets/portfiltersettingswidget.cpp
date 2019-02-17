#include "portfiltersettingswidget.h"
#include "ui_portfiltersettingswidget.h"

PortFilterSettingsWidget::PortFilterSettingsWidget(QWidget *parent)
	: QWidget(parent), ui(new Ui::PortFilterSettingsWidget) {
	ui->setupUi(this);
	ui->m_pTblMidiControllerFilter->horizontalHeader()->setSectionResizeMode(
		QHeaderView::ResizeToContents);
	ui->m_pTblChannelMessageFilter->horizontalHeader()->setSectionResizeMode(
		QHeaderView::ResizeToContents);
	ui->m_pTblChannelMessageFilter->verticalHeader()->setSectionResizeMode(
		QHeaderView::ResizeToContents);
}

PortFilterSettingsWidget::~PortFilterSettingsWidget() { delete ui; }
