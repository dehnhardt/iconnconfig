#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "../device.h"
#include <QSettings>
#include <map>

#define AUTHOR "Holger Dehnhardt"
#define LICENSE "MIT"

typedef std::map<unsigned long, Device *> Devices;

class Configuration {

	// start singleton
public:
	static Configuration &getInstance() {
		static Configuration instance;
		return instance;
	}

private:
	Configuration() {}
	~Configuration() {
		/*if (settings)
		  delete settings;*/
		if (devices != 0) {
			devices->clear();
			delete devices;
		}
	}

public:
	Configuration(Configuration const &) = delete;
	void operator=(Configuration const &) = delete;
	// end singleton

public:
	// getter
	Devices *getDevices() {
		if (devices == 0)
			devices = new std::map<unsigned long, Device *>;
		return devices;
	}
	QSettings *getSettings() { return new QSettings(); }

	long getDefaultDevice() {
		QSettings *settings = new QSettings();
		settings->beginGroup("Default Device");
		return settings->value("serialNumber", 0).toInt();
		settings->endGroup();
		delete settings;
	}

	bool getUsbDeviceDetection() { return enableUsbDetection; }
	bool getMidiDeviceDetection() { return enableMidiDeviceDetection; }

	// setter
	void setDevices(Devices *devices) { this->devices = devices; }
	void setDefaultDevice(int serialNumber) {
		QSettings *settings = new QSettings();
		settings->beginGroup("Default Device");
		settings->setValue("serialNumber", serialNumber);
		settings->endGroup();
		delete settings;
	}

private:
	Devices *devices;
	bool enableUsbDetection = false;
	bool enableMidiDeviceDetection = true;
	// QSettings *settings = 0;
};

#endif// CONFIGURATION_H
