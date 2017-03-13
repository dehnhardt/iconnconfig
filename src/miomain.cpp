#include "miomain.h"
#include "sysex/midi.h"
#include "ui_miomain.h"

#include <QtDebug>
#include <unistd.h>

MioMain::MioMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::MioMain) {
  ui->setupUi(this);
}

MioMain::~MioMain() {
  libusb_exit(NULL);
  delete midiin;
  delete midiout;
  delete ui;
}

bool MioMain::setupUSB() {
  int r;

  r = libusb_init(NULL);
  if (r < 0)
    return false;

  return true;
}

void MioMain::printUSBDevs() {
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

void MioMain::setupMidiPorts() {
  createMidiIn();
  createMidiOut();

  unsigned int nPorts = midiin->getPortCount();

  std::vector<unsigned char> message;
  std::string portName;
  for (unsigned int i = 0; i < nPorts; i++) {
    midiin->openPort(i);
    midiout->openPort(i);
    try {
      portName = midiout->getPortName(i);
      std::cout << std::endl << "op: " << portName;
      portName = midiin->getPortName(i);
      std::cout << " / ip: " << portName << ": ";
    } catch (RtMidiError &error) {
      error.printMessage();
    }
    sentSysexMessage(message);
    midiin->closePort();
    midiout->closePort();
  }
}

void MioMain::mmcallback(double deltatime, std::vector<unsigned char> *message,
                         void * /*userData*/) {
  unsigned int nBytes = message->size();
  long serialNumber;

  for (unsigned int i = 0; i < nBytes; i++)
    std::cout << std::hex << (int)message->at(i) << " ";
  if (nBytes > 0)
    std::cout << "stamp = " << deltatime << std::endl;

  // test for iConnectivity device
  if ((nBytes >= 16) && (message->at(0) == SYSEX_START) &&
      (message->at(1) == Device::MAUNFACTURER_SYSEX_ID[0]) &&
      (message->at(2) == Device::MAUNFACTURER_SYSEX_ID[1]) &&
      (message->at(3) == Device::MAUNFACTURER_SYSEX_ID[2]) &&
      (message->at(15) == 0x02)) {
    std::cout << "iConnectivity Device found \n";
    serialNumber = MIDI::byteJoin(message, (unsigned int)7, (unsigned int)5);
    std::cout << "device with serial number " << serialNumber
              << " should be created here \n";
    /*if( MioMain::devices.find(serialNumber) != MioMain::devices.end() ){
        MioMain::devices.insert(std::pair<long, Device*>(serialNumber, new
    Device(serialNumber, message)));
    } else {
        std::cout << "but it's already recognized \n";
    }*/
  }
}

void MioMain::on_actionQuit_triggered() { close(); }

void MioMain::createMidiIn() {
  // RtMidiIn constructor
  try {
    midiin = new RtMidiIn(RtMidi::LINUX_ALSA);
  } catch (RtMidiError &error) {
    // Handle the exception here
    error.printMessage();
  }
  midiin->ignoreTypes(false, true, true);
  midiin->setCallback(&mmcallback, this);
  unsigned int nPorts = midiin->getPortCount();
  qDebug() << "\nThere are " << nPorts << " MIDI input sources available.\n";
  std::string portName;
  for (unsigned int i = 0; i < nPorts; i++) {
    try {
      portName = midiin->getPortName(i);
    } catch (RtMidiError &error) {
      error.printMessage();
      close();
    }
    qDebug() << "  Input Port #" << i + 1 << ": " << portName.c_str() << "\n";
  }
}

void MioMain::createMidiOut() {
  // RtMidiOut constructor
  try {
    midiout = new RtMidiOut(RtMidi::LINUX_ALSA);
  } catch (RtMidiError &error) {
    error.printMessage();
    exit(EXIT_FAILURE);
  }
  // Check outputs.
  unsigned int nPorts = midiout->getPortCount();
  qDebug() << "\nThere are " << nPorts << " MIDI output ports available.\n";
  std::string portName;
  for (unsigned int i = 0; i < nPorts; i++) {
    try {
      portName = midiout->getPortName(i);
    } catch (RtMidiError &error) {
      error.printMessage();
    }
    qDebug() << "  Output Port #" << i + 1 << ": " << portName.c_str() << "\n";
  }
  qDebug() << "\n";
}

void MioMain::sentSysexMessage(std::vector<unsigned char> message) {
  std::vector<unsigned char> message1;
  message.clear();
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
  usleep(3500); // pause a little
}
