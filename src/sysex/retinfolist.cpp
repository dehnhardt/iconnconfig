#include "retinfolist.h"

RetInfoList::RetInfoList(Device *device)
	: SysExMessage(Command::RET_INFO_LIST, SysExMessage::QUERY, device) {}

void RetInfoList::parseAnswerData() {
	m_pImplementedInfos = new std::vector<DeviceInfoItem>();
	unsigned int nInfosSize = static_cast<unsigned int>(m_pData->size());
	DeviceInfoItem info;
	for (unsigned int i = 0; i < nInfosSize; ++i) {
		info = static_cast<DeviceInfoItem>(m_pData->at(i));
		if (info != 0)
			m_pImplementedInfos->push_back(info);
		if (info == 16)
			++i;
	}
}

bool RetInfoList::isInfoImplemented(SysExMessage::DeviceInfoItem info) {
	if (!m_pImplementedInfos)
		return false;
	std::vector<DeviceInfoItem>::iterator it;
	for (it = m_pImplementedInfos->begin(); it != m_pImplementedInfos->end(); it++) {
		if (*it == info)
			return true;
	}
	return false;
}
