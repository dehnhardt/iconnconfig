#ifndef AUDIOROUTINGWIDGET_H
#define AUDIOROUTINGWIDGET_H

#include "../device.h"
#include "../sysex/retsetaudiopatchbayparm.h"
#include "../sysex/retsetmixerportparm.h"
#include "views/hierarchicalheaderview.h"

#include <QAbstractTableModel>
#include <QMap>
#include <QStandardItemModel>
#include <QWidget>

typedef std::map<AudioPortChannelId, bool> MixerSource;
typedef std::map<AudioPortChannelId, MixerSource> MixerSink;

namespace Ui {
class AudioRoutingWidget;
}

class RoutingTableModel;

class AudioRoutingWidget : public QWidget {
	Q_OBJECT

  public:
	explicit AudioRoutingWidget(Device *device, QWidget *parent = nullptr);
	~AudioRoutingWidget();
	void loadHeaderStructure();
	void loadTableData();

  public slots:
	bool modelDataChanged(QModelIndex index, AudioPortChannelId source,
						  AudioPortChannelId sink);

  private:
	Ui::AudioRoutingWidget *ui;
	Device *m_pDevice = nullptr;
	RoutingTableModel *m_pRoutingTableModel = nullptr;
	unsigned int m_iNumberOfAudioPorts = 0;

	std::vector<AudioPortChannelId> m_vColumns;
	std::vector<AudioPortChannelId> m_vRows;
};

class RoutingTableModel : public QAbstractTableModel {
	Q_OBJECT

  public:
	RoutingTableModel(QObject *parent = nullptr);

	void setAudioPatchbayParm(
		const std::shared_ptr<RetSetAudioPatchbayParm> audioPatchbayParm);
	void addAudioPatches(
		std::shared_ptr<
			std::map<AudioPortChannelId, std::map<AudioPortChannelId, bool>>>);
	void setRows(std::vector<AudioPortChannelId> rows) { m_vRows = rows; }
	void setColumns(std::vector<AudioPortChannelId> columns) {
		m_vColumns = columns;
	}

	AudioPortChannelId getRowAudioPortChanneId(unsigned long row) {
		try {
			return m_vRows.at(row);
		} catch (__attribute__((unused)) const std::out_of_range &oor) {
			return 0;
		}
	}

	AudioPortChannelId getColumnAudioPortChanneId(unsigned long column) {
		try {
			return m_vColumns.at(column);
		} catch (__attribute__((unused)) const std::out_of_range &oor) {
			return 0;
		}
	}
	bool getValue(AudioPortChannelId column, AudioPortChannelId row) const;

	std::unique_ptr<QStandardItemModel> m_pHorizontalHeaderItemModel = nullptr;
	std::unique_ptr<QStandardItemModel> m_pVerticalHeaderItemModel = nullptr;

	// QAbstractItemModel interface
  public:
	virtual QVariant data(const QModelIndex &index, int role) const override;
	virtual bool setData(const QModelIndex &index, const QVariant &value,
						 int role) override;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

	virtual int columnCount(__attribute__((unused))
							const QModelIndex &parent) const override {
		return static_cast<int>(m_vColumns.size());
	}

	virtual int rowCount(__attribute__((unused))
						 const QModelIndex &parent) const override {
		return static_cast<int>(m_vRows.size());
	}

  private:
	MixerSink m_MapTableData;
	std::vector<AudioPortChannelId> m_vColumns;
	std::vector<AudioPortChannelId> m_vRows;

	void eraseColumn(unsigned int column);

  signals:
	bool modelDataChanged(QModelIndex index, AudioPortChannelId source,
						  AudioPortChannelId sink);
};

#endif // AUDIOROUTINGWIDGET_H
