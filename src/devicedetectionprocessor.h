#ifndef DEVICEDETECTIONPROCESSOR_H
#define DEVICEDETECTIONPROCESSOR_H

#include "RtMidi.h"
#include "device.h"
#include "libusb.h"

#include <QEvent>

class DeviceDetectionProcessor {
public:
  DeviceDetectionProcessor();
  ~DeviceDetectionProcessor();
  int getMidiInPortCount();
  int getMddiOutPortCount();
  void startDeviceDetection();

private:
  // Members
  RtMidiIn *midiin = 0;
  RtMidiOut *midiout = 0;

  libusb_device **devs;

  // MIDI-methods
  void createMidiIn();
  void createMidiOut();
  void setupMidiPorts();
  int detectDevices();
  double getMessage(BYTE_VECTOR *message);

  // USB-methods
  bool setupUSB();
  void printUSBDevs();

signals:
  void detectionProcess(int value);
};

class ProgressBarEvent : public QEvent {};

#endif // DEVICEDETECTIONPROCESSOR_H
