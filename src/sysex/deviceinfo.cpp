#include "deviceinfo.h"

DeviceInfo::DeviceInfo(Device *device)
    : SysExMessage(SysExMessage::GET_DEVICE_INFO, SysExMessage::QUERY, device) {
  deviceInfos = new std::vector<InfoItem>();
  mappedInfos = new std::map<DeviceInfoItem, InfoItem>();
}

DeviceInfo::DeviceInfo(Device *device, ImplementedInfos *infoList)
    : SysExMessage(SysExMessage::GET_DEVICE_INFO, SysExMessage::QUERY, device),
      infoList(infoList) {
  deviceInfos = new std::vector<InfoItem>();
  mappedInfos = new std::map<DeviceInfoItem, InfoItem>();
  std::vector<DeviceInfoItem> *implementedInfos =
      infoList->getImplementedInfos();
  for (std::vector<DeviceInfoItem>::iterator it = implementedInfos->begin();
       it < implementedInfos->end(); ++it) {
    this->setInfoItem(*it);
    execute();
    InfoItem i;
    i.name = getItemName(*it);
    i.infoItem = *it;
    i.value = getDataAsString();
    i.editable = isItemEditable(*it);
    deviceInfos->push_back(i);
    (*mappedInfos)[*it] = i;
  }
}

DeviceInfo::~DeviceInfo() {
  delete deviceInfos;
  delete mappedInfos;
}

BYTE_VECTOR *DeviceInfo::getMessageData() {
  BYTE_VECTOR *messageData = new BYTE_VECTOR();
  messageData->push_back(this->infoItem);
  return messageData;
}

std::string DeviceInfo::getDataAsString() {
  if (data->size() > 0) {
    std::string result(data->begin() + 1, data->end());
    return result;
  } else {
    return std::string();
  }
}

DeviceInfo::DeviceInfoItem DeviceInfo::getDeviceInfoItem() {
  return (DeviceInfoItem)(*data)[0];
}

bool DeviceInfo::isItemEditable(SysExMessage::DeviceInfoItem item) {
  switch (item) {
  case DEVICE_NAME:
    return true;
  default:
    return false;
  }
}

std::string DeviceInfo::getItemName(SysExMessage::DeviceInfoItem item) {
  switch (item) {
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

std::string DeviceInfo::getItemValue(SysExMessage::DeviceInfoItem item) {
  InfoItem i = (*mappedInfos)[item];
  return i.value;
}

std::vector<InfoItem> *DeviceInfo::getDeviceInfos() { return deviceInfos; }

void DeviceInfo::deviceInfoChanged(SysExMessage::DeviceInfoItem item,
																	 std::string value) {
	std::cout << "DeviceInfo: Item " << item
						<< " in DeviceInfoTable changed to value " << value << std::endl;
}
