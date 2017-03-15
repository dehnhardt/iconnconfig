#ifndef DEVICE_H
#define DEVICE_H

#include "sysex/midi.h"

#include <vector>

class Device {
public:
  Device(int inPortNumber, int outPortNumber, long serialNumber, int productId);

public:
  static const long MANUFACTURER_USB_ID = 0x2321;
  static constexpr char MANUFACTURER_SYSEX_ID[3] = {0x00, 0x01, 0x73};
  static BYTE_VECTOR *getManufacturerHeader();
  BYTE_VECTOR *getDeviceHeader();
  BYTE_VECTOR *getFullHeader();

private:
  int inPortNumber;
  int outPortNumber;
  MIDISysexValue *serialNumber;
  MIDISysexValue *productId;
  BYTE_VECTOR *deviceHeader = 0;
  BYTE_VECTOR *fullHeader = 0;
};

#endif // DEVICE_H
