#ifndef DEVICEDETECTION_H
#define DEVICEDETECTION_H

#include "devicedetectionprocessor.h"

#include <QDialog>

namespace Ui {
class DeviceDetection;
}

class DeviceDetection : public QDialog {
  Q_OBJECT

public:
  explicit DeviceDetection(QWidget *parent = 0);
  ~DeviceDetection();

private:
  Ui::DeviceDetection *ui = 0;
  DeviceDetectionProcessor *detectionProcessor = 0;

private slots:
  void startDeviceDetection();
};

#endif // DEVICEDETECTION_H
