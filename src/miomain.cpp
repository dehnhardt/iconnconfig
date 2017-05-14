#include "miomain.h"
#include "config/configuration.h"
#include "devicedetection.h"
#include "sysex/retcommandlist.h"
#include "sysex/midi.h"
#include "ui_miomain.h"
#include "widgets/centralwidget.h"
#include "widgets/deviceinfowidget.h"
#include "widgets/multiinfowidget.h"
#include "widgets/portswidget.h"

#include <QCloseEvent>
#include <QDesktopWidget>
#include <QSignalMapper>
#include <QStyle>
#include <QTimer>
#include <QtDebug>

MioMain::MioMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::MioMain) {
  ui->setupUi(this);
  setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::ForceTabbedDocks |
                 QMainWindow::VerticalTabs);
  readSettings();
  if (readDevicesFromSettings())
    openDefaultDevice();
  else
    QTimer::singleShot(100, this, SLOT(openDetectionWindow()));
}

MioMain::~MioMain() {
  if (deviceDetectionWindow)
    delete deviceDetectionWindow;
  delete ui;
}

void MioMain::openDefaultDevice() {
  writeDevicesToSettings();
  long defaultDeviceSN = Configuration::getInstance().getDefaultDevice();
  Device *d = Configuration::getInstance().getDevices()->at(defaultDeviceSN);
  addDevicesToSelectionMenu(defaultDeviceSN);
  openDeviceGUI(d);
}

