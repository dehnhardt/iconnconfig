#ifndef PROGRESSEVENT_H
#define PROGRESSEVENT_H

#include <QEvent>

#define PROGRESS_EVENT_TYPE 2000

class ProgressEvent : public QEvent {

public:
	ProgressEvent() : QEvent(static_cast<QEvent::Type>(PROGRESS_EVENT_TYPE)) {}

private:
	unsigned int value = 0;

public:
	static const QEvent::Type myType =
		static_cast<QEvent::Type>(PROGRESS_EVENT_TYPE);

	void setValue(unsigned int value) { this->value = value; }
	unsigned int getValue(void) { return value; }
};

#endif// PROGRESSEVENT_H
