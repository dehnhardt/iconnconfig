#ifndef PORTSWIDGET_H
#define PORTSWIDGET_H

#include "../device.h"
#include "multiinfowidget.h"

#include <QDockWidget>

class PortsWidget : public MultiInfoWidget {

public:
  explicit PortsWidget(MioMain *parent = 0, Device *device = 0,
                       QString windowTitle = tr("Ports"));
	~PortsWidget();

protected:
	QWidget *createWidget(std::string infoName);
};

#endif // PORTSWIDGET_H
