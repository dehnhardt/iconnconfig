#include "portsettingwidget.h"
#include "../device.h"
#include "ui_portsettingwidget.h"

PortSettingWidget::PortSettingWidget(QWidget *parent, Device *device)
    : QDockWidget(parent), ui(new Ui::PortSettingWidget) {
  ui->setupUi(this);
}

PortSettingWidget::~PortSettingWidget() { delete ui; }
