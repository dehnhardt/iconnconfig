#include "devicedetection.h"
#include "config/configuration.h"
#include "devicedetectionprocessor.h"
#include "deviceselectiontablemodel.h"
#include "events/events.h"
#include "iconnconfigmain.h"
#include "ui_devicedetection.h"

#include <QStandardItemModel>
#include <QTimer>

DeviceDetection::DeviceDetection(QWidget *parent)
	: QDialog(parent), m_pUi(new Ui::DeviceDetection) {
	connect(this, SIGNAL(openDefaultDevice()), parent,
			SLOT(openDefaultDevice()));
	m_pUi->setupUi(this);
	m_pUi->tableView->horizontalHeader()->setSectionResizeMode(
		QHeaderView::ResizeToContents);
	readSettings();
	detectionProcessor = new DeviceDetectionProcessor(this);
	QTimer::singleShot(1000, this, SLOT(startDeviceDetection()));
}

DeviceDetection::~DeviceDetection() { delete m_pUi; }

void DeviceDetection::customEvent(QEvent *e) {
	if (e->type() == static_cast<QEvent::Type>(PROGRESS_EVENT_TYPE)) {
		m_pUi->progressBar->setValue(
			static_cast<int>((dynamic_cast<ProgressEvent *>(e))->getValue()));
	}
}

void DeviceDetection::startDeviceDetection() {
	unsigned int maximum = detectionProcessor->getMidiOutPortCount() *
						   detectionProcessor->getMidiInPortCount();
	m_pUi->progressBar->setMaximum(static_cast<int>(maximum));
	m_pUi->progressBar->setMinimum(0);
	m_pUi->progressBar->setValue(0);
	detectionProcessor->startDeviceDetection();
	Devices *devices = Configuration::getInstance().getDevices();
	DeviceSelectionTableModel *deviceModel =
		new DeviceSelectionTableModel(devices);
	m_pUi->tableView->setModel(deviceModel);
	m_pUi->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void DeviceDetection::on_buttonBox_accepted() {
	writeSettings();
	emit openDefaultDevice();
}

void DeviceDetection::setProgressBar(int value) {
	m_pUi->progressBar->setValue(value);
}

void DeviceDetection::writeSettings() {
	QSettings *settings = Configuration::getInstance().getSettings();
	settings->beginGroup("DeviceDetectionWindow");
	settings->setValue("size", size());
	settings->setValue("pos", pos());
	settings->endGroup();
}

void DeviceDetection::readSettings() {
	QSettings *settings = Configuration::getInstance().getSettings();
	settings->beginGroup("DeviceDetectionWindow");
	resize(settings->value("size", QSize(400, 400)).toSize());
	move(settings->value("pos", QPoint(200, 200)).toPoint());
	settings->endGroup();
}

void DeviceDetection::closeEvent(QCloseEvent *event) {
	writeSettings();
	event->accept();
}
