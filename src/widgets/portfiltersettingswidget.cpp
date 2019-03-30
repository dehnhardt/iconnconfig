#include "portfiltersettingswidget.h"
#include "controls/midicontrollercombodelegate.h"
#include "ui_portfiltersettingswidget.h"

#include <QLabel>

PortFilterSettingsWidget::PortFilterSettingsWidget(PortDirection direction,
												   QWidget *parent)
	: QWidget(parent), ui(new Ui::PortFilterSettingsWidget),
	  portFilterDirection(direction) {
	ui->setupUi(this);
	MidiControllerComboDelegate *comboDelegate =
		new MidiControllerComboDelegate();
	ui->m_pTblMidiControllerFilter->setItemDelegateForColumn(0, comboDelegate);
	ui->m_pTblMidiControllerFilter->horizontalHeader()->setSectionResizeMode(
		QHeaderView::ResizeToContents);
	ui->m_pTblMidiControllerFilter->verticalHeader()->setSectionResizeMode(
		QHeaderView::ResizeToContents);
	ui->m_pTblMidiChannelMessageFilter->horizontalHeader()
		->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui->m_pTblMidiChannelMessageFilter->verticalHeader()->setSectionResizeMode(
		QHeaderView::ResizeToContents);
	createConnections();
}

PortFilterSettingsWidget::~PortFilterSettingsWidget() {
	ui->m_pTblMidiChannelMessageFilter->model()->deleteLater();
	delete ui;
}

void PortFilterSettingsWidget::setMIDISystemMessagesFilter(
	MIDISystemMessagesFilter *midiSystemMessagesFilter) {
	this->m_pMidiSystemMessagesFilter = midiSystemMessagesFilter;
	ui->m_pCbFilterMidiActiveSensingEvents->setChecked(
		midiSystemMessagesFilter->filterMidiActiveSensingEvents);
	ui->m_pCbFilterMidiRealtimeEvents->setChecked(
		midiSystemMessagesFilter->filterMidiRealtimeEvents);
	ui->m_pCbFilterMidiResetEvents->setChecked(
		midiSystemMessagesFilter->filterMidiResetEvents);
	ui->m_pCbFilterMidiSongPositionPointer->setChecked(
		midiSystemMessagesFilter->filterMidiSongPositionPointerEvents);
	ui->m_pCbFilterMidiSongSelectEvents->setChecked(
		midiSystemMessagesFilter->filterMidiSongSelectEvents);
	ui->m_pCbFilterMidiSysexEvents->setChecked(
		midiSystemMessagesFilter->filterMidiSysExEvents);
	ui->m_pCbFilterMidiTimeCodeEvents->setChecked(
		midiSystemMessagesFilter->filterMidiTimeCodeEvents);
	ui->m_pCbFilterMidiTuneRequestEvents->setChecked(
		midiSystemMessagesFilter->filterMidiTuneRequestEvents);
}

void PortFilterSettingsWidget::setMidiControllerFilter(
	MIDIControllerFilter **midiControllerFilter) {
	MidiControllerFilterTM *midiControllerFilterTM =
		new MidiControllerFilterTM(midiControllerFilter);
	ui->m_pTblMidiControllerFilter->setModel(midiControllerFilterTM);
	int numberOfMidiContollers =
		static_cast<int>(sizeof(*midiControllerFilter));
	for (int i = 0; i < numberOfMidiContollers; i++) {
		QModelIndex modelIndex =
			ui->m_pTblMidiControllerFilter->model()->index(i, 0, QModelIndex());
		ui->m_pTblMidiControllerFilter->openPersistentEditor(modelIndex);
	}
	connect(midiControllerFilterTM, &MidiControllerFilterTM::modelDataChanged,
			this, [=]() { emit filterDataChanged(portFilterDirection); });
}

