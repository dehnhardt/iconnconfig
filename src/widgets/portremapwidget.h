#ifndef PORTREMAPWIDGET_H
#define PORTREMAPWIDGET_H

#include "../device.h"
#include "../sysex/retsetmidiportremap.h"
#include "portremapsettingswidget.h"

#include <QGridLayout>
#include <QObject>
#include <QTabWidget>
#include <QTimer>
#include <QWidget>

class PortRemapWidget : public QWidget {
	Q_OBJECT

  public:
    explicit PortRemapWidget(Device *device, int portNumber,
							  QWidget *parent = nullptr);
    virtual ~PortRemapWidget();
  signals:

  public slots:
    void midiPortRemapUpdated(PortRemapDirection direction);
  protected slots:
    void updateInRemap();
    void updateOutRemap();

  private:
	// members
	Device *device;
	int portNumber = 0;
    RetSetMidiPortRemap *m_pMidiPortRemapIn = nullptr;
    RetSetMidiPortRemap *m_pMidiPortRemapOut = nullptr;
    QTabWidget *m_pRemapDirectionTabWidget = nullptr;
    PortRemapSettingsWidget *m_pInputRemapWidget = nullptr;
    PortRemapSettingsWidget *m_pOutputRemapWidget = nullptr;

    QTimer *m_pUpdateTimerInRemap = nullptr;
    QTimer *m_pUpdateTimerOutRemap = nullptr;

  private:
	// methods
	void createWidgets();
	void retrieveData();
	void loadData();
	void createConnections();
};

#endif // PORTREMAPWIDGET_H
