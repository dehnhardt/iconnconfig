#ifndef MULTIINFOLISTENTRY_H
#define MULTIINFOLISTENTRY_H

#include <QWidget>

class MultiInfoListEntry {
public:
	enum ListEntryCode {
		NONE = -1,
		GLOBAL_DEVICE_INFO = 0,
		NETWORK_INFO,
		MIDI_INFO,
		PORT_ROUTING
	};

public:
	MultiInfoListEntry();
	MultiInfoListEntry(ListEntryCode entryCode, std::string name, int index = -1)
			: entryCode(entryCode), name(name), index(index) {}

	ListEntryCode entryCode = NONE;
	std::string name;
	int index = -1;
	QWidget *widget = 0;
};

#endif // MULTIINFOLISTENTRY_H
