#include "pklevelmeter.h"
#include <QStyle>

PKLevelMeter::PKLevelMeter(QWidget *parent) : QProgressBar(parent) {
	setStyleSheet("QProgressBar::chunk{ margin: 0.5px; "
				  "background-color: #11CC11; "
				  "border-radius: 3px;"
				  "height: 10px; } ");
}
