#ifndef INFOTABLEWIDGET_H
#define INFOTABLEWIDGET_H

#include "../sysex/deviceinfo.h"
#include "deviceinfowidget.h"

#include <QObject>
#include <QTableWidget>
#include <QWidget>

class InfoTableWidget : public QWidget {
  Q_OBJECT
public:
  explicit InfoTableWidget(
      QWidget *parent,
      std::map<SysExMessage::DeviceInfoItem, RetSetInfo *> *retSetInfos);

signals:
  void deviceInfoChanged(SysExMessage::DeviceInfoItem item, std::string value);

public slots:
  void onDeviceInfoChanged(int row, int column);

private:
  std::map<SysExMessage::DeviceInfoItem, RetSetInfo *> *retSetInfos = 0;
  QTableWidget *tw = 0;
  void setupTable();
};

#endif // INFOTABLEWIDGET_H