void PortFilterSettingsWidget::setMidiChannelMessagesFilter(
	MIDIChannelMessagesFilter **midiChannelMessagesFilter) {
	MidiChannelMessagesFilterTM *midiChannelMessagesFilterTM =
		new MidiChannelMessagesFilterTM(midiChannelMessagesFilter);
	ui->m_pTblMidiChannelMessageFilter->setModel(midiChannelMessagesFilterTM);
	connect(midiChannelMessagesFilterTM,
			&MidiChannelMessagesFilterTM::modelDataChanged, this,
			[=]() { emit filterDataChanged(portFilterDirection); });
}

MIDISystemMessagesFilter *
PortFilterSettingsWidget::getMIDISystemMessagesFilter() {
	MIDISystemMessagesFilter *midiSystemMessagesFilter =
		new MIDISystemMessagesFilter();
	midiSystemMessagesFilter->filterMidiActiveSensingEvents =
		ui->m_pCbFilterMidiActiveSensingEvents->isChecked();
	midiSystemMessagesFilter->filterMidiRealtimeEvents =
		ui->m_pCbFilterMidiRealtimeEvents->isChecked();
	midiSystemMessagesFilter->filterMidiResetEvents =
		ui->m_pCbFilterMidiResetEvents->isChecked();
	midiSystemMessagesFilter->filterMidiSongPositionPointerEvents =
		ui->m_pCbFilterMidiSongPositionPointer->isChecked();
	midiSystemMessagesFilter->filterMidiSongSelectEvents =
		ui->m_pCbFilterMidiSongSelectEvents->isChecked();
	midiSystemMessagesFilter->filterMidiSysExEvents =
		ui->m_pCbFilterMidiSysexEvents->isChecked();
	midiSystemMessagesFilter->filterMidiTimeCodeEvents =
		ui->m_pCbFilterMidiTimeCodeEvents->isChecked();
	midiSystemMessagesFilter->filterMidiTuneRequestEvents =
		ui->m_pCbFilterMidiTuneRequestEvents->isChecked();
	return midiSystemMessagesFilter;
}

MIDIPortFilter *PortFilterSettingsWidget::getMidiPortFilter() {
	MIDIPortFilter *filter = new MIDIPortFilter();
	return filter;
}

QTableWidgetItem *PortFilterSettingsWidget::getCheckStateItem(bool checked) {
	QTableWidgetItem *item = new QTableWidgetItem();
	item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
	return item;
}

void PortFilterSettingsWidget::createConnections() {
	connect(ui->m_pCbFilterMidiResetEvents, &QCheckBox::stateChanged, this,
			[=](int state) {
				checkboxUpdated(state, ui->m_pCbFilterMidiResetEvents);
			});
	connect(ui->m_pCbFilterMidiActiveSensingEvents, &QCheckBox::stateChanged,
			this, [=](int state) {
				checkboxUpdated(state, ui->m_pCbFilterMidiActiveSensingEvents);
			});
	connect(ui->m_pCbFilterMidiRealtimeEvents, &QCheckBox::stateChanged, this,
			[=](int state) {
				checkboxUpdated(state, ui->m_pCbFilterMidiRealtimeEvents);
			});
	connect(ui->m_pCbFilterMidiTuneRequestEvents, &QCheckBox::stateChanged,
			this, [=](int state) {
				checkboxUpdated(state, ui->m_pCbFilterMidiTuneRequestEvents);
			});
	connect(ui->m_pCbFilterMidiSongSelectEvents, &QCheckBox::stateChanged, this,
			[=](int state) {
				checkboxUpdated(state, ui->m_pCbFilterMidiSongSelectEvents);
			});
	connect(ui->m_pCbFilterMidiSongPositionPointer, &QCheckBox::stateChanged,
			this, [=](int state) {
				checkboxUpdated(state, ui->m_pCbFilterMidiSongPositionPointer);
			});
	connect(ui->m_pCbFilterMidiTimeCodeEvents, &QCheckBox::stateChanged, this,
			[=](int state) {
				checkboxUpdated(state, ui->m_pCbFilterMidiTimeCodeEvents);
			});
	connect(ui->m_pCbFilterMidiSysexEvents, &QCheckBox::stateChanged, this,
			[=](int state) {
				checkboxUpdated(state, ui->m_pCbFilterMidiSysexEvents);
			});
}

