#include "devicewidget.h"
#include "ui_devicewidget.h"

DeviceWidget::DeviceWidget(QWidget *parent, Device *device)
    : QDockWidget(parent), ui(new Ui::DeviceWidget), device(device) {
  ui->setupUi(this);
  QString n = QString(device->getDeviceName().c_str());
  this->setWindowTitle(n);
}

DeviceWidget::~DeviceWidget() { delete ui; }

void DeviceWidget::infoTypeChanged(int type) {}