void MioMain::addDevicesToSelectionMenu(long defaultDeviceSN) {
  QSignalMapper *signalMapper = new QSignalMapper();
  Devices *devices = Configuration::getInstance().getDevices();
  QActionGroup *devicesGroup = new QActionGroup(this);
  devicesGroup->setExclusive(true);
  for (Devices::iterator it = devices->begin(); it != devices->end(); ++it) {
    Device *d = it->second;
    QAction *a =
        ui->menuSelect->addAction(QString::fromStdString(d->getDeviceName()));
    a->setCheckable(true);
    devicesGroup->addAction(a);
    connect(a, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(a, new DeviceMenuMapper(d));
    if (it->first == defaultDeviceSN)
      a->setChecked(true);
  }
  connect(signalMapper, SIGNAL(mapped(QObject *)), this,
          SLOT(openDeviceGUI(QObject *)));
}

void MioMain::openDeviceGUI(QObject *o) {
  DeviceMenuMapper *m = (DeviceMenuMapper *)o;
#ifdef __MIO_DEBUG__
  std::cout << "open device GUI: " << m->device->getDeviceName() << std::endl;
#endif //__MIO_DEBUG__
  openDeviceGUI(m->device);
}

void MioMain::addDock(QDockWidget *dockWidget, Qt::DockWidgetArea area) {
  if (MultiInfoWidget *miw = dynamic_cast<MultiInfoWidget *>(dockWidget)) {
    miw->createInfoSections();
  }
  switch (area) {
  case Qt::NoDockWidgetArea:
    setCentralWidget(dockWidget);
    break;
  case Qt::LeftDockWidgetArea:
    this->addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
    break;
  default:
    break;
  }
  std::vector<QDockWidget *> v = dockWidgetAreas[area];
  if (v.size() > 0) {
    tabifyDockWidget(v[v.size() - 1], dockWidget);
  }
  dockWidgetAreas[area].push_back(dockWidget);
}

void MioMain::clearDocWidgets() {
  for (std::map<Qt::DockWidgetArea, std::vector<QDockWidget *>>::iterator it =
           dockWidgetAreas.begin();
       it != dockWidgetAreas.end(); ++it) {
    std::vector<QDockWidget *> v = it->second;
    for (unsigned int j = 0; j < v.size(); j++) {
      QWidget *w = v.at(j);
      delete w;
    }
    v.clear();
  }
  dockWidgetAreas.clear();
}

void MioMain::replacePanel(QWidget *w) {
  CentralWidget *cw = (CentralWidget *)centralWidget();
  cw->replacePanel(w);
}

void MioMain::openDeviceGUI(Device *d) {
  clearDocWidgets();
  RetCommandList *c = d->getCommands();
  if (c == 0) {
    // TODO throw error
    exit(2);
  }
  setWindowTitle(this->title + QString(": ") +
                 QString::fromStdString(d->getDeviceName()));
  CentralWidget *centralWidget = new CentralWidget(this, d);
  this->addDock(centralWidget);

  DeviceInfoWidget *deviceInfoWidget =
      new DeviceInfoWidget(this, d, d->getDeviceInfo());
  this->addDock(deviceInfoWidget, Qt::LeftDockWidgetArea);

  PortsWidget *portsWidget = new PortsWidget(this, d);
  this->addDock(portsWidget, Qt::LeftDockWidgetArea);
  if (c->isCommandSupported(SysExMessage::GET_ETHERNET_PORT_INFO)) {
    std::cout << "EthernetPortsAvailable";
  }

  QSettings *settings = Configuration::getInstance().getSettings();
  settings->beginGroup("MainWindow");
  restoreGeometry(settings->value("geometry").toByteArray());
  settings->endGroup();
  settings->beginGroup("Docks");
  // restoreState(settings->value("DockWindows").toByteArray());
  settings->endGroup();
  deviceInfoWidget->show();
  deviceInfoWidget->raise();
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
  settings->setValue("geometry", saveGeometry());
  settings->setValue("size", size());
  settings->setValue("pos", pos());
  settings->endGroup();
  settings->beginGroup("Docks");
  settings->setValue("DockWindows", saveState());
  settings->endGroup();
}

void MioMain::writeDevicesToSettings() {
  QSettings *settings = Configuration::getInstance().getSettings();
  Devices *devices = Configuration::getInstance().getDevices();
  settings->beginWriteArray("Devices");
  int i = 0;
  for (Devices::iterator it = devices->begin(); it != devices->end(); ++it) {
    settings->setArrayIndex(i);
    Device *d = it->second;
    settings->setValue("Device Name",
                       QString::fromStdString(d->getDeviceName()));
    settings->setValue("Serial Number",
                       (qlonglong)(d->getSerialNumber()->getLongValue()));
    settings->setValue("Input Port", d->getInPortNumer());
    settings->setValue("Output Port", d->getOutPortNumer());
    settings->setValue("Product Id",
                       (qlonglong)d->getProductId()->getLongValue());
#ifdef __MIO_SIMULATE__
    if (d->getSimulate()) {
      settings->setValue("Simulate", true);
      settings->setValue("Model Name",
                         QString::fromStdString(d->getModelName()));
    }
#endif
    ++i;
  }
  settings->endArray();
}

void MioMain::connectSlots() {
  // connect(this->)
}

void MioMain::readSettings() {
  this->title = windowTitle();
  QSettings *settings = Configuration::getInstance().getSettings();
  settings->beginGroup("MainWindow");
  resize(settings->value("size", QSize(400, 400)).toSize());
  move(settings->value("pos", QPoint(200, 200)).toPoint());
  settings->endGroup();
}

bool MioMain::readDevicesFromSettings() {
  Devices *devices = Configuration::getInstance().getDevices();
  devices->clear();
  QSettings *settings = Configuration::getInstance().getSettings();
  int size = settings->beginReadArray("Devices");
  if (size == 0)
    return false;
  for (int i = 0; i < size; ++i) {
    settings->setArrayIndex(i);
    int productId = settings->value("Product Id").toInt();
    long serialNumber =
        (qlonglong)settings->value("Serial Number").toLongLong();
    int inputPort = (qlonglong)settings->value("Input Port").toInt();
    int outputPort = (qlonglong)settings->value("Output Port").toInt();
#ifdef __MIO_SIMULATE__
    Device *device = 0;
    if (settings->value("Simulate").toBool()) {
      std::string modelName =
          settings->value("Model Name").toString().toStdString();
      std::string deviceName =
          settings->value("Device Name").toString().toStdString();
      device = new Device(inputPort, outputPort, serialNumber, productId,
                          modelName, deviceName);
    } else {
      device = new Device(inputPort, outputPort, serialNumber, productId);
    }
#else
    Device *device = new Device(inputPort, outputPort, serialNumber, productId);
#endif
    if (device->queryDeviceInfo())
      devices->insert(std::pair<long, Device *>(serialNumber, device));
  }
  settings->endArray();
  if (devices->size() == 0)
    return false;
  return true;
}
void MioMain::on_actionQuit_triggered() { close(); }
