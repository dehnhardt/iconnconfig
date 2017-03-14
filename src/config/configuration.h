#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "../device.h"
#include <map>

class Configuration {

  // start singleton
public:
  static Configuration &getInstance() {
    static Configuration instance;
    return instance;
  }

private:
  Configuration() {}

public:
  Configuration(Configuration const &) = delete;
  void operator=(Configuration const &) = delete;
  // end singleton

public:
  std::map<long, Device *> *getDevices();
  bool getUsbDeviceDetection() { return enableUsbDetection; }
  bool getMidiDeviceDetection() { return enableMidiDeviceDetection; }

private:
  std::map<long, Device *> *devices;
  bool enableUsbDetection = false;
  bool enableMidiDeviceDetection = true;
};

#endif // CONFIGURATION_H
