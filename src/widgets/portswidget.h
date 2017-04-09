#ifndef PORTSWIDGET_H
#define PORTSWIDGET_H

#include "../device.h"

#include <QDockWidget>

namespace Ui {
class PortsWidget;
}

class PortsWidget : public QDockWidget {
  Q_OBJECT

public:
  explicit PortsWidget(QWidget *parent = 0, Device *device = 0);
  ~PortsWidget();

private:
  Ui::PortsWidget *ui;
  Device *device;
};

#endif // PORTSWIDGET_H
