#ifndef DEVICEDETECTION_H
#define DEVICEDETECTION_H

#include "devicedetectionprocessor.h"

#include <QAbstractButton>
#include <QCloseEvent>
#include <QDialog>

namespace Ui {
class DeviceDetection;
}

class DeviceDetection : public QDialog {
  Q_OBJECT

public:
  explicit DeviceDetection(QWidget *parent = 0);
  ~DeviceDetection();
  void customEvent(QEvent *e);

private:
  Ui::DeviceDetection *ui = 0;
  DeviceDetectionProcessor *detectionProcessor = 0;
  void writeSettings();
  void readSettings();

protected:
  void closeEvent(QCloseEvent *event);

private slots:
  void startDeviceDetection();
  void on_buttonBox_accepted();
  void setProgressBar(int value);

signals:
  void openDeviceGUI();
};

#endif // DEVICEDETECTION_H
