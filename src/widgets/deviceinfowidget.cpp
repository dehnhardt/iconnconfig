#include "deviceinfowidget.h"
#include "../sysex/retcommandlist.h"
#include "infotablewidget.h"
#include "ui_deviceinfowidget.h"

#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QListWidgetItem>
#include <QTableWidget>

DeviceInfoWidget::DeviceInfoWidget(MioMain *parent, Device *device,
                                   GetInfo *deviceInfo, QString windowTitle)
    : MultiInfoWidget(parent, device, windowTitle), deviceInfo(deviceInfo) {
  infoSections = std::vector<std::string>();
  if (device->getCommands()->isCommandSupported(SysExMessage::GET_INFO_LIST))
    infoSections.push_back("Global");
  if (device->getCommands()->isCommandSupported(
          SysExMessage::GET_ETHERNET_PORT_INFO))
    infoSections.push_back("Network");
}

DeviceInfoWidget::~DeviceInfoWidget() {}

void DeviceInfoWidget::deviceInfoChanged(SysExMessage::DeviceInfoItem item,
                                         std::string value) {
  std::cout << "DeviceInfoWidget: deviceInfoChanged " << item << " value "
            << value << std::endl;
  this->deviceInfo->deviceInfoChanged(item, value);
}

QWidget *DeviceInfoWidget::createWidget(std::string infoName) {
  if (infoName == "Global") {
    InfoTableWidget *w = new InfoTableWidget(
        this->parentWidget(), this->deviceInfo->getRetSetInfos());
    connect(w, &InfoTableWidget::deviceInfoChanged, this,
            &DeviceInfoWidget::deviceInfoChanged);
    return w;
  } else {
    QWidget *w = new QWidget(this->parentWidget());
    QGridLayout *lo = new QGridLayout();
    w->setLayout(lo);
    QLabel *l = new QLabel(w);
    l->setText(QString::fromStdString(infoName));
    lo->addWidget(l, 0, 0);
    return w;
  }
}
