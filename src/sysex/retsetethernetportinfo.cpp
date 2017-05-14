#include "retsetethernetportinfo.h"

RetSetEthernetPortInfo::RetSetEthernetPortInfo(Device *device)
    : SysExMessage(RET_SET_ETHERNET_PORT_INFO, QUERY, device) {}

void RetSetEthernetPortInfo::parseAnswerData() {
	std::cout << "parseAnswerData" << std::endl;
	unsigned char commandVersionNumber = data->at(0);
	BYTE_VECTOR *portId = new BYTE_VECTOR(data->begin() + 1, data->begin() + 3);
	unsigned int method = data->at(3);

	extractIp(4);
	extractIp(9);
	extractIp(14);
	extractIp(19);
	extractIp(24);
	extractIp(29);
}

void RetSetEthernetPortInfo::extractIp(int offset) {

	long address = MIDI::byteJoin(data, offset, 5);
	int a1 = address & 255;
	address >>= 8;
	int a2 = address & 255;
	address >>= 8;
	int a3 = address & 255;
	address >>= 8;
	int a4 = address & 255;
	std::cout << std::dec << a4 << "." << a3 << "." << a2 << "." << a1
						<< std::endl;
}
