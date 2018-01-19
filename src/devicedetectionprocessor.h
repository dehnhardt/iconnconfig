#ifndef DEVICEDETECTIONPROCESSOR_H
#define DEVICEDETECTIONPROCESSOR_H

#include "RtMidi.h"
#include "device.h"
#include "libusb.h"

#include <QEvent>
#include <QWidget>

class DeviceDetectionProcessor
{
public:
	DeviceDetectionProcessor(QWidget *m_pGui);
	~DeviceDetectionProcessor();
	unsigned int getMidiInPortCount();
	unsigned int getMidiOutPortCount();
	void startDeviceDetection();

private:

	libusb_device **devs;

	// Methods
	bool isIconnectivityDevice(BYTE_VECTOR *message);

	// MIDI-methods
	void createMidiIn();
	void createMidiOut();
	void setupMidiPorts();
	unsigned long detectDevices();
	double getMessage(BYTE_VECTOR *message);

	// USB-methods
	bool setupUSB();
	void printUSBDevs();

	void sendProgressEvent(unsigned int progress);

	// Members
	RtMidiIn *m_pMidiin = 0;
	RtMidiOut *m_pMidiout = 0;
	QWidget *m_pGui;

};

#endif// DEVICEDETECTIONPROCESSOR_H
