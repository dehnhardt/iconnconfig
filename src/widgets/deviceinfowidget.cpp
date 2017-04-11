#include "deviceinfowidget.h"
#include "ui_deviceinfowidget.h"

DeviceInfoWidget::DeviceInfoWidget(QWidget *parent, Device *device)
    : QDockWidget(parent), ui(new Ui::DeviceInfoWidget), device(device) {
  ui->setupUi(this);
  QWidget *lTitleBar = titleBarWidget();
  setTitleBarWidget(new QWidget());
  delete lTitleBar;
  ui->infoList->addItem(tr("Global"));
  ui->infoList->addItem(tr("Networking"));
}

DeviceInfoWidget::~DeviceInfoWidget() { delete ui; }
