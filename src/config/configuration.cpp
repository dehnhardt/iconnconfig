#include "configuration.h"

std::map<long, Device *> *Configuration::getDevices() {
  if (devices == 0)
    devices = new std::map<long, Device *>;
  return devices;
}
