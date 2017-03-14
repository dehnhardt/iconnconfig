#ifndef DEVICEDETECTION_H
#define DEVICEDETECTION_H

#include "RtMidi.h"
#include "device.h"
#include "devicedetection.h"
#include "libusb.h"

#include <QDialog>

namespace Ui {
class DeviceDetection;
}

class DeviceDetection : public QDialog {
  Q_OBJECT

public:
  explicit DeviceDetection(QWidget *parent = 0);
  ~DeviceDetection();

private slots:
  void startDeviceDetection();

private:
  Ui::DeviceDetection *ui = 0;

  // Members
  RtMidiIn *midiin = 0;
  RtMidiOut *midiout = 0;

  libusb_device **devs;

  // MIDI-methods
  void createMidiIn();
  void createMidiOut();
  void setupMidiPorts();
  int detectDevices();
  void sentSysexMessage();
  double getMessage(std::vector<unsigned char> *message);

  // USB-methods
  bool setupUSB();
  void printUSBDevs();
};

#endif // DEVICEDETECTION_H
