#include "devicedetection.h"
#include "devicedetectionprocessor.h"
#include "ui_devicedetection.h"

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
}
