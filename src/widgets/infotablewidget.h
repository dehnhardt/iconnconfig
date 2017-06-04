#ifndef INFOTABLEWIDGET_H
#define INFOTABLEWIDGET_H

#include "../sysex/getinfo.h"
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

public slots:
  void onDeviceInfoChanged(int row, int column);

signals:
  void deviceInfoChanged(SysExMessage::DeviceInfoItem item, std::string value);

private:
  std::map<SysExMessage::DeviceInfoItem, RetSetInfo *> *retSetInfos = 0;
  QTableWidget *tw = 0;

private:
  void setupTable();
  void setUpTableItems();
};

#endif // INFOTABLEWIDGET_H
