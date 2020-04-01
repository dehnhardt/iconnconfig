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

typedef struct {
	AudioPortClass audioPortClass;
	AudioPortId audioPortId;
	AudioChannelId auioChannelId;
} AudioRoutingChannel;

std::shared_ptr<AudioRoutingChannel>
decodeRoutingChannel(AudioPortChannelId audioPortChannelId);

namespace Ui {
class AudioRoutingWidget;
}

class RoutingTableModel;

class AudioRoutingWidget : public QWidget {
	Q_OBJECT

  public:
	explicit AudioRoutingWidget(Device *device, QWidget *parent = nullptr);
	~AudioRoutingWidget() override;
	void loadHeaderStructure();
	void loadTableData();

  public slots:
	bool modelDataChanged(QModelIndex index, AudioPortChannelId source,
						  AudioPortChannelId sink, bool value);
	void needsDataReload();

  private:
	Ui::AudioRoutingWidget *ui;
	Device *m_pDevice = nullptr;
	RoutingTableModel *m_pRoutingTableModel = nullptr;
	unsigned int m_iNumberOfAudioPorts = 0;
	bool m_bDataReload = true;

	std::vector<AudioPortChannelId> m_vColumns;
	std::vector<AudioPortChannelId> m_vRows;
	bool modifyPhysicalPortConnection(
		std::shared_ptr<AudioRoutingChannel> sinkChannel);
	bool modifyMixerSinkConnection(
		std::shared_ptr<AudioRoutingChannel> sourceChannel,
		AudioPortChannelId sink, bool value);
	bool modifyMixerSourceConnection(
		AudioPortChannelId source,
		std::shared_ptr<AudioRoutingChannel> sinkChannel, bool value);

	// QWidget interface
  protected:
	virtual void showEvent(QShowEvent *event) override;
};

class RoutingTableModel : public QAbstractTableModel {
	Q_OBJECT

  public:
	RoutingTableModel(AudioRoutingWidget *audioRoutingWidget,
					  QObject *parent = nullptr);

	void setAudioPatchbayParm(
		const std::shared_ptr<RetSetAudioPatchbayParm> audioPatchbayParm);
	void addAudioPatches(
		std::shared_ptr<
			std::map<AudioPortChannelId, std::map<AudioPortChannelId, bool>>>);
	void setRows(std::vector<AudioPortChannelId> rows) { m_vRows = rows; }
	void setColumns(std::vector<AudioPortChannelId> columns) {
		m_vColumns = columns;
	}
	void cleanData();
	void refreshAll();

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

	AudioRoutingWidget *m_pAudioRoutigWidget = nullptr;

	void eraseColumn(unsigned int column);
	bool checkConnectionValid(AudioPortChannelId source,
							  AudioPortChannelId sink) const;

  signals:
	bool modelDataChanged(QModelIndex index, AudioPortChannelId source,
						  AudioPortChannelId sink, bool value);
};

#endif // AUDIOROUTINGWIDGET_H
