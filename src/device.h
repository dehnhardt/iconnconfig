#ifndef DEVICE_H
#define DEVICE_H

#include "sysex/midi.h"

#include <string>
#include <vector>

class Device {
public:
  Device(int inPortNumber, int outPortNumber, long serialNumber, int productId);
#ifdef __MIO_SIMULATE__
  Device(int inPortNumber, int outPortNumber, long serialNumber, int productId,
         std::string modelName, std::string deviceName);
#endif //__MIO_DEBUG__

public:
  static const long MANUFACTURER_USB_ID = 0x2321;
  static const int MESSAGE_CLASS = 0x7e;
  static constexpr char MANUFACTURER_SYSEX_ID[3] = {0x00, 0x01, 0x73};

  // getter
  static BYTE_VECTOR *getManufacturerHeader();
  BYTE_VECTOR *getDeviceHeader();
  BYTE_VECTOR *getFullHeader();
  std::string getModelName() { return modelName; }
  std::string getDeviceName() { return deviceName; }
  MIDISysexValue *getSerialNumber() { return serialNumber; }
  MIDISysexValue *getProductId() { return productId; }

  // setter
  void setDeviceInformation(std::string modelName, std::string deviceName) {
    this->modelName = modelName;
    this->deviceName = deviceName;
  }

private:
  int inPortNumber;
  int outPortNumber;
  MIDISysexValue *serialNumber;
  MIDISysexValue *productId;

  std::string modelName;
  std::string deviceName;

  BYTE_VECTOR *deviceHeader = 0;
  BYTE_VECTOR *fullHeader = 0;
};

#endif // DEVICE_H
