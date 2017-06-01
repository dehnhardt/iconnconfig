#ifndef PORTBUTTON_H
#define PORTBUTTON_H

#include "../portdisplayhelper.h"

#include <QToolButton>

class PortButton : public QToolButton {
public:
	PortButton(const long val, const QString iconText, const QString &text,
						 const MidiPortType midiPortType, QWidget *parent = Q_NULLPTR);

	long getValue() const;
	void setValue(long value);

protected:
	void paintEvent(QPaintEvent *event);

private:
	long value = 0;
	QString iconText = 0;
	MidiPortType midiPortType;
	QIcon icon;
};

#endif // PORTBUTTON_H
