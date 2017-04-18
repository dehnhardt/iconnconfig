#include "devicedetectionprocessor.h"
#include "config/configuration.h"
#include "events/events.h"
#include "sysex/devicedetectionquery.h"
#include "sysex/midi.h"

#include <QApplication>
#include <QDebug>
#include <QSettings>
#include <iomanip>
#include <unistd.h>

DeviceDetectionProcessor::DeviceDetectionProcessor(QWidget *gui) : gui(gui) {
  if (Configuration::getInstance().getMidiDeviceDetection()) {
    setupMidiPorts();
  }
  if (Configuration::getInstance().getUsbDeviceDetection()) {
    setupUSB();
  }
}

DeviceDetectionProcessor::~DeviceDetectionProcessor() {
  if (Configuration::getInstance().getUsbDeviceDetection())
    libusb_exit(NULL);
  if (Configuration::getInstance().getMidiDeviceDetection()) {
    midiin = NULL;
    midiout = NULL;
  }
}

int DeviceDetectionProcessor::getMidiInPortCount() {
  if (midiin)
    return midiin->getPortCount();
  return 0;
}

int DeviceDetectionProcessor::getMddiOutPortCount() {
  if (midiout)
    return midiout->getPortCount();
  return 0;
}

/* MIDI-methods */

void DeviceDetectionProcessor::startDeviceDetection() {
  if (Configuration::getInstance().getMidiDeviceDetection()) {
    setupMidiPorts();
    detectDevices();
  }
  if (Configuration::getInstance().getUsbDeviceDetection()) {
    setupUSB();
  }
}

bool DeviceDetectionProcessor::isIconnectivityDevice(
    std::vector<unsigned char> *message) {
  int nMessageSize = message->size();
  return ((nMessageSize >= 16) && (message->at(0) == SYSEX_START) &&
          (message->at(1) == Device::MANUFACTURER_SYSEX_ID[0]) &&
          (message->at(2) == Device::MANUFACTURER_SYSEX_ID[1]) &&
          (message->at(3) == Device::MANUFACTURER_SYSEX_ID[2]) &&
          (message->at(15) == 0x02));
}

void DeviceDetectionProcessor::setupMidiPorts() {
  createMidiIn();
  createMidiOut();
}

void DeviceDetectionProcessor::sendProgressEvent(int progress) {
  ProgressEvent *e = new ProgressEvent();
  e->setValue(progress);
  QApplication::sendEvent(gui, e);
}

int DeviceDetectionProcessor::detectDevices() {

  int defaultDeviceSerialNumber =
      Configuration::getInstance().getDefaultDevice();
  int nOutPortCount = midiout->getPortCount();
  int nInPortCount = midiin->getPortCount();
#ifdef __MIO_DEBUG__
  std::cout << "Out ports: " << std::dec << nOutPortCount
            << ", in ports: " << nInPortCount
            << " - combinations to probe:  " << nOutPortCount * nInPortCount
            << std::endl;
#endif //__MIO_DEBUG__
  DeviceDetectionQuery *q = new DeviceDetectionQuery();
  long serialNumber;
  BYTE_VECTOR *qMessage = q->getMIDISysExMessage();
  std::map<long, Device *> *devices = Configuration::getInstance().getDevices();
  // for each output signal
  for (int i = 0; i < nOutPortCount; i++) {
    midiout->openPort(i);
    // and each input signal
    for (int j = 0; j < nInPortCount; j++) {
      int progress = (i * nInPortCount) + j + 1;
      sendProgressEvent(progress);
      midiin->openPort(j);
      midiout->sendMessage(qMessage);
      // pause a little
      usleep(1000);
      BYTE_VECTOR *message = new BYTE_VECTOR;
      midiin->getMessage(message);
      unsigned int nMessageSize = message->size();
      if (nMessageSize > 0) {
#ifdef __MIO_DEBUG__
        MIDI::printMessage(message);
#endif //__MIO_DEBUG__
        // test for iConnectivity device
        if (isIconnectivityDevice(message)) {
          serialNumber =
              MIDI::byteJoin(message, (unsigned int)7, (unsigned int)5);
          qDebug() << "device with serial number " << serialNumber
                   << "detected...";
          midiin->closePort();

          if (devices->find(serialNumber) == devices->end()) {
            int productId = MIDI::byteJoin(message, 5, 2);
            Device *device = new Device(j, i, serialNumber, productId);
            devices->insert(std::pair<long, Device *>(serialNumber, device));
            qDebug() << "... and added to list of devices";
          } else {
            qDebug() << "but it's already recognized";
          }
          break;
        }
      }
      midiin->closePort();
    }
    midiout->closePort();
    ProgressEvent *e = new ProgressEvent();
    e->setValue(getMidiInPortCount() * getMddiOutPortCount());
    QApplication::sendEvent(gui, e);
  }
#ifdef __MIO_SIMULATE__
  // if simulation is enabled, send some events to progress bar...
  int base = midiin->getPortCount() * midiout->getPortCount();
  for (int i = 0; i <= 27; i++) {
    sendProgressEvent(base + i);
    usleep(10000);
  }
  //... and create two devices
  Device *ds = new Device(1, 1, 0x11, 0x0101, "mio10", "Device 1");
  devices->insert(std::pair<long, Device *>(0x11, ds));
  ds = new Device(2, 2, 0x12, 0x0201, "mio2", "Device 2");
  devices->insert(std::pair<long, Device *>(0x12, ds));
#endif //__MIO_SIMULATE__
  Device *d = 0;
  try {
    d = devices->at(defaultDeviceSerialNumber);
    d->setDefault(true);
  } catch (std::out_of_range) {
    d = devices->begin()->second;
    d->setDefault(true);
    defaultDeviceSerialNumber = d->getSerialNumber()->getLongValue();
    Configuration::getInstance().setDefaultDevice(defaultDeviceSerialNumber);
  }

  for (Devices::iterator it = devices->begin(); it != devices->end(); it++) {
    d = it->second;
    d->queryDeviceInfo();
  }
  Configuration::getInstance().setDevices(devices);
  return devices->size();
}

