#ifndef AUDIOROUTINGWIDGET_H
#define AUDIOROUTINGWIDGET_H

#include "../device.h"
#include "../sysex/retsetaudiopatchbayparm.h"

#include <QAbstractTableModel>
#include <QMap>
#include <QWidget>

namespace Ui {
class AudioRoutingWidget;
}

class RoutingTableModel;

class AudioRoutingWidget : public QWidget {
	Q_OBJECT

  public:
	explicit AudioRoutingWidget(Device *device, QWidget *parent = nullptr);
	~AudioRoutingWidget();
	void loadData();

  private:
	Ui::AudioRoutingWidget *ui;
	Device *m_pDevice = nullptr;
	RoutingTableModel *m_pRoutingTableModel = nullptr;
	unsigned int m_iNumberOfAudioPorts = 0;

  private:
};

class RoutingTableModel : public QAbstractTableModel {

	// QAbstractItemModel interface
  public:
	void setAudioPatchbayParm(
		const std::shared_ptr<RetSetAudioPatchbayParm> audioPatchbayParm);
	int rowCount(__attribute__((unused))
				 const QModelIndex &parent) const override {
		return static_cast<int>(m_iRoutingConfigs);
	}
	int columnCount(__attribute__((unused))
					const QModelIndex &parent) const override {
		return static_cast<int>(m_iMaxOutputChannel);
	}
	QVariant data(const QModelIndex &index, int role) const override;

  private:
	QMap<unsigned int, std::shared_ptr<std::vector<AudioPatchbayConfiguration>>>
		m_MapTableData;

	unsigned int m_iMaxInputChannel = 0;
	unsigned int m_iMaxOutputChannel = 0;
	unsigned int m_iRoutingConfigs = 0;
};

#endif // AUDIOROUTINGWIDGET_H
