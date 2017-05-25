#include "portbutton.h"

#include <QPaintEvent>
#include <QStyleOptionToolButton>
#include <QStylePainter>

PortButton::PortButton(const long value, const QString &text,
											 const PortType portType, QWidget *parent)
		: QToolButton(parent), value(value), portType(portType) {
	switch (portType) {
	case MIDI:
		icon = QIcon(":/pixmaps/midijack");
		break;
	default:
		break;
	}
	setIcon(icon);
	setText(text);
	setCheckable(true);
	setMinimumSize(50, 50);
	setIconSize(QSize(35, 35));
}

void PortButton::paintEvent(QPaintEvent *event) {
	QRect rect = event->rect();
	QStylePainter p(this);
	QStyleOptionToolButton opt;
	initStyleOption(&opt);
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform |
												 QPainter::Antialiasing);
	p.drawComplexControl(QStyle::CC_ToolButton, opt);
	QFont font = painter.font();
	font.setBold(true);
	font.setPointSize(10);
	painter.setFont(font);
	if (isChecked()) {
		QPen penHText(QColor("#66ff66"));
		painter.setPen(penHText);
	} else {
		QPen penHText(QColor("#ffffff"));
		painter.setPen(penHText);
	}
	painter.setRenderHint(QPainter::Antialiasing);
	painter.drawText(rect, Qt::AlignCenter, opt.text);
}

long PortButton::getValue() const { return value; }

void PortButton::setValue(long value) { value = value; }