void PortFilterSettingsWidget::checkboxUpdated(int state, QCheckBox *checkBox) {
	if (checkBox->objectName() == "m_pCbFilterMidiResetEvents") {
		this->m_pMidiSystemMessagesFilter->filterMidiResetEvents = state;
	}
	if (checkBox->objectName() == "m_pCbFilterMidiActiveSensingEvents") {
		this->m_pMidiSystemMessagesFilter->filterMidiActiveSensingEvents =
			state;
	}
	if (checkBox->objectName() == "m_pCbFilterMidiRealtimeEvents") {
		this->m_pMidiSystemMessagesFilter->filterMidiRealtimeEvents = state;
	}
	if (checkBox->objectName() == "m_pCbFilterMidiTuneRequestEvents") {
		this->m_pMidiSystemMessagesFilter->filterMidiTuneRequestEvents = state;
	}
	if (checkBox->objectName() == "m_pCbFilterMidiSongSelectEvents") {
		this->m_pMidiSystemMessagesFilter->filterMidiSongSelectEvents = state;
	}
	if (checkBox->objectName() == "m_pCbFilterMidiSongPositionPointer") {
		this->m_pMidiSystemMessagesFilter->filterMidiSongPositionPointerEvents =
			state;
	}
	if (checkBox->objectName() == "m_pCbFilterMidiTimeCodeEvents") {
		this->m_pMidiSystemMessagesFilter->filterMidiTimeCodeEvents = state;
	}
	if (checkBox->objectName() == "m_pCbFilterMidiSysexEvents") {
		this->m_pMidiSystemMessagesFilter->filterMidiSysExEvents = state;
	}
	emit filterDataChanged(this->portFilterDirection);
}

/* ************************
 * MidiControllerFilterTM *
 **************************/

MidiControllerFilterTM::MidiControllerFilterTM(
	MIDIControllerFilter **midiControllerFilter) {
	this->m_ppMidiControllerFilter = midiControllerFilter;
}

MidiControllerFilterTM::~MidiControllerFilterTM() {
	delete[] m_ppMidiControllerFilter;
}

int MidiControllerFilterTM::rowCount(const QModelIndex &parent
									 __attribute__((unused))) const {
	return sizeof(m_ppMidiControllerFilter);
}

int MidiControllerFilterTM::columnCount(const QModelIndex &) const {
	return MIDI_CHANNELS + 2;
}

QVariant MidiControllerFilterTM::data(const QModelIndex &index,
									  int role) const {
	int row = index.row();
	MIDIControllerFilter *midiControllerFilter = m_ppMidiControllerFilter[row];
	switch (role) {
	case Qt::DisplayRole:
		if (index.column() == 0) {
			std::cout << "Model - controller number: "
					  << midiControllerFilter->midiContollerNumber << std::endl;
			return midiControllerFilter->midiContollerNumber;
		}
		break;
	case Qt::CheckStateRole:
		if (index.column() == 1) {
			for (int channel = 0; channel < MIDI_CHANNELS; channel++) {
				if (!midiControllerFilter->channel[channel])
					return Qt::Unchecked;
			}
			return Qt::Checked;
		}
		if (index.column() > 1)
			return midiControllerFilter->channel[index.column() - 2]
					   ? Qt::Checked
					   : Qt::Unchecked;
		break;
	}
	return QVariant();
}

