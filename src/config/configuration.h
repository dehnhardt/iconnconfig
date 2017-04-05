#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "../device.h"
#include <QSettings>
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
  Configuration() { settings = new QSettings(); }
  ~Configuration() {
    delete settings;
    if (devices != 0)
      delete devices;
  }

public:
  Configuration(Configuration const &) = delete;
  void operator=(Configuration const &) = delete;
  // end singleton

public:
  // getter
  Devices *getDevices() {
    if (devices == 0)
      devices = new std::map<long, Device *>;
    return devices;
  }
  QSettings *getSettings() { return settings; }

  long getDefaultDevice() {
    return settings->value("/devices/defaultDevice/serialNumber", 0).toInt();
  }

  bool getUsbDeviceDetection() { return enableUsbDetection; }
  bool getMidiDeviceDetection() { return enableMidiDeviceDetection; }

  // setter
  void setDevices(std::map<long, Device *> *devices) {
    this->devices = devices;
  }
  void setDefaultDevice(int serialNumber) {
    settings->setValue("/devices/defaultDevice/serialNumber", serialNumber);
  }

private:
  std::map<long, Device *> *devices;
  bool enableUsbDetection = false;
  bool enableMidiDeviceDetection = true;
  QSettings *settings = 0;
};

#endif // CONFIGURATION_H
