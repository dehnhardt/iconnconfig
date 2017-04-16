#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include "../device.h"

#include <QDockWidget>

namespace Ui {
class DeviceWidget;
}

class DeviceWidget : public QDockWidget {
  Q_OBJECT

public:
  explicit DeviceWidget(QWidget *parent = 0, Device *device = nullptr);
  ~DeviceWidget();

public slots:
  void infoTypeChanged(int type);

private:
  Ui::DeviceWidget *ui;
  Device *device = 0;
};

#endif // DEVICEWIDGET_H
