#ifndef PORTSETTINGWIDGET_H
#define PORTSETTINGWIDGET_H

#include "../device.h"

#include <QDockWidget>

namespace Ui {
class PortSettingWidget;
}

class PortSettingWidget : public QDockWidget {
  Q_OBJECT

public:
  explicit PortSettingWidget(QWidget *parent = 0, Device *device = nullptr);
  ~PortSettingWidget();

private:
  Ui::PortSettingWidget *ui;
  Device *device;
};

#endif // PORTSETTINGWIDGET_H
