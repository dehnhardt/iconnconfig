#ifndef PORTBUTTON_H
#define PORTBUTTON_H

#include "../portdisplayhelper.h"

#include <QToolButton>

class PortButton : public QToolButton {
public:
	PortButton(const long val, const QString m_sIconText, const QString &text,
						 const MidiPortType m_MidiPortType, QWidget *parent = nullptr);

	long getValue() const;
	void setValue(long m_iValue);

protected:
	void paintEvent(QPaintEvent *event);

private:
	long m_iValue = 0;
	QString m_sIconText = 0;
	MidiPortType m_MidiPortType;
	QIcon m_Icon;
};

#endif // PORTBUTTON_H
