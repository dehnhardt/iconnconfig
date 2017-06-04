#include "portsettingwidget.h"
#include "../device.h"

PortSettingWidget::PortSettingWidget(MioMain *parent, Device *device,
                                     QString windowTitle)
    : MultiInfoWidget(parent, device, windowTitle) {
  infoSections = new std::vector<MultiInfoListEntry *>();
}

PortSettingWidget::~PortSettingWidget() {}
