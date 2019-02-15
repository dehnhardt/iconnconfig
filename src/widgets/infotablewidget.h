#ifndef INFOTABLEWIDGET_H
#define INFOTABLEWIDGET_H

#include "../sysex/getinfo.h"
#include "deviceinfowidget.h"

#include <QObject>
#include <QTableWidget>
#include <QWidget>

class InfoTableWidget : public QWidget {
	Q_OBJECT
  public:
	explicit InfoTableWidget(
		QWidget *parent,
		std::map<SysExMessage::DeviceInfoItem, RetSetInfo *> *m_pRetSetInfos);

  public slots:
	void onDeviceInfoChanged(int row, int column);

  signals:
	void deviceInfoChanged(SysExMessage::DeviceInfoItem item,
						   std::string value);

  private:
	void setupTable();
	void setUpTableItems();

  private:
	std::map<SysExMessage::DeviceInfoItem, RetSetInfo *> *m_pRetSetInfos =
		nullptr;
	QTableWidget *m_pTableWidget = nullptr;
};

#endif // INFOTABLEWIDGET_H
