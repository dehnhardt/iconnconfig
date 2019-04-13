#include "pklevelmeter.h"
#include <QStyle>

PKLevelMeter::PKLevelMeter(QWidget *parent) : QProgressBar(parent) {
	setStyleSheet("QProgressBar::chunk{margin: 0.5px; "
				  "background-color: #11CC11; "
				  "height: 10px; } ");
}

void PKLevelMeter::paintEvent(QPaintEvent *event) {
	QProgressBar::paintEvent(event);
}
