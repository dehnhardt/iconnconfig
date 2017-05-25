#ifndef PORTROUTINGWIDGET_H
#define PORTROUTINGWIDGET_H

#include "controls/portbutton.h"

#include <QGridLayout>
#include <QWidget>

class PortRoutingWidget : public QWidget {
	Q_OBJECT
public:
	explicit PortRoutingWidget(QWidget *parent = 0, int numDinPorts = 0);

signals:

public slots:

private:
	// members
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
};

#endif // PORTROUTINGWIDGET_H
