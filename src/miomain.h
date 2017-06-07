#ifndef MIOMAIN_H
#define MIOMAIN_H

#include "devicedetection.h"
#include "sysex/saverestore.h"
#include "widgets/centralwidget.h"

#include <QMainWindow>
#include <QToolBar>

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
  void replacePanel(QWidget *w);

public slots:
  void openDefaultDevice();

private slots:
  void on_actionQuit_triggered();
  void openDetectionWindow();
  void openDeviceGUI(QObject *m);
  void openDeviceGUI(Device *d);
	void storeToDevice();
	void restoreFromDevice();
	void resetToFactoryDefaults();

private:
  // Members
  Ui::MioMain *ui;
  DeviceDetection *deviceDetectionWindow = 0;
  QToolBar *toolBar = 0;
  std::map<Qt::DockWidgetArea, std::vector<QDockWidget *>> dockWidgetAreas;
  QString title;
	Device *currentDevice = 0;

  // methods
  void readSettings();
  bool readDevicesFromSettings();
  void writeSettings();
  void writeDevicesToSettings();
  void connectSlots();
  void addDevicesToSelectionMenu(long defaultDeviceSN);

  void addDock(QDockWidget *widget,
               Qt::DockWidgetArea area = Qt::NoDockWidgetArea);

  void clearDocWidgets();
	void saveRestore(SaveRestore::SaveResstoreId saveRestoreId);
	void addDeviceToolButtons();

protected:
  void closeEvent(QCloseEvent *event);

signals:
  void deviceSelected(Device *d);
};

#endif // MIOMAIN_H
