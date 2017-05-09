#include "deviceinfowidget.h"
#include "../sysex/commands.h"
#include "ui_deviceinfowidget.h"

#include <QListWidgetItem>
#include <QTableWidget>

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

QWidget *DeviceInfoWidget::createWidget(std::string infoName) {
	if (infoName == "Global") {
		QWidget *w = new QWidget(this->parentWidget());
		QGridLayout *lo = new QGridLayout();
		w->setLayout(lo);
		QTableWidget *tw = new QTableWidget(10, 2, this);
		tw->setItem(0, 0, new QTableWidgetItem("Name"));
		tw->setItem(0, 1,
								new QTableWidgetItem(this->device->getDeviceName().c_str()));
		tw->setItem(1, 0, new QTableWidgetItem("Model"));
		tw->setItem(1, 1,
								new QTableWidgetItem(this->device->getModelName().c_str()));
		tw->setItem(2, 0, new QTableWidgetItem("SerialNumber"));
		tw->setItem(2, 1, new QTableWidgetItem(
													this->device->getSerialNumberString().c_str()));
		tw->setItem(3, 0, new QTableWidgetItem("Manufacturer"));
		tw->setItem(3, 1, new QTableWidgetItem(
													this->device->getManufacturerName().c_str()));
		tw->setItem(4, 0, new QTableWidgetItem("Firmware Version"));
		tw->setItem(
				4, 1, new QTableWidgetItem(this->device->getFirmwareVersion().c_str()));
		tw->setItem(5, 0, new QTableWidgetItem("Hardware Version"));
		tw->setItem(
				5, 1, new QTableWidgetItem(this->device->getHardwareVersion().c_str()));
		tw->setItem(6, 0, new QTableWidgetItem("Model Number"));
		tw->setItem(6, 1,
								new QTableWidgetItem(this->device->getModelNumber().c_str()));

		lo->addWidget(tw, 0, 0);
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
