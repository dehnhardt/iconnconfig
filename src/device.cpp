#include "device.h"
#include "sysex/commands.h"
#include "sysex/midi.h"

#include <array>
#include <cstring>
#include <iostream>
#include <unistd.h>

Device::Device(int inPortNumber, int outPortNumber, long serialNumber,
               int productId) {
  this->inPortNumber = inPortNumber;
  this->outPortNumber = outPortNumber;
  this->serialNumber = new MIDISysexValue(serialNumber, 5);
  this->productId = new MIDISysexValue(productId, 2);
}

#ifdef __MIO_SIMULATE__
Device::Device(int inPortNumber, int outPortNumber, long serialNumber,
               int productId, std::string modelName, std::string deviceName) {
  this->inPortNumber = inPortNumber;
  this->outPortNumber = outPortNumber;
  this->serialNumber = new MIDISysexValue(serialNumber);
  this->productId = new MIDISysexValue(productId);
  this->modelName = modelName;
  this->deviceName = deviceName;
}
#endif //__MIO_DEBUG__

BYTE_VECTOR *Device::getManufacturerHeader() {
  BYTE_VECTOR *mfh = new BYTE_VECTOR();
  mfh->push_back(MANUFACTURER_SYSEX_ID[0]);
  mfh->push_back(MANUFACTURER_SYSEX_ID[1]);
  mfh->push_back(MANUFACTURER_SYSEX_ID[2]);
  return mfh;
}

BYTE_VECTOR *Device::getDeviceHeader() {
  if (deviceHeader == 0) {
    deviceHeader = new BYTE_VECTOR();
    deviceHeader->reserve(productId->getByteValue()->size() +
                          serialNumber->getByteValue()->size());
    deviceHeader->insert(deviceHeader->end(),
                         productId->getByteValue()->begin(),
                         productId->getByteValue()->end());
    deviceHeader->insert(deviceHeader->end(),
                         serialNumber->getByteValue()->begin(),
                         serialNumber->getByteValue()->end());
  }
  return deviceHeader;
}

BYTE_VECTOR *Device::getFullHeader() {
  if (fullHeader == 0) {
    fullHeader = new BYTE_VECTOR();
    fullHeader->reserve(Device::getManufacturerHeader()->size() +
                        getDeviceHeader()->size());
    fullHeader->insert(fullHeader->end(),
                       Device::getManufacturerHeader()->begin(),
                       Device::getManufacturerHeader()->end());
    fullHeader->insert(fullHeader->end(), getDeviceHeader()->begin(),
                       getDeviceHeader()->end());
  }
  return fullHeader;
}

void Device::queryDeviceInfo() {
  Commands *c = new Commands(getDeviceHeader());
  c->getMIDISysExMessage();
}
