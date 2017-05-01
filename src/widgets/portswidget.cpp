#include "portswidget.h"

PortsWidget::PortsWidget(QWidget *parent, Device *device, QString windowTitle)
    : MultiInfoWidget(parent, device, windowTitle) {
  infoSections = std::vector<std::string>{"DIN", "USB-Host", "ETH"};
}

PortsWidget::~PortsWidget() {}
