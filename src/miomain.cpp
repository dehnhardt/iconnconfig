#include "miomain.h"
#include "config/configuration.h"
#include "devicedetection.h"
#include "sysex/commands.h"
#include "sysex/midi.h"
#include "ui_miomain.h"
#include "widgets/deviceinfowidget.h"
#include "widgets/devicewidget.h"
#include "widgets/portswidget.h"

#include <QCloseEvent>
#include <QTimer>
#include <QtDebug>

MioMain::MioMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::MioMain) {
  ui->setupUi(this);
  setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::ForceTabbedDocks |
                 QMainWindow::VerticalTabs);
  readSettings();
  QTimer::singleShot(100, this, SLOT(openDetectionWindow()));
}

MioMain::~MioMain() {
  delete deviceDetectionWindow;
  delete ui;
}

void MioMain::openDefaultDevice() {
  long defaultDeviceSN = Configuration::getInstance().getDefaultDevice();
  Device *d = Configuration::getInstance().getDevices()->at(defaultDeviceSN);
  openDeviceGUI(d);
}

void MioMain::openDeviceGUI(Device *d) {
  Commands *c = d->getCommands();
  if (c == 0) {
    // TODO throw error
    exit(2);
  }
  setWindowTitle(this->windowTitle() + QString(": ") +
                 QString::fromStdString(d->getDeviceName()));
  DeviceWidget *deviceWidget = new DeviceWidget(this, d);
  setCentralWidget(deviceWidget);

  DeviceInfoWidget *deviceInfoWidget = new DeviceInfoWidget(this, d);
  this->addDockWidget(Qt::LeftDockWidgetArea, deviceInfoWidget);

  PortsWidget *portsWidget = new PortsWidget(this, d);
  this->addDockWidget(Qt::LeftDockWidgetArea, portsWidget);
  tabifyDockWidget(deviceInfoWidget, portsWidget);
  if (c->isCommandSupported(SysExMessage::GET_ETHERNET_PORT_INFO)) {
    std::cout << "EthernetPortsAvailable";
  }

  QSettings *settings = Configuration::getInstance().getSettings();
  settings->beginGroup("MainWindow");
  settings->beginGroup("Docks");
  restoreState(settings->value("DockWindows").toByteArray());
  settings->endGroup();
  settings->endGroup();
}

void MioMain::closeEvent(QCloseEvent *event) {
  writeSettings();
  event->accept();
}

void MioMain::openDetectionWindow() {
  deviceDetectionWindow = new DeviceDetection(this);
  deviceDetectionWindow->exec();
}

void MioMain::writeSettings() {
  QSettings *settings = Configuration::getInstance().getSettings();
  settings->beginGroup("MainWindow");
  settings->setValue("size", size());
  settings->setValue("pos", pos());
  settings->beginGroup("Docks");
  settings->setValue("DockWindows", saveState());
  settings->endGroup();
  settings->endGroup();
}

void MioMain::connectSlots() {
  // connect(this->)
}

void MioMain::readSettings() {
  QSettings *settings = Configuration::getInstance().getSettings();
  settings->beginGroup("MainWindow");
  resize(settings->value("size", QSize(400, 400)).toSize());
  move(settings->value("pos", QPoint(200, 200)).toPoint());
  settings->endGroup();
}
void MioMain::on_actionQuit_triggered() { close(); }
