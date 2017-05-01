#include "deviceinfowidget.h"
#include "../sysex/commands.h"
#include "ui_deviceinfowidget.h"

#include <QListWidgetItem>

DeviceInfoWidget::DeviceInfoWidget(MioMain *parent, Device *device,
                                   QString windowTitle)
    : MultiInfoWidget(parent, device, windowTitle) {
	infoSections = std::vector<std::string>();
	if (device->getCommands()->isCommandSupported(SysExMessage::GET_INFO_LIST))
		infoSections.push_back("Global");
	if (device->getCommands()->isCommandSupported(
					SysExMessage::GET_ETHERNET_PORT_INFO))
		infoSections.push_back("Network");
}

DeviceInfoWidget::~DeviceInfoWidget() {}

/*
QWidget *DeviceInfoWidget::createWidget(std::string infoName) {
  QWidget *w = new QWidget(this->parentWidget());
  QPalette pal;
  pal.setColor(QPalette::Background, Qt::yellow);
  w->setPalette(pal);
  QGridLayout *lo = new QGridLayout();
  w->setLayout(lo);
  QLabel *l = new QLabel(w);
  l->setText(QString::fromStdString(infoName));
  lo->addWidget(l, 0, 0);
  return w;
}
*/