double DeviceDetectionProcessor::getMessage(BYTE_VECTOR *message) {
  double deltatime = midiin->getMessage(message);
  return deltatime;
}

void DeviceDetectionProcessor::createMidiIn() {
  midiin = MIDI::createMidiIn();
#ifdef __MIO_DEBUG__
  unsigned int nPorts = midiin->getPortCount();
  std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
  std::string portName;
  for (unsigned int i = 0; i < nPorts; i++) {
    try {
      portName = midiin->getPortName(i);
    } catch (RtMidiError &error) {
      error.printMessage();
    }
    std::cout << "  Input Port #" << i + 1 << ": " << portName.c_str() << "\n";
  }
#endif //__MIO_DEBUG_
}

void DeviceDetectionProcessor::createMidiOut() {
  // RtMidiOut constructor
  midiout = MIDI::createMidiOut();
// Check outputs.
#ifdef __MIO_DEBUG__
  unsigned int nPorts = midiout->getPortCount();
  std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";
  std::string portName;
  for (unsigned int i = 0; i < nPorts; i++) {
    try {
      portName = midiout->getPortName(i);
    } catch (RtMidiError &error) {
      error.printMessage();
    }
    std::cout << "  Output Port #" << i + 1 << ": " << portName.c_str() << "\n";
  }
#endif //__MIO_DEBUG__
}

/* USB methods - currently not used */
bool DeviceDetectionProcessor::setupUSB() {
  int r;

  r = libusb_init(NULL);
  if (r < 0)
    return false;

  return true;
}

void DeviceDetectionProcessor::printUSBDevs() {
  ssize_t cnt;
  libusb_device **devs;
  libusb_device *dev;
  int i = 0, j = 0;
  uint8_t path[8];

  cnt = libusb_get_device_list(NULL, &devs);
  if (cnt < 0)
    return;

  while ((dev = devs[i++]) != NULL) {
    struct libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0) {
      fprintf(stderr, "failed to get device descriptor");
      return;
    }

    printf("%04x:%04x (bus %d, device %d)", desc.idVendor, desc.idProduct,
           libusb_get_bus_number(dev), libusb_get_device_address(dev));
    if (desc.idVendor == Device::MANUFACTURER_USB_ID) {
      std::cout << "Found iConnectivity Device";
    }
    r = libusb_get_port_numbers(dev, path, sizeof(path));
    if (r > 0) {
      printf(" path: %d", path[0]);
      for (j = 1; j < r; j++)
        printf(".%d", path[j]);
    }
    printf("\n");
  }

  libusb_free_device_list(devs, 1);
}
