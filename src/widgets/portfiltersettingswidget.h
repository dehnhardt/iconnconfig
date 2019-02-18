#ifndef PORTFILTERSETTINGSWIDGET_H
#define PORTFILTERSETTINGSWIDGET_H

#include "../sysex/retsetmidiportfilter.h"

#include <QAbstractTableModel>
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
	MIDIControllerFilter **getMidiControllerFiler();

  private: // Methods
	QTableWidgetItem *getCheckStateItem(bool checked);

  private: // Members
	Ui::PortFilterSettingsWidget *ui;
};

class MidiControllerFilterTM : public QAbstractTableModel {
  public:
	MidiControllerFilterTM(MIDIControllerFilter **midiControllerFilter);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex & /*parent*/) const override;
	QVariant data(const QModelIndex &index,
				  int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
						int role) const override;
	bool setData(const QModelIndex &index, const QVariant &value,
				 int role = Qt::EditRole) override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;

  private:
	MIDIControllerFilter **m_ppMidiControllerFilter;
};

#endif // PORTFILTERSETTINGSWIDGET_H
