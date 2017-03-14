#include "miomain.h"
#include "devicedetection.h"
#include "sysex/midi.h"
#include "ui_miomain.h"

#include <QTimer>
#include <QtDebug>

MioMain::MioMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::MioMain) {
  ui->setupUi(this);
  QTimer::singleShot(100, this, SLOT(openDetectionWindow()));
}

MioMain::~MioMain() {
  delete deviceDetectionWindow;
  delete ui;
}

void MioMain::openDetectionWindow() {
  deviceDetectionWindow = new DeviceDetection(this);
  deviceDetectionWindow->exec();
}

void MioMain::on_actionQuit_triggered() { close(); }
