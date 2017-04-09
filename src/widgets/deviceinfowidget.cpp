#include "deviceinfowidget.h"
#include "ui_deviceinfowidget.h"

DeviceInfoWidget::DeviceInfoWidget(QWidget *parent, Device *device)
    : QDockWidget(parent), ui(new Ui::DeviceInfoWidget), device(device) {
  ui->setupUi(this);
  QTableWidgetItem *item_0_0 = new QTableWidgetItem("Name");
  ui->deciceInfoTable->setItem(0, 0, item_0_0);
  QTableWidgetItem *item_0_1 =
      new QTableWidgetItem(QString::fromStdString(device->getDeviceName()));
  ui->deciceInfoTable->setItem(0, 1, item_0_1);
  QTableWidgetItem *item_1_0 = new QTableWidgetItem("Model");
  ui->deciceInfoTable->setItem(1, 0, item_1_0);
  QTableWidgetItem *item_1_1 =
      new QTableWidgetItem(QString::fromStdString(device->getModelName()));
  ui->deciceInfoTable->setItem(1, 1, item_1_1);
}

DeviceInfoWidget::~DeviceInfoWidget() { delete ui; }
