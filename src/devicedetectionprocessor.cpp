#include "devicedetectionprocessor.h"
#include "config/configuration.h"
#include "events/events.h"
#include "regex"
#include "sysex/communicationexception.h"
#include "sysex/getdevice.h"
#include "sysex/midi.h"

#include <QApplication>
#include <QDebug>
#include <QSettings>
#include <iomanip>
#include <unistd.h>

DeviceDetectionProcessor::DeviceDetectionProcessor(QWidget *gui) : m_pGui(gui) {
	if (Configuration::getInstance().getMidiDeviceDetection())
        setupMidiPorts();
#ifdef __LINUX__
	if (Configuration::getInstance().getUsbDeviceDetection())
        setupUSB();
#endif
}

DeviceDetectionProcessor::~DeviceDetectionProcessor() {
#ifdef __LINUX__
	if (Configuration::getInstance().getUsbDeviceDetection())
        libusb_exit(nullptr);
#endif
	if (Configuration::getInstance().getMidiDeviceDetection()) {
		m_pMidiin = nullptr;
		m_pMidiout = nullptr;
	}
}

unsigned int DeviceDetectionProcessor::getMidiInPortCount() {
	if (m_pMidiin)
		return m_pMidiin->getPortCount();
	return 0;
}

unsigned int DeviceDetectionProcessor::getMidiOutPortCount() {
	if (m_pMidiout)
		return m_pMidiout->getPortCount();
	return 0;
}

/* MIDI-methods */

void DeviceDetectionProcessor::startDeviceDetection() {
	if (Configuration::getInstance().getMidiDeviceDetection())
		detectDevices();
}

