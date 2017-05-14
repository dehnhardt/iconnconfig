#include "retsetethernetportinfo.h"

RetSetEthernetPortInfo::RetSetEthernetPortInfo(Device *device)
    : SysExMessage(RET_SET_ETHERNET_PORT_INFO, QUERY, device) {}
