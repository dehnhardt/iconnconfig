#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "../device.h"
#include <map>

typedef std::map<long, Device *> Devices;

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
  // getter
  Devices *getDevices();
  // std::map<long, Device *> *getDevices();
  bool getUsbDeviceDetection() { return enableUsbDetection; }
  bool getMidiDeviceDetection() { return enableMidiDeviceDetection; }
  // setter
  void setDevices(std::map<long, Device *> *devices) {
    this->devices = devices;
  }

private:
  std::map<long, Device *> *devices;
  bool enableUsbDetection = false;
  bool enableMidiDeviceDetection = true;
};

#endif // CONFIGURATION_H
