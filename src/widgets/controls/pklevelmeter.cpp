#include "pklevelmeter.h"
#include <QStyle>

PKLevelMeter::PKLevelMeter(QWidget *parent) : QProgressBar(parent) {}

void PKLevelMeter::paintEvent(QPaintEvent *event) {
	QProgressBar::paintEvent(event);
}
