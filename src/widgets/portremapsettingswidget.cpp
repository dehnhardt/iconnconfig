#include "portremapsettingswidget.h"
#include "controls/midicontrollercombodelegate.h"
#include "ui_portremapsettingswidget.h"

#include <QLabel>

PortRemapSettingsWidget::PortRemapSettingsWidget(
    PortRemapDirection direction, QWidget *parent)
    : QWidget(parent), ui(new Ui::PortRemapSettingsWidget),
      portRemapDirection(direction) {
	ui->setupUi(this);
	MidiControllerComboDelegate *comboDelegate =
		new MidiControllerComboDelegate();
    ui->m_pTblMidiControllerRemap->setItemDelegateForColumn(0, comboDelegate);
    ui->m_pTblMidiControllerRemap->horizontalHeader()->setSectionResizeMode(
		QHeaderView::ResizeToContents);
    ui->m_pTblMidiControllerRemap->verticalHeader()->setSectionResizeMode(
		QHeaderView::ResizeToContents);
    ui->m_pTblMidiChannelMessageRemap->horizontalHeader()
		->setSectionResizeMode(QHeaderView::ResizeToContents);
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
	int numberOfMidiContollers =
        static_cast<int>(sizeof(*midiControllerRemap));
	for (int i = 0; i < numberOfMidiContollers; i++) {
		QModelIndex modelIndex =
            ui->m_pTblMidiControllerRemap->model()->index(i, 0, QModelIndex());
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

void PortRemapSettingsWidget::createConnections() {
}

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
	return MIDI_CHANNELS + 2;
}

QVariant MidiControllerRemapTM::data(const QModelIndex &index,
									  int role) const {
	int row = index.row();
    MIDIControllerRemap *midiControllerRemap = m_ppMidiControllerRemap[row];
	switch (role) {
	case Qt::DisplayRole:
		if (index.column() == 0) {
            return midiControllerRemap->midiContollerNumber;
		}
		break;
	case Qt::CheckStateRole:
		if (index.column() == 1) {
			for (int channel = 0; channel < MIDI_CHANNELS; channel++) {
                if (!midiControllerRemap->channel[channel])
					return Qt::Unchecked;
			}
			return Qt::Checked;
		}
		if (index.column() > 1)
            return midiControllerRemap->channel[index.column() - 2]
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
			return QString(tr("MIDI-Controller"));
		case 1:
			return QString(tr("all"));
		default:
			return QString::number(section - 1);
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
            midiControllerRemap->midiContollerNumber = value.toUInt();
			emit modelDataChanged();
		}
	}
	if (role == Qt::CheckStateRole) {
		if (index.column() == 1) {
			for (int column = 0; column < MIDI_CHANNELS; column++) {
                midiControllerRemap->channel[column] = value.toBool();
			}
			emit dataChanged(createIndex(index.row(), 2),
							 createIndex(index.row(), MIDI_CHANNELS + 1));
			emit modelDataChanged();
		} else if (index.column() > 1) {
            midiControllerRemap->channel[index.column() - 2] = value.toBool();
			emit dataChanged(createIndex(index.row(), 1),
							 createIndex(index.row(), 1));
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
	return 6;
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
			return QString(tr("MIDI-Channel"));
		case 1:
			return QString(tr("Pitch Bend"));
		case 2:
			return QString(tr("Mono Key Pressure"));
		case 3:
			return QString(tr("Program Change"));
		case 4:
			return QString(tr("Control Change"));
		case 5:
			return QString(tr("Poly Key Pressure"));
		case 6:
			return QString(tr("Note On / Note Off"));
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
	if (role == Qt::CheckStateRole) {
		switch (index.row()) {
		case 0:
            midiChannelMessagesRemap->remapMidiPitchBendEvents =
				value.toBool();
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
            midiChannelMessagesRemap->remapMidiNoteOnOffEvents =
				value.toBool();
			break;
		}
		emit modelDataChanged();
		return true;
	}
	return false;
}

Qt::ItemFlags MidiChannelMessagesRemapTM::flags(const QModelIndex
												 __attribute__((unused)) &
												 index) const {
	return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

Qt::CheckState MidiChannelMessagesRemapTM::boolToCheckState(bool value) const {
	if (value)
		return Qt::Checked;
	else
		return Qt::Unchecked;
}
