#ifndef MULTIINFOLISTENTRY_H
#define MULTIINFOLISTENTRY_H

#include <QIcon>
#include <QWidget>

class MultiInfoListEntry {
public:
	enum ListEntryCode {
		NONE = -1,
		SECTION = 0,
		GLOBAL_DEVICE_INFO,
		NETWORK_INFO,
		MIDI_INFO,
		PORT_ROUTING
	};

public:
	MultiInfoListEntry();
	MultiInfoListEntry(ListEntryCode entryCode, std::string name, int index = -1);

	ListEntryCode entryCode = NONE;
	std::string name;
	QIcon icon;
	int index = -1;
	QWidget *widget = 0;
	bool enabled = true;
	bool selectable = true;
};

#endif // MULTIINFOLISTENTRY_H