QVariant MidiControllerFilterTM::headerData(int section,
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

bool MidiControllerFilterTM::setData(const QModelIndex &index,
									 const QVariant &value, int role) {
	MIDIControllerFilter *midiControllerFilter =
		m_ppMidiControllerFilter[index.row()];
	if (role == Qt::EditRole) {
		if (!hasIndex(index.row(), index.column()))
			return false;
		if (index.column() == 0) {
			midiControllerFilter->midiContollerNumber = value.toUInt();
			emit modelDataChanged();
			return true;
		}
	}
	if (role == Qt::CheckStateRole) {
		if (index.column() == 1) {
			for (int column = 0; column < MIDI_CHANNELS; column++) {
				midiControllerFilter->channel[column] = value.toBool();
			}
			emit dataChanged(createIndex(index.row(), 2),
							 createIndex(index.row(), MIDI_CHANNELS + 1));
			emit modelDataChanged();
		} else if (index.column() > 1) {
			midiControllerFilter->channel[index.column() - 2] = value.toBool();
			emit dataChanged(createIndex(index.row(), 1),
							 createIndex(index.row(), 1));
			emit modelDataChanged();
		}
		return true;
	}
	return false;
}

Qt::ItemFlags MidiControllerFilterTM::flags(const QModelIndex &index) const {
	if (index.column() > 0)
		return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled |
			   Qt::ItemIsSelectable;
	return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

/* *****************************
 * MidiChannelMessagesFilterTM *
 *******************************/

MidiChannelMessagesFilterTM::MidiChannelMessagesFilterTM(
	MIDIChannelMessagesFilter **midiChannelMessagesFilter) {
	this->m_ppMidiChannelMessagesFilter = midiChannelMessagesFilter;
}

MidiChannelMessagesFilterTM::~MidiChannelMessagesFilterTM() { deleteLater(); }

int MidiChannelMessagesFilterTM::rowCount(const QModelIndex &) const {
	return 6;
}

int MidiChannelMessagesFilterTM::columnCount(const QModelIndex &) const {
	return MIDI_CHANNELS;
}

QVariant MidiChannelMessagesFilterTM::data(const QModelIndex &index,
										   int role) const {
	MIDIChannelMessagesFilter *midiChannelMessagesFilter =
		this->m_ppMidiChannelMessagesFilter[index.column()];
	switch (role) {
	case Qt::CheckStateRole:
		switch (index.row()) {
		case 0:
			return boolToCheckState(
				midiChannelMessagesFilter->filterMidiPitchBendEvents);
		case 1:
			return boolToCheckState(
				midiChannelMessagesFilter->filterMidiChannelPressureEvents);
		case 2:
			return boolToCheckState(
				midiChannelMessagesFilter->filterMidiProgrammChangeEvents);
		case 3:
			return boolToCheckState(
				midiChannelMessagesFilter->filterMidiControlChangeEvents);
		case 4:
			return boolToCheckState(
				midiChannelMessagesFilter->filterMidiPolyKeyPressureEvents);
		case 5:
			return boolToCheckState(
				midiChannelMessagesFilter->filterMidiNoteOnOffEvents);
		}
	}
	return QVariant();
}

QVariant MidiChannelMessagesFilterTM::headerData(int section,
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
		default:
			return QVariant();
		}
	}
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		return QString::number(section + 1);
	}
	return QVariant();
}

bool MidiChannelMessagesFilterTM::setData(const QModelIndex &index,
										  const QVariant &value, int role) {
	MIDIChannelMessagesFilter *midiChannelMessagesFilter =
		this->m_ppMidiChannelMessagesFilter[index.column()];
	if (role == Qt::CheckStateRole) {
		switch (index.row()) {
		case 0:
			midiChannelMessagesFilter->filterMidiPitchBendEvents =
				value.toBool();
			break;
		case 1:
			midiChannelMessagesFilter->filterMidiChannelPressureEvents =
				value.toBool();
			break;
		case 2:
			midiChannelMessagesFilter->filterMidiProgrammChangeEvents =
				value.toBool();
			break;
		case 3:
			midiChannelMessagesFilter->filterMidiControlChangeEvents =
				value.toBool();
			break;
		case 4:
			midiChannelMessagesFilter->filterMidiPolyKeyPressureEvents =
				value.toBool();
			break;
		case 5:
			midiChannelMessagesFilter->filterMidiNoteOnOffEvents =
				value.toBool();
			break;
		}
		emit modelDataChanged();
		return true;
	}
	return false;
}

Qt::ItemFlags MidiChannelMessagesFilterTM::flags(const QModelIndex
												 __attribute__((unused)) &
												 index) const {
	return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

Qt::CheckState MidiChannelMessagesFilterTM::boolToCheckState(bool value) const {
	if (value)
		return Qt::Checked;
	else
		return Qt::Unchecked;
}
