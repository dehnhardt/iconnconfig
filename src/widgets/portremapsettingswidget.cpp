#include "portremapsettingswidget.h"
#include "controls/midichannelcombodelegate.h"
#include "controls/midicontrollercombodelegate.h"
#include "ui_portremapsettingswidget.h"

#include <QLabel>

PortRemapSettingsWidget::PortRemapSettingsWidget(PortDirection direction,
												 QWidget *parent)
	: QWidget(parent), ui(new Ui::PortRemapSettingsWidget),
	  portRemapDirection(direction) {
	ui->setupUi(this);
	MidiControllerComboDelegate *comboDelegate =
		new MidiControllerComboDelegate();
	MidiChannelComboDelegate *midiChannelDelegate =
		new MidiChannelComboDelegate();
	ui->m_pTblMidiControllerRemap->setItemDelegateForColumn(0, comboDelegate);
	ui->m_pTblMidiControllerRemap->setItemDelegateForColumn(1, comboDelegate);
	ui->m_pTblMidiControllerRemap->horizontalHeader()->setSectionResizeMode(
		QHeaderView::ResizeToContents);
	ui->m_pTblMidiControllerRemap->verticalHeader()->setSectionResizeMode(
		QHeaderView::ResizeToContents);
	ui->m_pTblMidiChannelMessageRemap->setItemDelegateForRow(
		6, midiChannelDelegate);
	ui->m_pTblMidiChannelMessageRemap->horizontalHeader()->setSectionResizeMode(
		QHeaderView::ResizeToContents);
	ui->m_pTblMidiChannelMessageRemap->verticalHeader()->setSectionResizeMode(
		QHeaderView::ResizeToContents);
	createConnections();
}

PortRemapSettingsWidget::~PortRemapSettingsWidget() { delete ui; }

void PortRemapSettingsWidget::setMidiControllerRemap(
	MIDIControllerRemap **midiControllerRemap) {
	MidiControllerRemapTM *midiControllerRemapTM =
		new MidiControllerRemapTM(midiControllerRemap);
	ui->m_pTblMidiControllerRemap->setModel(midiControllerRemapTM);
	int numberOfMidiContollers = static_cast<int>(sizeof(*midiControllerRemap));
	for (int i = 0; i < numberOfMidiContollers; i++) {
		QModelIndex modelIndex =
			ui->m_pTblMidiControllerRemap->model()->index(i, 0, QModelIndex());
		ui->m_pTblMidiControllerRemap->openPersistentEditor(modelIndex);
		modelIndex =
			ui->m_pTblMidiControllerRemap->model()->index(i, 1, QModelIndex());
		ui->m_pTblMidiControllerRemap->openPersistentEditor(modelIndex);
	}
	connect(midiControllerRemapTM, &MidiControllerRemapTM::modelDataChanged,
			this, [=]() { emit remapDataChanged(portRemapDirection); });
}

void PortRemapSettingsWidget::setMidiChannelMessagesRemap(
	MIDIChannelMessagesRemap **midiChannelMessagesRemap) {
	MidiChannelMessagesRemapTM *midiChannelMessagesRemapTM =
		new MidiChannelMessagesRemapTM(midiChannelMessagesRemap);
	ui->m_pTblMidiChannelMessageRemap->setModel(midiChannelMessagesRemapTM);
	for (int i = 0; i < midiChannelMessagesRemapTM->columnCount(
							midiChannelMessagesRemapTM->index(0, 0));
		 i++) {
		int rows = midiChannelMessagesRemapTM->rowCount(
			midiChannelMessagesRemapTM->index(0, 0));
		QModelIndex modelIndex = midiChannelMessagesRemapTM->index(rows - 1, i);
		ui->m_pTblMidiChannelMessageRemap->openPersistentEditor(modelIndex);
	}
	connect(midiChannelMessagesRemapTM,
			&MidiChannelMessagesRemapTM::modelDataChanged, this,
			[=]() { emit remapDataChanged(portRemapDirection); });
}

