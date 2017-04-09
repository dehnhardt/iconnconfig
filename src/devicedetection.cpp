#include "devicedetection.h"
#include "config/configuration.h"
#include "devicedetectionprocessor.h"
#include "deviceselectiontablemodel.h"
#include "events/events.h"
#include "miomain.h"
#include "ui_devicedetection.h"

#include <QStandardItemModel>
#include <QTimer>

DeviceDetection::DeviceDetection(QWidget *parent)
    : QDialog(parent), ui(new Ui::DeviceDetection) {
  connect(this, SIGNAL(openDefaultDevice()), parent, SLOT(openDefaultDevice()));
  ui->setupUi(this);
  readSettings();
  detectionProcessor = new DeviceDetectionProcessor(this);
  QTimer::singleShot(1000, this, SLOT(startDeviceDetection()));
}

DeviceDetection::~DeviceDetection() { delete ui; }

void DeviceDetection::customEvent(QEvent *e) {
  if (e->type() == (QEvent::Type)PROGRESS_EVENT_TYPE) {
    ui->progressBar->setValue(((ProgressEvent *)e)->getValue());
  }
}

void DeviceDetection::startDeviceDetection() {
  int portCount = detectionProcessor->getMddiOutPortCount() *
                  detectionProcessor->getMidiInPortCount();
#ifdef __MIO_SIMULATE__
  ui->progressBar->setMaximum(portCount + 27);
#else
  ui->progressBar->setMaximum(portCount);
#endif
  ui->progressBar->setMinimum(0);
  ui->progressBar->setValue(0);
  detectionProcessor->startDeviceDetection();
  Devices *devices = Configuration::getInstance().getDevices();
  DeviceSelectionTableModel *deviceModel =
      new DeviceSelectionTableModel(devices);
  ui->tableView->setModel(deviceModel);
  ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void DeviceDetection::on_buttonBox_accepted() {
  writeSettings();
  emit openDefaultDevice();
}

void DeviceDetection::setProgressBar(int value) {
  ui->progressBar->setValue(value);
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
