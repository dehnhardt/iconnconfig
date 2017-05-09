#include "implementedinfos.h"

ImplementedInfos::ImplementedInfos(Device *device)
		: SysExMessage(SysExMessage::RET_INFO_LIST, SysExMessage::QUERY, device) {}

void ImplementedInfos::parseAnswerData() {
	supportedInfos = new std::vector<DeviceInfoItem>();
	unsigned int nInfosSize = data->size();
	DeviceInfoItem info;
	for (unsigned int i = 0; i < nInfosSize; i++) {
		info = (DeviceInfoItem)data->at(i);
		if (info != 0)
			supportedInfos->push_back(info);
	}
}

bool ImplementedInfos::isInfoSupported(SysExMessage::DeviceInfoItem info) {
	if (!supportedInfos)
		return false;
	std::vector<DeviceInfoItem>::iterator it;
	for (it = supportedInfos->begin(); it != supportedInfos->end(); it++) {
		if (*it == info)
			return true;
	}
	return false;
}
