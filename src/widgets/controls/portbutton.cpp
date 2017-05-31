#include "portbutton.h"
#include "../portdisplayhelper.h"

#include <QPaintEvent>
#include <QStyleOptionToolButton>
#include <QStylePainter>

PortButton::PortButton(const long value, const QString &text,
											 const MidiPortType midiPortType, QWidget *parent)
		: QToolButton(parent), value(value), midiPortType(midiPortType) {
	icon = PortDisplayHelper::getPortIcon(midiPortType);
  setIcon(icon);
  setText(text);
  setCheckable(true);
  setMinimumSize(50, 50);
  setIconSize(QSize(35, 35));
}

void PortButton::paintEvent(QPaintEvent *event) {
  QToolButton::paintEvent(event);

  QPainter painter(this);
  QRect rect = event->rect();
  QRect rect1 = this->rect();

  painter.setRenderHints(QPainter::SmoothPixmapTransform |
                         QPainter::Antialiasing);
  QFont font = painter.font();
  font.setBold(true);
  font.setPointSize(12);
  painter.setFont(font);
  if (isChecked()) {
    QPen penHText(QColor("#66ff66"));
    painter.setPen(penHText);
  } else {
    QPen penHText(QColor("#cccccc"));
    painter.setPen(penHText);
  }
  if (rect1.intersects(rect))
    painter.drawText(rect1, Qt::AlignCenter, this->text());
}

long PortButton::getValue() const { return value; }

void PortButton::setValue(long value) { value = value; }
