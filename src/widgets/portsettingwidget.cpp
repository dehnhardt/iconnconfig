#include "portsettingwidget.h"
#include "../device.h"
#include "ui_portsettingwidget.h"

PortSettingWidget::PortSettingWidget(MioMain *parent, Device *device,
                                     QString windowTitle)
    : MultiInfoWidget(parent, device, windowTitle) {
  infoSections =
      std::vector<std::string>{"DIN", "USB-Host", "USB-Blala", "ETH"};
}

PortSettingWidget::~PortSettingWidget() {}
