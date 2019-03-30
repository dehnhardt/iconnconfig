#ifndef PORTROUTINGWIDGET_H
#define PORTROUTINGWIDGET_H

#include "../device.h"
#include "../sysex/retsetmidiportroute.h"
#include "controls/portbutton.h"

#include <QGridLayout>
#include <QSignalMapper>
#include <QTimer>
#include <QWidget>

class PortRoutingWidget : public QWidget {
	Q_OBJECT
  public:
	explicit PortRoutingWidget(Device *device, int portNumber,
							   QWidget *parent = nullptr);
	virtual ~PortRoutingWidget();
  signals:

  public slots:

  protected slots:
	void lineButtonClicked(PortButton *b);
	void portButtonClicked(PortButton *b);
	void updateRouting();

  private:
	// members
	Device *device;
	QGridLayout *layout = nullptr;
	int portNumber = 0;
	std::shared_ptr<RetSetMidiPortRoute> m_pMidiPortRoute;
	QTimer *updateTimer = nullptr;

	std::vector<std::vector<PortButton *>> *buttonLines;

  private:
	// methods
	void createWidgets();
	void setupWidgets();
	void setupLayout();
	void retrieveData();
	void loadData();
	void createMidiPortSections(Device *device);
	void createMidiPorts(
		int line,
		std::vector<std::shared_ptr<RetSetMidiPortInfo>> *midiPortInfos);
	int getButtonLineIndex(PortButton *b);
	bool isButtonChecked(int row);
	void setButtonsChecked(int row, bool checked);
};

#endif // PORTROUTINGWIDGET_H
