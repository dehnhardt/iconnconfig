#include "devicedetection.h"
#include "config/configuration.h"
#include "devicedetectionprocessor.h"
#include "deviceselectiontablemodel.h"
#include "ui_devicedetection.h"

#include <QStandardItemModel>
#include <QTimer>

DeviceDetection::DeviceDetection(QWidget *parent)
    : QDialog(parent), ui(new Ui::DeviceDetection) {
  ui->setupUi(this);
  detectionProcessor = new DeviceDetectionProcessor();
  QTimer::singleShot(1000, this, SLOT(startDeviceDetection()));
}

DeviceDetection::~DeviceDetection() { delete ui; }

void DeviceDetection::startDeviceDetection() {
  detectionProcessor->startDeviceDetection();
  Devices *devices = Configuration::getInstance().getDevices();
  DeviceSelectionTableModel *deviceModel =
      new DeviceSelectionTableModel(devices);
  ui->tableView->setModel(deviceModel);
}
