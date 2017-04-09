#include "portswidget.h"
#include "ui_portswidget.h"

PortsWidget::PortsWidget(QWidget *parent, Device *device)
    : QDockWidget(parent), ui(new Ui::PortsWidget), device(device) {
  ui->setupUi(this);
}

PortsWidget::~PortsWidget() { delete ui; }
