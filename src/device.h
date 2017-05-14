#ifndef DEVICE_H
#define DEVICE_H

#include "RtMidi.h"
#include "sysex/midi.h"

#include <map>
#include <string>
#include <vector>

class SysExMessage;
class RetCommandList;
class ImplementedInfos;
class GetInfo;

class Device {
public:
  Device(int inPortNumber, int outPortNumber, long serialNumber, int productId);
  ~Device();

public:
  static const long MANUFACTURER_USB_ID = 0x2321;
  static const char MESSAGE_CLASS = 0x7e;
  static constexpr char MANUFACTURER_SYSEX_ID[3] = {0x00, 0x01, 0x73};
  static BYTE_VECTOR *manufacturerHeader;

  static const int DATA_LENGTH_OFFSET = 16;
  static const int DATA_LENGTH_LENGTH = 2;
  static const int DATA_OFFSET = 18;

  void sentSysex(BYTE_VECTOR *data);
  BYTE_VECTOR *retrieveSysex();
  BYTE_VECTOR *nextTransactionId();

  // getter
  static BYTE_VECTOR *getManufacturerHeader();
  BYTE_VECTOR *getDeviceHeader();
  BYTE_VECTOR *getFullHeader();
  bool queryDeviceInfo();

  std::string getModelName() { return modelName; }
  std::string getDeviceName() { return deviceName; }
  std::string getSerialNumberString() { return serialNumberString; }
  std::string getManufacturerName() { return manufacturerName; }
  std::string getFirmwareVersion() { return firmwareVersion; }
  std::string getHardwareVersion() { return hardwareVersion; }
  std::string getModelNumber() { return modelNumber; }
  int getInPortNumer() { return inPortNumber; }
  int getOutPortNumer() { return outPortNumber; }

  MIDISysexValue *getSerialNumber() { return serialNumber; }
  MIDISysexValue *getProductId() { return productId; }
  bool getDefault() { return isDefault; }
  RetCommandList *getCommands() { return commands; }
  GetInfo *getDeviceInfo() { return deviceInfo; }

#ifdef __MIO_SIMULATE__
  Device(int inPortNumber, int outPortNumber, long serialNumber, int productId,
         std::string modelName, std::string deviceName);
  bool getSimulate() { return deviceIsSimulated; }
  RetCommandList *simulateCommands();
#endif

  // setter
  void setDeviceInformation(std::string modelName, std::string deviceName) {
    this->modelName = modelName;
    this->deviceName = deviceName;
  }
  void setDefault(bool isDefault) { this->isDefault = isDefault; }
#ifdef __MIO_SIMULATE__
  void setSimulate() { deviceIsSimulated = true; }
#endif

private:
#ifdef __MIO_SIMULATE__
  bool deviceIsSimulated = false;
#endif
  int inPortNumber;
  int outPortNumber;

  int transactionId = 0;

  bool isDefault = false;

  RtMidiIn *midiin = 0;
  RtMidiOut *midiout = 0;

  MIDISysexValue *serialNumber;
  MIDISysexValue *productId;

  std::string modelName;
  std::string deviceName;
  std::string serialNumberString;
  std::string manufacturerName;
  std::string firmwareVersion;
  std::string hardwareVersion;
  std::string modelNumber;

  RetCommandList *commands = 0;
  ImplementedInfos *ii = 0;
  GetInfo *deviceInfo = 0;

  BYTE_VECTOR *deviceHeader = 0;
  BYTE_VECTOR *fullHeader = 0;

  void setupMidi();
  bool checkSysex(BYTE_VECTOR *data);
};

#endif // DEVICE_H
