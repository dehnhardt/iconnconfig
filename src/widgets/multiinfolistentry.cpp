#include "multiinfolistentry.h"

MultiInfoListEntry::MultiInfoListEntry(QObject *parent) : QObject(parent) {}

MultiInfoListEntry::MultiInfoListEntry(
	MultiInfoListEntry::ListEntryCode entryCode, std::string name, int index,
	QObject *parent)
	: QObject(parent), entryCode(entryCode), name(name), index(index) {
	icon = QIcon();
	if (entryCode == SECTION) {
		enabled = false;
		selectable = false;
	}
}

MultiInfoListEntry::~MultiInfoListEntry() {}

void MultiInfoListEntry::changeName(std::string name) {
	std::cout << "Change name to " << name << std::endl;
	this->name = name;
}
