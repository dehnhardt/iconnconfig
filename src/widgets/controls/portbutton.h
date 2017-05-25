#ifndef PORTBUTTON_H
#define PORTBUTTON_H

#include <QToolButton>

class PortButton : public QToolButton {
public:
	enum PortType { MIDI, USB_DEVICE, USB_HOTS, ETH };

	PortButton(const long val, const QString &text, const PortType portType,
						 QWidget *parent = Q_NULLPTR);

	long getValue() const;
	void setValue(long value);

protected:
	void paintEvent(QPaintEvent *event);

private:
	long value = 0;
	PortType portType;
	QIcon icon;
};

#endif // PORTBUTTON_H