MIDIPortRemap *PortRemapSettingsWidget::getMidiPortRemap() {
	MIDIPortRemap *Remap = new MIDIPortRemap();
	return Remap;
}

QTableWidgetItem *PortRemapSettingsWidget::getCheckStateItem(bool checked) {
	QTableWidgetItem *item = new QTableWidgetItem();
	item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
	return item;
}

void PortRemapSettingsWidget::createConnections() {}

/* ************************
 * MidiControllerRemapTM *
 **************************/

MidiControllerRemapTM::MidiControllerRemapTM(
	MIDIControllerRemap **midiControllerRemap) {
	this->m_ppMidiControllerRemap = midiControllerRemap;
}

int MidiControllerRemapTM::rowCount(const QModelIndex &parent
									__attribute__((unused))) const {
	return sizeof(m_ppMidiControllerRemap);
}

int MidiControllerRemapTM::columnCount(const QModelIndex &) const {
	return MIDI_CHANNELS + 3;
}

QVariant MidiControllerRemapTM::data(const QModelIndex &index, int role) const {
	int row = index.row();
	MIDIControllerRemap *midiControllerRemap = m_ppMidiControllerRemap[row];
	switch (role) {
	case Qt::DisplayRole:
		if (index.column() == 0) {
			return midiControllerRemap->midiContollerSourceNumber;
		} else if (index.column() == 1) {
			return midiControllerRemap->midiContollerDestinationNumber;
		}
		break;
	case Qt::CheckStateRole:
		if (index.column() == 2) {
			for (int channel = 0; channel < MIDI_CHANNELS; channel++) {
				if (!midiControllerRemap->channel[channel])
					return Qt::Unchecked;
			}
			return Qt::Checked;
		}
		if (index.column() > 2)
			return midiControllerRemap->channel[index.column() - 3]
					   ? Qt::Checked
					   : Qt::Unchecked;
		break;
	}
	return QVariant();
}

QVariant MidiControllerRemapTM::headerData(int section,
										   Qt::Orientation orientation,
										   int role) const {
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		switch (section) {
		case 0:
			return QString(tr("MIDI-Controller Source"));
		case 1:
			return QString(tr("MIDI-Controller Destination"));
		case 2:
			return QString(tr("all"));
		default:
			return QString::number(section - 2);
		}
	}
	if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
		return QString::number(section + 1);
	}
	return QVariant();
}

bool MidiControllerRemapTM::setData(const QModelIndex &index,
									const QVariant &value, int role) {
	MIDIControllerRemap *midiControllerRemap =
		m_ppMidiControllerRemap[index.row()];
	if (role == Qt::EditRole) {
		if (!hasIndex(index.row(), index.column()))
			return false;
		if (index.column() == 0) {
			midiControllerRemap->midiContollerSourceNumber = value.toUInt();
			emit modelDataChanged();
			return true;
		}
		if (index.column() == 1) {
			midiControllerRemap->midiContollerDestinationNumber =
				value.toUInt();
			emit modelDataChanged();
			return true;
		}
	}
	if (role == Qt::CheckStateRole) {
		if (index.column() == 2) {
			for (int column = 0; column < MIDI_CHANNELS; column++) {
				midiControllerRemap->channel[column] = value.toBool();
			}
			emit dataChanged(createIndex(index.row(), 3),
							 createIndex(index.row(), MIDI_CHANNELS + 2));
			emit modelDataChanged();
		} else if (index.column() > 2) {
			midiControllerRemap->channel[index.column() - 3] = value.toBool();
			emit dataChanged(createIndex(index.row(), 2),
							 createIndex(index.row(), 2));
			emit modelDataChanged();
		}
		return true;
	}
	return false;
}

