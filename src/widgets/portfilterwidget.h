#ifndef PORTFILTERWIDGET_H
#define PORTFILTERWIDGET_H

#include "../device.h"
#include "../sysex/retsetmidiportfilter.h"
#include "portfiltersettingswidget.h"

#include <QGridLayout>
#include <QObject>
#include <QTabWidget>
#include <QTimer>
#include <QWidget>

class PortFilterWidget : public QWidget {
	Q_OBJECT

  public:
	explicit PortFilterWidget(Device *device, int portNumber,
							  QWidget *parent = nullptr);
	virtual ~PortFilterWidget();
  signals:

  public slots:
	void midiPortFilterUpdated(PortFilterDirection direction);
  protected slots:
	void updateInFilter();
	void updateOutFilter();

  private:
	// members
	Device *device;
	int portNumber = 0;
	RetSetMidiPortFilter *m_pMidiPortFilterIn = nullptr;
	RetSetMidiPortFilter *m_pMidiPortFilterOut = nullptr;
	QTabWidget *m_pFilterDirectionTabWidget = nullptr;
	PortFilterSettingsWidget *m_pInputFilterWidget = nullptr;
	PortFilterSettingsWidget *m_pOutputFilterWidget = nullptr;

	QTimer *m_pUpdateTimerInFilter = nullptr;
	QTimer *m_pUpdateTimerOutFilter = nullptr;

  private:
	// methods
	void createWidgets();
	void retrieveData();
	void loadData();
	void createConnections();
};

#endif // PORTFILTERWIDGET_H
