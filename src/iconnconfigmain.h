#ifndef MIOMAIN_H
#define MIOMAIN_H

#include "devicedetection.h"
#include "sysex/saverestore.h"
#include "widgets/centralwidget.h"

#include <QCommandLineParser>
#include <QFile>
#include <QMainWindow>
#include <QSettings>
#include <QToolBar>

namespace Ui {
class MioMain;
}

class DeviceMenuMapper : public QObject {
  public:
	DeviceMenuMapper(Device *device) : device(device) {}
	~DeviceMenuMapper();
	Device *device;
};

class MioMain : public QMainWindow {
	Q_OBJECT

  public:
	explicit MioMain(QCommandLineParser *parser, QWidget *parent = nullptr);
	~MioMain();
	void setConfigurationFile(QString *file) {
		this->m_sConfigurationFile = file;
	}
	void replacePanel(QWidget *w);

  public slots:
	void openDefaultDevice();

  protected:
	void closeEvent(QCloseEvent *event);

  private slots:
	void on_actionQuit_triggered();
	void openDetectionWindow();
	void openDeviceGUI(QObject *m);
	void openDeviceGUI(Device *d);
	void storeToDevice();
	void restoreFromDevice();
	void resetToFactoryDefaults();
	void signalAction(int);
	void openAboutDialog();

  private:
	// methods
	void connectSignals();
	void readSettings();
	bool readDevicesFromSettings();
	void writeSettings();
	void writeDevicesToSettings();
	void connectSlots();
	void addDevicesToSelectionMenu(unsigned long defaultDeviceSN);

	void addDock(QDockWidget *widget,
				 Qt::DockWidgetArea area = Qt::NoDockWidgetArea);

	void clearDocWidgets();
	void saveRestore(SaveRestore::SaveResstoreId saveRestoreId);
	void addDeviceToolButtons();

	void reinitDevice(int ret);
	void reinitDevice();

	void readConfigurationFromFile();
	void writeConfigurtionToFile();

	// signalling
	static void handleSignal(int);
	bool installSignalHandlers();

  private:
	// Members
	Ui::MioMain *m_pUi;
	DeviceDetection *m_pDeviceDetectionWindow = nullptr;
	QToolBar *m_pToolBar = nullptr;
	std::map<Qt::DockWidgetArea, std::vector<QDockWidget *>>
		m_DockWidgetAreasMap;
	QString m_sTitle;
	QString *m_sConfigurationFile = nullptr;
	Device *m_pCurrentDevice = nullptr;

	QSettings *m_pConfiguration = nullptr;

	static int sigpipe[2];

  signals:
	void deviceSelected(Device *d);
};

#endif // MIOMAIN_H
