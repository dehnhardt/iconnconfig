#ifndef PORTSETTINGWIDGET_H
#define PORTSETTINGWIDGET_H

#include "../device.h"
#include "multiinfowidget.h"

class PortSettingWidget : public MultiInfoWidget {

public:
  explicit PortSettingWidget(MioMain *parent = 0, Device *device = nullptr, QString windowTitle = "PortSettings");
  ~PortSettingWidget();
};

#endif // PORTSETTINGWIDGET_H
