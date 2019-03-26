#include "multiinfolistentry.h"

#include <QListWidgetItem>

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

MultiInfoListEntry::~MultiInfoListEntry() {
	/*	if (widget != nullptr)
			delete widget;*/
	/* if (message != nullptr)
		delete message; */
	if (m_pListWidgetItem != nullptr)
		delete m_pListWidgetItem;
}

void MultiInfoListEntry::changeName(std::string name) {
	std::cout << "Change name to " << name << std::endl;
	this->name = name;
	if (m_pListWidgetItem != nullptr)
		m_pListWidgetItem->setText(QString(name.c_str()));
}
