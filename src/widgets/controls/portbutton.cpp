#include "portbutton.h"
#include "../portdisplayhelper.h"

#include <QPaintEvent>
#include <QStyleOptionToolButton>
#include <QStylePainter>

PortButton::PortButton(const long value, const QString iconText,
					   const QString &text, const MidiPortType midiPortType,
					   QWidget *parent)
	: QToolButton(parent), m_iValue(value), m_sIconText(iconText),
	  m_MidiPortType(midiPortType) {
	m_Icon = PortDisplayHelper::getPortIcon(midiPortType);
	//	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	setToolButtonStyle(Qt::ToolButtonIconOnly);
	setIcon(m_Icon);
	setText(text);
	setToolTip(text);
	setCheckable(true);
	setMinimumSize(35, 35);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	if (m_MidiPortType == USB_DEVICE)
		setIconSize(QSize(32, 35));
	else
		setIconSize(QSize(25, 25));
}

void PortButton::paintEvent(QPaintEvent *event) {
	QToolButton::paintEvent(event);
	if (m_sIconText == "")
		return;
	QPainter painter(this);
	QRect rect = event->rect();
	QRect rect1 = this->rect();

	int top = 0;

	switch (m_MidiPortType) {
	case DIN:
		top = rect1.top() + 6;
		break;
	case USB_HOST:
		top = rect1.top() + 7;
		break;
	case ETHERNET:
		top = rect1.top() + 4;
		break;
	default:
		top = rect1.top() + 1;
		break;
	}

	QRect iconRect =
		QRect(rect1.left(), top, rect1.width(), iconSize().height());

	painter.setRenderHints(QPainter::SmoothPixmapTransform |
						   QPainter::Antialiasing);
	QFont font = painter.font();
	font.setBold(true);
	font.setPointSize(9);
	painter.setFont(font);
	if (isChecked()) {
		QPen penHText(QColor("#66ff66"));
		painter.setPen(penHText);
	} else {
		QPen penHText(QColor("#dd8888"));
		painter.setPen(penHText);
	}
	if (rect1.intersects(rect))
		painter.drawText(iconRect, Qt::AlignCenter, m_sIconText);
}

long PortButton::getValue() const { return m_iValue; }

void PortButton::setValue(long value) { m_iValue = value; }
