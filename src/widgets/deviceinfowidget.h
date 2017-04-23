#ifndef DEVICEINFOWIDGET_H
#define DEVICEINFOWIDGET_H

#include "../device.h"
#include "multiinfo.h"

#include <QDockWidget>

namespace Ui {
class DeviceInfoWidget;
}

class DeviceInfoWidget : public QDockWidget, MultiInfo {
  Q_OBJECT

public:
  explicit DeviceInfoWidget(QWidget *parent = 0, Device *device = 0);
  ~DeviceInfoWidget();

private slots:
  void on_infoList_currentRowChanged(int currentRow);

private:
  Ui::DeviceInfoWidget *ui;
  Device *device;

signals:
  void infoChanged(int row);
};

#endif // DEVICEINFOWIDGET_H
