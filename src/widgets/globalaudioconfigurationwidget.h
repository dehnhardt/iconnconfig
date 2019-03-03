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
		RetSetAudioGlobalParm *m_pRetSetAudioGlobalParam,
		QWidget *parent = nullptr);
	~GlobalAudioConfigurationWidget();

  private:
	QTimer *m_pUpdateTimer = nullptr;
	Ui::GlobalAudioConfiguration *ui;
	RetSetAudioGlobalParm *m_pRetSetAudioGlobalParam = nullptr;
	void loadData();
	void createConnections();

  private slots:
	void valueEdited(QWidget *widget, QString text);
	void save();

  signals:
	void changeAudioConfiguration();
};

#endif // GLOBALAUDIOCONFIGURATIONWIDGET_H