Qt::ItemFlags MidiControllerRemapTM::flags(const QModelIndex &index) const {
	if (index.column() > 0)
		return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled |
			   Qt::ItemIsSelectable;
	return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

/* *****************************
 * MidiChannelMessagesRemapTM *
 *******************************/

MidiChannelMessagesRemapTM::MidiChannelMessagesRemapTM(
	MIDIChannelMessagesRemap **midiChannelMessagesRemap) {
	this->m_ppMidiChannelMessagesRemap = midiChannelMessagesRemap;
}

int MidiChannelMessagesRemapTM::rowCount(const QModelIndex &) const {
	return 7;
}

int MidiChannelMessagesRemapTM::columnCount(const QModelIndex &) const {
	return MIDI_CHANNELS;
}

QVariant MidiChannelMessagesRemapTM::data(const QModelIndex &index,
										  int role) const {
	MIDIChannelMessagesRemap *midiChannelMessagesRemap =
		this->m_ppMidiChannelMessagesRemap[index.column()];
	switch (role) {
	case Qt::CheckStateRole:
		switch (index.row()) {
		case 0:
			return boolToCheckState(
				midiChannelMessagesRemap->remapMidiPitchBendEvents);
		case 1:
			return boolToCheckState(
				midiChannelMessagesRemap->remapMidiChannelPressureEvents);
		case 2:
			return boolToCheckState(
				midiChannelMessagesRemap->remapMidiProgrammChangeEvents);
		case 3:
			return boolToCheckState(
				midiChannelMessagesRemap->remapMidiControlChangeEvents);
		case 4:
			return boolToCheckState(
				midiChannelMessagesRemap->remapMidiPolyKeyPressureEvents);
		case 5:
			return boolToCheckState(
				midiChannelMessagesRemap->remapMidiNoteOnOffEvents);
		default:
			return QVariant();
		}
	case Qt::DisplayRole:
		switch (index.row()) {
		case 6:
			return QVariant(midiChannelMessagesRemap->remapChannel);
		}
	}
	return QVariant();
}

QVariant MidiChannelMessagesRemapTM::headerData(int section,
												Qt::Orientation orientation,
												int role) const {
	if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
		switch (section) {
		case 0:
			return QString(tr("Pitch Bend"));
		case 1:
			return QString(tr("Mono Key Pressure"));
		case 2:
			return QString(tr("Program Change"));
		case 3:
			return QString(tr("Control Change"));
		case 4:
			return QString(tr("Poly Key Pressure"));
		case 5:
			return QString(tr("Note On / Note Off"));
		case 6:
			return QString(tr("Map to Channel"));
		default:
			return QVariant();
		}
	}
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		return QString::number(section + 1);
	}
	return QVariant();
}

bool MidiChannelMessagesRemapTM::setData(const QModelIndex &index,
										 const QVariant &value, int role) {
	MIDIChannelMessagesRemap *midiChannelMessagesRemap =
		this->m_ppMidiChannelMessagesRemap[index.column()];
	switch (role) {
	case Qt::CheckStateRole:
		switch (index.row()) {
		case 0:
			midiChannelMessagesRemap->remapMidiPitchBendEvents = value.toBool();
			break;
		case 1:
			midiChannelMessagesRemap->remapMidiChannelPressureEvents =
				value.toBool();
			break;
		case 2:
			midiChannelMessagesRemap->remapMidiProgrammChangeEvents =
				value.toBool();
			break;
		case 3:
			midiChannelMessagesRemap->remapMidiControlChangeEvents =
				value.toBool();
			break;
		case 4:
			midiChannelMessagesRemap->remapMidiPolyKeyPressureEvents =
				value.toBool();
			break;
		case 5:
			midiChannelMessagesRemap->remapMidiNoteOnOffEvents = value.toBool();
			break;
		}
		emit modelDataChanged();
		return true;
	case Qt::EditRole:
		switch (index.row()) {
		case 6:
			midiChannelMessagesRemap->remapChannel =
				static_cast<unsigned int>(value.toInt());
			break;
		}
		emit modelDataChanged();
		return true;
	}
	return false;
}

Qt::ItemFlags
MidiChannelMessagesRemapTM::flags(const QModelIndex &index) const {
	if (index.row() < 6)
		return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled |
			   Qt::ItemIsSelectable;
	return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

Qt::CheckState MidiChannelMessagesRemapTM::boolToCheckState(bool value) const {
	if (value)
		return Qt::Checked;
	else
		return Qt::Unchecked;
}
