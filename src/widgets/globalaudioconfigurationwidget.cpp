#include "globalaudioconfigurationwidget.h"
#include "ui_globalaudioconfiguration.h"

#include <QMessageBox>

GlobalAudioConfigurationWidget::GlobalAudioConfigurationWidget(
	RetSetAudioGlobalParm *retSetAudioGlobalParam, QWidget *parent)
	: QWidget(parent), ui(new Ui::GlobalAudioConfiguration),
	  m_pRetSetAudioGlobalParam(retSetAudioGlobalParam) {
	ui->setupUi(this);
	m_pUpdateTimer = new QTimer();
	m_pUpdateTimer->setSingleShot(true);

	loadData();
	createConnections();
}

GlobalAudioConfigurationWidget::~GlobalAudioConfigurationWidget() {
	delete m_pUpdateTimer;
	delete ui;
}

void GlobalAudioConfigurationWidget::loadData() {

	for (unsigned int i =
			 m_pRetSetAudioGlobalParam->getMinAudioFramesBuffered();
		 i <= m_pRetSetAudioGlobalParam->getMaxAudioFramesBuffered(); i++) {
		ui->m_pCbCurrentAudioFramesBuffered->addItem(QString::number(i));
	}
	for (unsigned int i = m_pRetSetAudioGlobalParam->getMinAllowedSyncFactor();
		 i <= m_pRetSetAudioGlobalParam->getMaxAllowedSyncFactor(); i++) {
		ui->m_pCbCurrentSyncFactor->addItem(QString::number(i));
	}
	ui->m_pCbCurrentAudioFramesBuffered->setCurrentText(QString::number(
		m_pRetSetAudioGlobalParam->getCurrentAudioFramesBuffered()));

	ui->m_pCbCurrentSyncFactor->setCurrentText(
		QString::number(m_pRetSetAudioGlobalParam->getCurrentSyncFactor()));

	ui->m_pEditNumberAudioPorts->setText(
		QString::number(m_pRetSetAudioGlobalParam->getNumberOfAudioPorts()));

	int numberOfConfigBlocks =
		static_cast<int>(m_pRetSetAudioGlobalParam->getNumberOfConfigBlocks());
	ui->m_pTblAudioConfigurations->verticalHeader()->setSectionResizeMode(
		QHeaderView::ResizeToContents);
	ui->m_pTblAudioConfigurations->horizontalHeader()->setSectionResizeMode(
		QHeaderView::ResizeToContents);
	ui->m_pTblAudioConfigurations->horizontalHeader()->setStretchLastSection(
		true);
	ui->m_pTblAudioConfigurations->setRowCount(numberOfConfigBlocks);
	ui->m_pTblAudioConfigurations->setColumnCount(3);
	ui->m_pTblAudioConfigurations->setSelectionBehavior(
		QAbstractItemView::SelectRows);
	for (int i = 0; i < numberOfConfigBlocks; i++) {
		AUDIOConfiguration *audioConfiguration =
			m_pRetSetAudioGlobalParam->getAudioConfiguration(i);
		for (int j = 0; j < 3; j++) {
			QTableWidgetItem *item = new QTableWidgetItem();
			switch (j) {
			case 0:
				item->setText(
					QString::number(audioConfiguration->configurationNumber));
				break;
			case 1:
				item->setText(m_pRetSetAudioGlobalParam
								  ->translateBitDepthText(
									  audioConfiguration->bitDepthCode)
								  .c_str());
				break;
			case 2:
				item->setText(m_pRetSetAudioGlobalParam
								  ->translateSampleRateText(
									  audioConfiguration->sampleRateCode)
								  .c_str());
				break;
			}
			ui->m_pTblAudioConfigurations->setItem(i, j, item);
		}
	}
	ui->m_pTblAudioConfigurations->selectRow(static_cast<int>(
		m_pRetSetAudioGlobalParam->getNumberOfActiveAudioConfiguration() - 1));
}

void GlobalAudioConfigurationWidget::createConnections() {
	connect(ui->m_pCbCurrentAudioFramesBuffered,
			QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
			this, [=](const QString &text) {
				valueEdited(ui->m_pCbCurrentAudioFramesBuffered, text);
			});
	connect(ui->m_pCbCurrentSyncFactor,
			QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
			this, [=](const QString &text) {
				valueEdited(ui->m_pCbCurrentSyncFactor, text);
			});
	connect(ui->m_pTblAudioConfigurations, &QTableWidget::itemSelectionChanged,
			this, [=]() { valueEdited(ui->m_pTblAudioConfigurations, ""); });

	connect(m_pUpdateTimer, &QTimer::timeout, this,
			&GlobalAudioConfigurationWidget::save);

	// connect( this, &GlobalAudioConfigurationWidget::changeAudioConfiguration,
	// dev)
}

void GlobalAudioConfigurationWidget::valueEdited(QWidget *widget,
												 QString text) {
	QComboBox *cb = dynamic_cast<QComboBox *>(widget);
	if (cb) {
		if (cb->objectName() == "m_pCbCurrentAudioFramesBuffered") {
			m_pRetSetAudioGlobalParam->setCurrentAudioFramesBuffered(
				static_cast<unsigned int>(text.toInt()));
		}
		if (cb->objectName() == "m_pCbCurrentSyncFactor") {
			m_pRetSetAudioGlobalParam->setCurrentSyncFactor(
				static_cast<unsigned int>(text.toInt()));
		}
		m_pUpdateTimer->start(1000);
	} else {
		QTableWidget *tw = dynamic_cast<QTableWidget *>(widget);
		if (tw) {
			QModelIndexList list = tw->selectionModel()->selectedRows();
			if (list.size() > 0) {
				int row = list.begin()->row();
				if (m_pRetSetAudioGlobalParam
						->getNumberOfActiveAudioConfiguration() !=
					static_cast<unsigned int>(row + 1)) {
					if (QMessageBox::question(
							this, tr("Change Audio Configuration"),
							tr("Are you shure you want to change the global "
							   "audio "
							   "configuration?"),
							QMessageBox::No | QMessageBox::Ok) ==
						QMessageBox::Ok) {
						m_pRetSetAudioGlobalParam
							->setNumberOfActiveAudioConfiguration(
								static_cast<unsigned int>(row + 1));
						m_pUpdateTimer->start(1000);
					}
				}
			}
		}
	}
}

void GlobalAudioConfigurationWidget::save() {
	if (m_pRetSetAudioGlobalParam->execute() == 0)
		emit changeAudioConfiguration();
}
