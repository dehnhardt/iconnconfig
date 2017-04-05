#include "miomain.h"
#include "config/configuration.h"
#include "devicedetection.h"
#include "sysex/midi.h"
#include "ui_miomain.h"

#include <QCloseEvent>
#include <QTimer>
#include <QtDebug>

MioMain::MioMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::MioMain) {
  ui->setupUi(this);
  readSettings();
  QTimer::singleShot(100, this, SLOT(openDetectionWindow()));
}

MioMain::~MioMain() {
  delete deviceDetectionWindow;
  delete ui;
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
  settings->endGroup();
}

void MioMain::readSettings() {
  QSettings *settings = Configuration::getInstance().getSettings();
  settings->beginGroup("MainWindow");
  resize(settings->value("size", QSize(400, 400)).toSize());
  move(settings->value("pos", QPoint(200, 200)).toPoint());
  settings->endGroup();
}
void MioMain::on_actionQuit_triggered() { close(); }
