#include "deviceinfowidget.h"
#include "../sysex/commands.h"
#include "ui_deviceinfowidget.h"

#include <QHeaderView>
#include <QListWidgetItem>
#include <QTableWidget>

DeviceInfoWidget::DeviceInfoWidget(MioMain *parent, Device *device,
                                   DeviceInfo *deviceInfo, QString windowTitle)
    : MultiInfoWidget(parent, device, windowTitle), deviceInfo(deviceInfo) {
  infoSections = std::vector<std::string>();
  if (device->getCommands()->isCommandSupported(SysExMessage::GET_INFO_LIST))
    infoSections.push_back("Global");
  if (device->getCommands()->isCommandSupported(
          SysExMessage::GET_ETHERNET_PORT_INFO))
    infoSections.push_back("Network");
}

DeviceInfoWidget::~DeviceInfoWidget() {}

QWidget *DeviceInfoWidget::createWidget(std::string infoName) {
  if (infoName == "Global") {
    QWidget *w = new QWidget(this->parentWidget());
    QGridLayout *lo = new QGridLayout();
    QPalette qp;

    w->setLayout(lo);
    if (this->deviceInfo) {
      std::vector<InfoItem> *infoItems = this->deviceInfo->getDeviceInfos();
      QTableWidget *tw = new QTableWidget(infoItems->size(), 2, this);
      tw->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Name")));
      tw->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Value")));
      tw->verticalHeader()->hide();
      tw->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
      for (unsigned int i = 0; i < infoItems->size(); i++) {
        InfoItem infoItem = infoItems->at(i);
        QTableWidgetItem *name = new QTableWidgetItem(infoItem.name.c_str());
        name->setForeground(qp.dark());
        QTableWidgetItem *value = new QTableWidgetItem(infoItem.value.c_str());
        name->setFlags(name->flags() & ~Qt::ItemIsEditable);
        if (!infoItem.editable) {
          value->setFlags(value->flags() & ~Qt::ItemIsEditable);
          value->setForeground(qp.dark());
        }
        tw->setItem(i, 0, name);
        tw->setItem(i, 1, value);
      }
      lo->addWidget(tw, 0, 0);
    }
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
