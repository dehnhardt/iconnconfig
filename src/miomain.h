#ifndef MIOMAIN_H
#define MIOMAIN_H

#include "devicedetection.h"
#include <QMainWindow>

namespace Ui {
class MioMain;
}

class DeviceMenuMapper : public QObject {
public:
  DeviceMenuMapper(Device *device) : device(device) {}
  Device *device;
};

class MioMain : public QMainWindow {
  Q_OBJECT

public:
  explicit MioMain(QWidget *parent = 0);
  ~MioMain();

public slots:
  void openDefaultDevice();

private slots:
  void on_actionQuit_triggered();
  void openDetectionWindow();
  void openDeviceGUI(QObject *m);
  void openDeviceGUI(Device *d);

private:
  // Members
  Ui::MioMain *ui;
  DeviceDetection *deviceDetectionWindow;
  void readSettings();
  void writeSettings();
  void connectSlots();
  void addDevicesToSelectionMenu(long defaultDeviceSN);

protected:
  void closeEvent(QCloseEvent *event);

signals:
  void deviceSelected(Device *d);
};

#endif // MIOMAIN_H
