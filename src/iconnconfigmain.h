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
#include <QTranslator>

namespace Ui {
class MioMain;
}

class MioMain : public QMainWindow {
	Q_OBJECT

  public:
	explicit MioMain(QCommandLineParser *parser, QWidget *parent = nullptr);
	~MioMain() override;
	void setConfigurationFile(QString *file) {
		this->m_sConfigurationFile = file;
	}
	void replacePanel(QWidget *w);

	CentralWidget *getCentralDeviceWidget() const;
	static MioMain *getMainWin();

  protected:
	void closeEvent(QCloseEvent *event) override;
	void changeEvent(QEvent *event) override;

  protected slots:
	void slotLanguageChanged(QAction *action);

  private:
	// methods
	void createLanguageMenu();
	void loadLanguage(const QString &rLanguage);

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
	CentralWidget *m_pCentralDeviceWidget = nullptr;
	std::map<Qt::DockWidgetArea, std::vector<QDockWidget *>>
		m_DockWidgetAreasMap;
	QString m_sTitle;
	QString *m_sConfigurationFile = nullptr;
	Device *m_pCurrentDevice = nullptr;

	QSettings *m_pConfiguration = nullptr;

	QTranslator
		m_translator; /*'''< contains the translations for this application*/
	QTranslator m_translatorQt; /*'''< contains the translations for qt*/
	QString m_currLang;         /*'''< contains the currently loaded language*/

  private:
	static MioMain *pMainWindow;
	static int sigpipe[2];

  signals:
	void deviceSelected(Device *d);

  public slots:
	void openDefaultDevice();

  private slots:
	void on_actionQuit_triggered();
	void openDetectionWindow();
	void openDeviceGUI(Device *d);
	void storeToDevice();
	void restoreFromDevice();
	void resetToFactoryDefaults();
	void signalAction(int);
	void openAboutDialog();
};

void switchTranslator(QTranslator &translator, const QString &filename);

#endif // MIOMAIN_H