bool DeviceDetectionProcessor::isIconnectivityDevice(
	std::vector<unsigned char> *message) {
	unsigned long nMessageSize = message->size();
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

void DeviceDetectionProcessor::sendProgressEvent(unsigned int progress) {
	ProgressEvent *e = new ProgressEvent();
	e->setValue(progress);
	QApplication::sendEvent(m_pGui, e);
}

unsigned long DeviceDetectionProcessor::detectDevices() {

	unsigned long defaultDeviceSerialNumber = static_cast<unsigned long>(
		Configuration::getInstance().getDefaultDevice());
	unsigned int nOutPortCount = m_pMidiout->getPortCount();
	unsigned int nInPortCount = m_pMidiin->getPortCount();
#ifdef __MIO_DEBUG__
	std::cout << "Out ports: " << std::dec << nOutPortCount
			  << ", in ports: " << nInPortCount
			  << " - combinations to probe:  " << nOutPortCount * nInPortCount
			  << std::endl;
#endif //__MIO_DEBUG__
	GetDevice *q = new GetDevice();
	unsigned long serialNumber;
	bool bDeviceFound = false;
	BYTE_VECTOR *qMessage = q->getMIDISysExMessage();
	std::map<unsigned long, std::shared_ptr<Device>> *devices =
		Configuration::getInstance().getDevices();
	// for each output signal
	for (unsigned int i = 0; i < nOutPortCount; i++) {
        try{
		m_pMidiout->openPort(i);
        } catch(...){
            std::cerr << "failed to open output port " << i << std::endl;
        }
		std::string currentOutPortName = m_pMidiout->getPortName(i);
		std::regex re("(.+) [0-9]+:[0-9]+");
		std::cmatch mOut;
		std::regex_search(currentOutPortName.c_str(), mOut, re);
		std::cout << "OutputPort " << currentOutPortName << " (" << mOut[1]
				  << ")" << std::endl;
		// and each input signal
		for (unsigned int j = 0; j < nInPortCount; j++) {
			QCoreApplication::processEvents();
			unsigned int progress = (i * nInPortCount) + j + 1;
			if (bDeviceFound) {
				bDeviceFound = false;
				progress = (i * nInPortCount) + nInPortCount + 1;
			}
			sendProgressEvent(progress);
            try{
			m_pMidiin->openPort(j);
            } catch(...){
                std::cerr << "failed to open input port "<< j << std::endl;
                continue;
            }
			std::string currentInputPortName = m_pMidiin->getPortName(j);
			std::cmatch mIn;
			std::regex_search(currentInputPortName.c_str(), mIn, re);
			std::cout << "InputPort " << currentInputPortName << " (" << mIn[1]
					  << ")" << std::endl;
			if (mIn[1] != mOut[1]) {
				m_pMidiin->closePort();
				continue;
			}
			m_pMidiout->sendMessage(qMessage);
			// pause a little
			BYTE_VECTOR *message = new BYTE_VECTOR;
			for (int i = 0; i < WAIT_LOOPS && message->size() == 0; i++) {
				SLEEP(WAIT_TIME);
				m_pMidiin->getMessage(message);
			}
			unsigned long nMessageSize = message->size();
			if (nMessageSize > 0) {
#ifdef __MIO_DEBUG__
				MIDI::printMessage(message);
#endif //__MIO_DEBUG__
	   // test for iConnectivity device
				if (isIconnectivityDevice(message)) {
					serialNumber = static_cast<unsigned long>(
						MIDI::byteJoin7bit(message, 7, 5));
					std::cout << "device with serial number " << serialNumber
							  << " detected... (" << m_pMidiout->getPortName(i)
							  << ":" << i << " - " << m_pMidiin->getPortName(j)
							  << ":" << j << ") ";
					m_pMidiin->closePort();
					m_pMidiout->closePort();
					if (devices->find(serialNumber) == devices->end()) {
						unsigned int productId = static_cast<unsigned int>(
							MIDI::byteJoin7bit(message, 5, 2));
						Device *device =
							new Device(j, i, serialNumber, productId);
						devices->insert(
							std::pair<long, Device *>(serialNumber, device));
						std::cout << "... and added to list of devices"
								  << std::endl;
						bDeviceFound = true;
					} else
						std::cout << "... but it's already recognized"
								  << std::endl;
					break;
				}
#ifdef __MIO_DEBUG__
				else {
					std::cout << "there is an answer from a device but no "
								 "iConnectivity: "
							  << m_pMidiout->getPortName(i) << " - "
							  << m_pMidiin->getPortName(j) << std::endl;
				}
#endif // __MIO_DEBUG__
			}
#ifdef __MIO_DEBUG__
			else {
				std::cout << "no answer from any device: "
						  << m_pMidiout->getPortName(i) << " - "
						  << m_pMidiin->getPortName(j) << std::endl;
			}
#endif //__MIO_DEBUG__
			m_pMidiin->closePort();
		}
		if (m_pMidiout->isPortOpen())
			m_pMidiout->closePort();
	}
	sendProgressEvent(getMidiInPortCount() * getMidiOutPortCount());

	std::shared_ptr<Device> d;
	try {
		d = devices->at(defaultDeviceSerialNumber);
		d->setDefault(true);
	} catch (std::out_of_range) {
		if (devices->size() > 0) {
			d = devices->begin()->second;
			d->setDefault(true);
			defaultDeviceSerialNumber = static_cast<unsigned long>(
				d->getSerialNumber()->getLongValue());
			Configuration::getInstance().setDefaultDevice(
				defaultDeviceSerialNumber);
		}
	}

	if (devices->size() > 0) {
		for (Devices::iterator it = devices->begin(); it != devices->end();
			 ++it) {
			d = it->second;
			try {
				d->queryDeviceInfo();
			} catch (...) {
				continue;
			}
		}
		Configuration::getInstance().setDevices(devices);
	}
	return devices->size();
}

double DeviceDetectionProcessor::getMessage(BYTE_VECTOR *message) {
	double deltatime = m_pMidiin->getMessage(message);
	return deltatime;
}

void DeviceDetectionProcessor::createMidiIn() {
	m_pMidiin = MIDI::createMidiIn();
#ifdef __MIO_DEBUG__
	unsigned int nPorts = m_pMidiin->getPortCount();
	std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
	std::string portName;
	for (unsigned int i = 0; i < nPorts; i++) {
		try {
			portName = m_pMidiin->getPortName(i);
		} catch (RtMidiError &error) {
			error.printMessage();
		}
		std::cout << "  Input Port #" << i + 1 << ": " << portName.c_str()
				  << "\n";
	}
#endif //__MIO_DEBUG_
}

void DeviceDetectionProcessor::createMidiOut() {
	// RtMidiOut constructor
	m_pMidiout = MIDI::createMidiOut();
	// Check outputs.
#ifdef __MIO_DEBUG__
	unsigned int nPorts = m_pMidiout->getPortCount();
	std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";
	std::string portName;
	for (unsigned int i = 0; i < nPorts; i++) {
		try {
			portName = m_pMidiout->getPortName(i);
		} catch (RtMidiError &error) {
			error.printMessage();
		}
		std::cout << "  Output Port #" << i + 1 << ": " << portName.c_str()
				  << "\n";
	}
#endif //__MIO_DEBUG__
}

#ifdef __LINUX__
/* USB methods - currently not used */
bool DeviceDetectionProcessor::setupUSB() {
	int r;

	r = libusb_init(nullptr);
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

	cnt = libusb_get_device_list(nullptr, &devs);
	if (cnt < 0)
		return;

	while ((dev = devs[i++]) != nullptr) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return;
		}

		printf("%04x:%04x (bus %d, device %d)", desc.idVendor, desc.idProduct,
			   libusb_get_bus_number(dev), libusb_get_device_address(dev));
		if (desc.idVendor == Device::MANUFACTURER_USB_ID)
			std::cout << "Found iConnectivity Device";
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
#endif
