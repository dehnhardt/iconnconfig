#ifndef PORTROUTINGWIDGET_H
#define PORTROUTINGWIDGET_H

#include "../device.h"
#include "../sysex/retsetmidiportroute.h"
#include "controls/portbutton.h"

#include <QGridLayout>
#include <QSignalMapper>
#include <QWidget>

class PortButtonMapper : public QObject {
public:
	PortButtonMapper(PortButton *portButton) : portButton(portButton) {}
	PortButton *portButton;
};

class PortRoutingWidget : public QWidget {
	Q_OBJECT
public:
	explicit PortRoutingWidget(Device *device, int portNumber,
														 QWidget *parent = 0);
	virtual ~PortRoutingWidget();
signals:

public slots:

protected slots:
	void lineButtonClicked(QObject *object);

private:
	// members
	Device *device;
	QGridLayout *layout = 0;
	int portNumber = 0;
	RetSetMidiPortRoute *midiPortRoute = 0;

	std::vector<std::vector<PortButton *> *> *buttonLines;

	QSignalMapper *lineButtonMapper = 0;
	QSignalMapper *portButtonMapper = 0;

private:
	// methods
	void createWidgets();
	void setupWidgets();
	void setupLayout();
	void retrieveData();
	void createSignalMapper();
	void createMidiPortSections(Device *device);
	void createMidiPorts(int line,
											 std::vector<RetSetMidiPortInfo *> *midiPortInfos);
	int getButtonLineIndex(PortButton *b);
	bool isButtonChecked(int row);
	void setButtonsChecked(int row, bool checked);
};

#endif // PORTROUTINGWIDGET_H
