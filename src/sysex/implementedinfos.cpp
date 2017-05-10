#include "implementedinfos.h"

ImplementedInfos::ImplementedInfos(Device *device)
		: SysExMessage(SysExMessage::RET_INFO_LIST, SysExMessage::QUERY, device) {}

void ImplementedInfos::parseAnswerData() {
	implementedInfos = new std::vector<DeviceInfoItem>();
	unsigned int nInfosSize = data->size();
	DeviceInfoItem info;
	for (unsigned int i = 0; i < nInfosSize; i++) {
		info = (DeviceInfoItem)data->at(i);
		if (info != 0)
			implementedInfos->push_back(info);
	}
}

bool ImplementedInfos::isInfoImplemented(SysExMessage::DeviceInfoItem info) {
	if (!implementedInfos)
		return false;
	std::vector<DeviceInfoItem>::iterator it;
	for (it = implementedInfos->begin(); it != implementedInfos->end(); it++) {
		if (*it == info)
			return true;
	}
	return false;
}
