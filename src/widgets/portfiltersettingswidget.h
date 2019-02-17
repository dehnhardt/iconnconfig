#ifndef PORTFILTERSETTINGSWIDGET_H
#define PORTFILTERSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class PortFilterSettingsWidget;
}

class PortFilterSettingsWidget : public QWidget
{
	Q_OBJECT

public:
	explicit PortFilterSettingsWidget(QWidget *parent = nullptr);
	~PortFilterSettingsWidget();

private:
	Ui::PortFilterSettingsWidget *ui;
};

#endif // PORTFILTERSETTINGSWIDGET_H
