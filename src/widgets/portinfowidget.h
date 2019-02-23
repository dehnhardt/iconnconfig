#ifndef PORTINFOWIDGET_H
#define PORTINFOWIDGET_H

#include "../sysex/retsetmidiportinfo.h"
#include <QTimer>
#include <QWidget>

namespace Ui {
class PortInfoWidget;
}

class PortInfoWidget : public QWidget {
	Q_OBJECT

  public:
	explicit PortInfoWidget(RetSetMidiPortInfo *portInfo,
							QWidget *parent = nullptr);
	~PortInfoWidget();

  private:
	Ui::PortInfoWidget *ui;
	RetSetMidiPortInfo *m_pMidiPortInfo = nullptr;
	void createConnections();
	QTimer *m_pUpdateTimer = nullptr;

  public slots:
	void portNameChanged();
	void inputStateChanged(int state);
	void outputStateChanged(int state);
	void timerElapsed();

  signals:
	void changePortName(std::string portName);
};

#endif // PORTINFOWIDGET_H
