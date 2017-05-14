#include "retsetinfo.h"

RetSetInfo::RetSetInfo(Device *device)
    : RetSetInfo(SysExMessage::QUERY, device) {}

RetSetInfo::RetSetInfo(CommandFlags flags, Device *device)
    : SysExMessage(SysExMessage::RET_SET_INFO, flags, device) {}

SysExMessage::DeviceInfoItem RetSetInfo::getInfoItem() { return infoItem; }

std::string RetSetInfo::getValue() { return value; }

void RetSetInfo::parseAnswerData() {
  this->infoItem = (SysExMessage::DeviceInfoItem)data->at(0);
  this->value = std::string(data->begin() + 1, data->end());
}

bool RetSetInfo::isItemEditable() {
  switch (infoItem) {
  case DEVICE_NAME:
    return true;
  default:
    return false;
  }
}

std::string RetSetInfo::getItemName() {
  switch (infoItem) {
  case ACCESSORY_NAME:
    return tr("Accessory Name").toStdString();
  case MANUFACTURER_NAME:
    return tr("Manufacturer Name").toStdString();
  case MODEL_NUMBER:
    return tr("Model Number").toStdString();
  case SERIAL_NUMBER:
    return tr("Serial Numer").toStdString();
  case FIRMWARE_VERSION:
    return tr("Firmware Version").toStdString();
  case HARDWARE_VERSION:
    return tr("Hardware Version").toStdString();
  case DEVICE_NAME:
    return tr("Name").toStdString();
  default:
    return tr("unknown").toStdString();
  }
}
