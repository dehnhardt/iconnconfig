#include "retinfolist.h"

RetInfoList::RetInfoList(Device *device)
	: SysExMessage(Command::RET_INFO_LIST, SysExMessage::QUERY, device) {}

void RetInfoList::parseAnswerData() {
	implementedInfos = new std::vector<DeviceInfoItem>();
	unsigned int nInfosSize = static_cast<unsigned int>(data->size());
	DeviceInfoItem info;
	for (unsigned int i = 0; i < nInfosSize; ++i) {
		info = static_cast<DeviceInfoItem>(data->at(i));
		if (info != 0)
			implementedInfos->push_back(info);
		if (info == 16)
			++i;
	}
}

bool RetInfoList::isInfoImplemented(SysExMessage::DeviceInfoItem info) {
	if (!implementedInfos)
		return false;
	std::vector<DeviceInfoItem>::iterator it;
	for (it = implementedInfos->begin(); it != implementedInfos->end(); it++) {
		if (*it == info)
			return true;
	}
	return false;
}
