#ifndef PORTFILTERSETTINGSWIDGET_H
#define PORTFILTERSETTINGSWIDGET_H

#include "../sysex/retsetmidiportfilter.h"

#include <QAbstractTableModel>
#include <QCheckBox>
#include <QTableWidget>
#include <QWidget>

namespace Ui {
class PortFilterSettingsWidget;
}

class PortFilterSettingsWidget : public QWidget {
	Q_OBJECT

  public:
	explicit PortFilterSettingsWidget(QWidget *parent = nullptr);
	~PortFilterSettingsWidget();

  public: // getter and setter
	void setMIDISystemMessagesFilter(
		MIDISystemMessagesFilter *midiSystemMessagesFilter);
	void setMidiControllerFilter(MIDIControllerFilter **midiControllerFilter);
	void setMidiChannelMessagesFilter(
		MIDIChannelMessagesFilter **midiChannelMessagesFilter);

	MIDISystemMessagesFilter *getMIDISystemMessagesFilter();
	MIDIPortFilter *getMidiPortFilter();

  signals:
	void filterDataChanged(PortFilterDirection direction);

  private: // Methods
	QTableWidgetItem *getCheckStateItem(bool checked);
	void createConnections();

  private: // Members
	Ui::PortFilterSettingsWidget *ui;
	MIDISystemMessagesFilter *m_pMidiSystemMessagesFilter = nullptr;

  private slots:
	void checkboxUpdated(int state, QCheckBox *checkBox);
};

class MidiControllerFilterTM : public QAbstractTableModel {
  public:
	MidiControllerFilterTM(MIDIControllerFilter **midiControllerFilter);

	int rowCount(const QModelIndex & /* parent */) const override;
	int columnCount(const QModelIndex & /*parent*/) const override;
	QVariant data(const QModelIndex &index,
				  int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
						int role) const override;
	bool setData(const QModelIndex &index, const QVariant &value,
				 int role = Qt::EditRole) override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;

	MIDIControllerFilter **getMidiControllerFilter() {
		return m_ppMidiControllerFilter;
	}

  private:
	MIDIControllerFilter **m_ppMidiControllerFilter;
};

class MidiChannelMessagesFilterTM : public QAbstractTableModel {
  public:
	MidiChannelMessagesFilterTM(
		MIDIChannelMessagesFilter **midiChannelMessagesFilter);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex & /*parent*/) const override;
	QVariant data(const QModelIndex &index,
				  int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
						int role) const override;
	bool setData(const QModelIndex &index, const QVariant &value,
				 int role = Qt::EditRole) override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	MIDIChannelMessagesFilter **getMidiChannelMessagesFilter() {
		return m_ppMidiChannelMessagesFilter;
	}

  private:
	MIDIChannelMessagesFilter **m_ppMidiChannelMessagesFilter;
	Qt::CheckState boolToCheckState(bool value) const;
};
#endif // PORTFILTERSETTINGSWIDGET_H
