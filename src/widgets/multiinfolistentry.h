#ifndef MULTIINFOLISTENTRY_H
#define MULTIINFOLISTENTRY_H

#include "../sysex/sysexmessage.h"

#include <QIcon>
#include <QObject>
#include <QWidget>

class QListWidgetItem;

class MultiInfoListEntry : public QObject {
	Q_OBJECT

  public:
	enum ListEntryCode {
		NONE = -1,
		SECTION = 0,
		GLOBAL_DEVICE_INFO,
		NETWORK_INFO,
		MIDI_INFO,
		PORT_ROUTING,
		AUDIO_INFO
	};

  public:
	explicit MultiInfoListEntry(QObject *parent = nullptr);
	MultiInfoListEntry(ListEntryCode entryCode, std::string name,
					   int index = -1, QObject *parent = nullptr);
	virtual ~MultiInfoListEntry();

	ListEntryCode entryCode = NONE;
	std::string name;
	QIcon icon;
	int index = -1;
	QWidget *widget = nullptr;
	bool enabled = true;
	bool selectable = true;
	SysExMessage *message = nullptr;
	QListWidgetItem *m_pListWidgetItem = nullptr;

  public slots:
	void changeName(std::string name);
};

#endif // MULTIINFOLISTENTRY_H
