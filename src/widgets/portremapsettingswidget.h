#ifndef PORTREMAPSETTINGSWIDGET_H
#define PORTREMAPSETTINGSWIDGET_H

#include "../sysex/retsetmidiportremap.h"

#include <QAbstractTableModel>
#include <QCheckBox>
#include <QTableWidget>
#include <QWidget>

namespace Ui {
class PortRemapSettingsWidget;
}

class PortRemapSettingsWidget : public QWidget {
	Q_OBJECT

  public:
    explicit PortRemapSettingsWidget(PortDirection portRemapDirection,
									  QWidget *parent = nullptr);
    ~PortRemapSettingsWidget();

  public: // getter and setter
    void setMidiControllerRemap(MIDIControllerRemap **midiControllerRemap);
    void setMidiChannelMessagesRemap(
        MIDIChannelMessagesRemap **midiChannelMessagesRemap);

    MIDIPortRemap *getMidiPortRemap();

  signals:
    void remapDataChanged(PortDirection portRemapDirection);

  private: // Methods
	QTableWidgetItem *getCheckStateItem(bool checked);
	void createConnections();

  private: // Members
    Ui::PortRemapSettingsWidget *ui;
    PortDirection portRemapDirection;

};

/* ************************
 * MidiControllerRemapTM *
 **************************/

class MidiControllerRemapTM : public QAbstractTableModel {
	Q_OBJECT
  public:
    MidiControllerRemapTM(MIDIControllerRemap **midiControllerRemap);

	int rowCount(const QModelIndex & /* parent */) const override;
	int columnCount(const QModelIndex & /*parent*/) const override;
	QVariant data(const QModelIndex &index,
				  int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
						int role) const override;
	bool setData(const QModelIndex &index, const QVariant &value,
				 int role = Qt::EditRole) override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;

    MIDIControllerRemap **getMidiControllerRemap() {
        return m_ppMidiControllerRemap;
	}

  signals:
	void modelDataChanged();

  private:
    MIDIControllerRemap **m_ppMidiControllerRemap;
};

/* *****************************
 * MidiChannelMessagesRemapTM *
 *******************************/

class MidiChannelMessagesRemapTM : public QAbstractTableModel {
	Q_OBJECT
  public:
    MidiChannelMessagesRemapTM(
        MIDIChannelMessagesRemap **midiChannelMessagesRemap);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex & /*parent*/) const override;
	QVariant data(const QModelIndex &index,
				  int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
						int role) const override;
	bool setData(const QModelIndex &index, const QVariant &value,
				 int role = Qt::EditRole) override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
    MIDIChannelMessagesRemap **getMidiChannelMessagesRemap() {
        return m_ppMidiChannelMessagesRemap;
	}

  signals:
	void modelDataChanged();

  private:
    MIDIChannelMessagesRemap **m_ppMidiChannelMessagesRemap;
	Qt::CheckState boolToCheckState(bool value) const;
};
#endif // PORTREMAPSETTINGSWIDGET_H
