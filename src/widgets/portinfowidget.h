#ifndef PORTINFOWIDGET_H
#define PORTINFOWIDGET_H

#include "../sysex/retsetmidiportinfo.h"
#include <QRegExpValidator>
#include <QTimer>
#include <QWidget>

namespace Ui {
class PortInfoWidget;
}

class PortInfoWidget : public QWidget {
	Q_OBJECT

  public:
	explicit PortInfoWidget(std::shared_ptr<RetSetMidiPortInfo> portInfo,
							QWidget *parent = nullptr);
	~PortInfoWidget();

  private:
	Ui::PortInfoWidget *ui;
	std::shared_ptr<RetSetMidiPortInfo> m_pMidiPortInfo;
	void createConnections();
	QTimer *m_pUpdateTimer = nullptr;
	QRegExpValidator *m_pRegExValidator = nullptr;

  public slots:
	void portNameChanged();
	void inputStateChanged(int state);
	void outputStateChanged(int state);
	void timerElapsed();

  signals:
	void changePortName(std::string portName);
};

#endif // PORTINFOWIDGET_H
