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
		if (m_pDevices != nullptr) {
			m_pDevices->clear();
			delete m_pDevices;
		}
	}

  public:
	Configuration(Configuration const &) = delete;
	void operator=(Configuration const &) = delete;
	// end singleton

  public:
	// getter
	Devices *getDevices() {
		if (m_pDevices == nullptr)
			m_pDevices = new std::map<unsigned long, Device *>;
		return m_pDevices;
	}
	QSettings *getSettings() { return new QSettings(); }

	unsigned long getDefaultDevice() {
		unsigned long defaultDevice = 0;
		QSettings *settings = new QSettings();
		settings->beginGroup("Default Device");
		defaultDevice = settings->value("serialNumber", 0).toUInt();
		settings->endGroup();
		delete settings;
		return defaultDevice;
	}

	bool getUsbDeviceDetection() { return m_bEnableUsbDetection; }
	bool getMidiDeviceDetection() { return m_bEnableMidiDeviceDetection; }

	// setter
	void setDevices(Devices *devices) { this->m_pDevices = devices; }
	void setDefaultDevice(long serialNumber) {
		QSettings *settings = new QSettings();
		settings->beginGroup("Default Device");
		settings->setValue("serialNumber",
						   static_cast<qlonglong>(serialNumber));
		settings->endGroup();
		delete settings;
	}

  private:
	Devices *m_pDevices;
	bool m_bEnableUsbDetection = false;
	bool m_bEnableMidiDeviceDetection = true;
	// QSettings *settings = 0;
};

#endif // CONFIGURATION_H
