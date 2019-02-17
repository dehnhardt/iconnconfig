#ifndef PORTFILTERWIDGET_H
#define PORTFILTERWIDGET_H

#include "../device.h"
#include "../sysex/retsetmidiportfilter.h"
#include "portfiltersettingswidget.h"

#include <QGridLayout>
#include <QObject>
#include <QTabWidget>
#include <QWidget>

class PortFilterWidget : public QWidget {
	Q_OBJECT

  public:
	explicit PortFilterWidget(Device *device, unsigned int portNumber,
							  QWidget *parent = nullptr);
	virtual ~PortFilterWidget();
  signals:

  public slots:

  protected slots:

  private:
	// members
	Device *device;
	unsigned int portNumber = 0;
	RetSetMidiPortFilter *m_pMidiPortFilterIn = nullptr;
	RetSetMidiPortFilter *m_pMidiPortFilterOut = nullptr;
	QTabWidget *m_pFilterDirectionTabWidget = nullptr;
	PortFilterSettingsWidget *m_pInputFilterWidget = nullptr;
	PortFilterSettingsWidget *m_pOutputFilterWidget = nullptr;

	/*	QTimer *updateTimer = 0; */

  private:
	// methods
	void createWidgets();
	void retrieveData();
	void loadData();
	/*void setupWidgets();
	void setupLayout();
	void createSignalMapper();
	void createMidiPortSections(Device *device);
	void createMidiPorts(int line,
						 std::vector<RetSetMidiPortInfo *> *midiPortInfos);
	int getButtonLineIndex(PortButton *b);
	bool isButtonChecked(int row);
	void setButtonsChecked(int row, bool checked);*/
};

#endif // PORTFILTERWIDGET_H
