#ifndef GLOBALAUDIOCONFIGURATIONWIDGET_H
#define GLOBALAUDIOCONFIGURATIONWIDGET_H

#include "../sysex/retsetaudioglobalparm.h"

#include <QTimer>
#include <QWidget>

namespace Ui {
class GlobalAudioConfiguration;
}

class GlobalAudioConfigurationWidget : public QWidget {
	Q_OBJECT

  public:
	explicit GlobalAudioConfigurationWidget(
		std::shared_ptr<RetSetAudioGlobalParm> m_pRetSetAudioGlobalParam,
		QWidget *parent = nullptr);
	~GlobalAudioConfigurationWidget();

  private:
	QTimer *m_pUpdateTimer = nullptr;
	Ui::GlobalAudioConfiguration *ui;
	std::shared_ptr<RetSetAudioGlobalParm> m_pRetSetAudioGlobalParam;
	void loadData();
	void createConnections();

  private slots:
	void valueEdited(QWidget *widget, QString text);
	void save();

  signals:
	void changeAudioConfiguration();
};

#endif // GLOBALAUDIOCONFIGURATIONWIDGET_H
