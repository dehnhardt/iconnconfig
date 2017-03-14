#include "devicedetection.h"
#include "config/configuration.h"
#include "sysex/midi.h"
#include "ui_devicedetection.h"

#include <QTimer>
#include <unistd.h>

DeviceDetection::DeviceDetection(QWidget *parent)
    : QDialog(parent), ui(new Ui::DeviceDetection) {
  ui->setupUi(this);
  QTimer::singleShot(1000, this, SLOT(startDeviceDetection()));
}

DeviceDetection::~DeviceDetection() {
  delete ui;
  if (Configuration::getInstance().getUsbDeviceDetection())
    libusb_exit(NULL);
  if (Configuration::getInstance().getMidiDeviceDetection()) {
    midiin = NULL;
    midiout = NULL;
  }
}

/* MIDI-methods */

void DeviceDetection::startDeviceDetection() {
  if (Configuration::getInstance().getMidiDeviceDetection()) {
    setupMidiPorts();
    detectDevices();
  }
  if (Configuration::getInstance().getUsbDeviceDetection()) {
    setupUSB();
    detectDevices();
  }
}

void DeviceDetection::setupMidiPorts() {
  createMidiIn();
  createMidiOut();
}

int DeviceDetection::detectDevices() {
  int nOutPortCount = midiout->getPortCount();
  int nInPortCount = midiin->getPortCount();
  long serialNumber;
  std::map<long, Device *> *devices = Configuration::getInstance().getDevices();
  for (int i = 0; i < nOutPortCount; i++) {
    midiout->openPort(i);
    for (int j = 0; j < nInPortCount; j++) {
      midiin->openPort(j);
      sentSysexMessage();
      usleep(1000); // pause a little
      std::vector<unsigned char> *message = new std::vector<unsigned char>;
      double deltatime = getMessage(message);
      unsigned int nMessageSize = message->size();
      if (nMessageSize > 0) {
        for (unsigned int i = 0; i < nMessageSize; i++)
          std::cout << std::hex << (int)message->at(i) << " ";
        if (nMessageSize > 0)
          std::cout << "stamp = " << deltatime << std::endl;

        // test for iConnectivity device
        if ((nMessageSize >= 16) && (message->at(0) == SYSEX_START) &&
            (message->at(1) == Device::MAUNFACTURER_SYSEX_ID[0]) &&
            (message->at(2) == Device::MAUNFACTURER_SYSEX_ID[1]) &&
            (message->at(3) == Device::MAUNFACTURER_SYSEX_ID[2]) &&
            (message->at(15) == 0x02)) {
          std::cout << "iConnectivity Device found \n";
          serialNumber =
              MIDI::byteJoin(message, (unsigned int)7, (unsigned int)5);
          std::cout << "device with serial number " << serialNumber
                    << " should be created here \n";
          midiin->closePort();
          if (devices->find(serialNumber) != devices->end()) {
            devices->insert(std::pair<long, Device *>(
                serialNumber, new Device(serialNumber, message)));
          } else {
            std::cout << "but it's already recognized \n";
          }

          break;
        }
      }
      midiin->closePort();
    }
    midiout->closePort();
  }
  return 0;
}

double DeviceDetection::getMessage(std::vector<unsigned char> *message) {
  double deltatime = midiin->getMessage(message);
  return deltatime;
}

void DeviceDetection::createMidiIn() {
  // RtMidiIn constructor
  try {
    midiin = new RtMidiIn(RtMidi::LINUX_ALSA);
  } catch (RtMidiError &error) {
    // Handle the exception here
    error.printMessage();
  }
  midiin->ignoreTypes(false, true, true);
  // midiin->setCallback(&mmcallback, this);
  unsigned int nPorts = midiin->getPortCount();
  std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
  std::string portName;
  for (unsigned int i = 0; i < nPorts; i++) {
    try {
      portName = midiin->getPortName(i);
    } catch (RtMidiError &error) {
      error.printMessage();
      close();
    }
    std::cout << "  Input Port #" << i + 1 << ": " << portName.c_str() << "\n";
  }
}

void DeviceDetection::createMidiOut() {
  // RtMidiOut constructor
  try {
    midiout = new RtMidiOut(RtMidi::LINUX_ALSA);
  } catch (RtMidiError &error) {
    error.printMessage();
    exit(EXIT_FAILURE);
  }
  // Check outputs.
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
  std::cout << "\n";
}

void DeviceDetection::sentSysexMessage() {
  std::vector<unsigned char> message1;
  message1.clear();
  message1.push_back(0xF0);
  message1.push_back(0x00);
  message1.push_back(0x01);
  message1.push_back(0x73);
  message1.push_back(0x7e);
  message1.push_back(0x00);
  message1.push_back(0x00);
  message1.push_back(0x00);
  message1.push_back(0x00);
  message1.push_back(0x00);
  message1.push_back(0x00);
  message1.push_back(0x00);
  message1.push_back(0x00);
  message1.push_back(0x01);
  message1.push_back(0x40);
  message1.push_back(0x01);
  message1.push_back(0x00);
  message1.push_back(0x00);
  message1.push_back(0x3E);
  message1.push_back(0xF7);
  midiout->sendMessage(&message1);
}

/* USB methods - currently not used */
bool DeviceDetection::setupUSB() {
  int r;

  r = libusb_init(NULL);
  if (r < 0)
    return false;

  return true;
}

void DeviceDetection::printUSBDevs() {
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
    if (desc.idVendor == Device::MAUNFACTURER_USB_ID) {
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
