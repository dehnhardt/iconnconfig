#include "portbutton.h"
#include "../portdisplayhelper.h"

#include <QPaintEvent>
#include <QStyleOptionToolButton>
#include <QStylePainter>

PortButton::PortButton(const long value, const QString iconText,
											 const QString &text, const MidiPortType midiPortType,
											 QWidget *parent)
		: QToolButton(parent), value(value), iconText(iconText),
			midiPortType(midiPortType) {
	icon = PortDisplayHelper::getPortIcon(midiPortType);
	//	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	setToolButtonStyle(Qt::ToolButtonIconOnly);
	setIcon(icon);
  setText(text);
	setToolTip(text);
  setCheckable(true);
  setMinimumSize(50, 50);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  setIconSize(QSize(35, 35));
}

void PortButton::paintEvent(QPaintEvent *event) {
  QToolButton::paintEvent(event);
	if (iconText == "")
		return;
  QPainter painter(this);
  QRect rect = event->rect();
  QRect rect1 = this->rect();

	int top = 0;

	switch (midiPortType) {
	case DIN:
		top = rect1.top() + 10;
		break;
	case USB_HOST:
		top = rect1.top() + 10;
		break;
	case ETHERNET:
		top = rect1.top() + 5;
		break;
	default:
		top = rect1.top() + 5;
		break;
	}

	QRect iconRect = QRect(rect1.left(), top, rect1.width(), iconSize().height());

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
		painter.drawText(iconRect, Qt::AlignCenter, iconText);
}

long PortButton::getValue() const { return value; }

void PortButton::setValue(long value) { value = value; }
