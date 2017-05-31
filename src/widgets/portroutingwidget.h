#ifndef PORTROUTINGWIDGET_H
#define PORTROUTINGWIDGET_H

#include "../device.h"
#include "controls/portbutton.h"

#include <QGridLayout>
#include <QWidget>

class PortRoutingWidget : public QWidget {
	Q_OBJECT
public:
	explicit PortRoutingWidget(Device *device, QWidget *parent = 0,
														 int numDinPorts = 0);

signals:

public slots:

private:
	// members
	Device *device;
	int numDinPorts = 0;
	std::vector<PortButton *> *portButtons = 0;
	QGridLayout *layout = 0;

private:
	// methods
	void createWidgets();
	void setupWidgets();
	void setupLayout();
	void setData();
	void createConnections();
	void getMidiPortSections(Device *device);
	void getMidiPorts(int line, std::vector<RetSetMidiPortInfo *> *midiPortInfos);
};

#endif // PORTROUTINGWIDGET_H
