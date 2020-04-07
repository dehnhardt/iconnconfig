#include "pkslider.h"
#include "math.h"

#include <QPaintEvent>
#include <QPainter>
#include <QString>
#include <iostream>
#include <unistd.h>

PKSlider::PKSlider(QWidget *parent) : QSlider(parent) {
	connect(this, &QAbstractSlider::valueChanged, this,
			&PKSlider::onValueChange);
	setStyleSheet("/******** Qslider Vertical ********/"

				  "QSlider { padding: 0; margin: 0;}"

				  "QSlider::groove:vertical {"
				  "border: 1px solid #bbb;"
				  "background: grey;"
				  "width: 6px;"
				  "border-radius: 2px;"
				  "margin-right: 1px;"
				  "}"

				  "QSlider::sub-page:vertical {"
				  "background: #DCEAEA;"
				  "border: 1px solid #777;"
				  "width:: 8px;"
				  "border-radius: 4px;"
				  "margin-right: 2px;"
				  "}"

				  "QSlider::add-page:vertical {"
				  "background: #11CC11;"
				  "border: 1px solid #777;"
				  "width: 8px;"
				  "border-radius: 4px;"
				  "margin-right: 2px;"
				  "}"

				  "QSlider::handle:vertical {"
				  "background: #ced7d9;"
				  "border: 1px solid #777;"
				  "height: 10px;"
				  "margin-right: -2px;"
				  "margin-left: -8px;"
				  "margin-top: -1px;"
				  "border-radius: 4px;"
				  "}");
}

PKSlider::PKSlider(QWidget *parent, std::shared_ptr<ScaleCalc> scaleCalc)
	: PKSlider(parent) {
	setScaleCalc(scaleCalc);
}

void PKSlider::setScaleCalc(std::shared_ptr<ScaleCalc> scaleCalc) {
	m_pScaleCalc = scaleCalc;
	setMinimum(static_cast<int>(scaleCalc->minAllowedValue()));
	setMaximum(static_cast<int>(scaleCalc->maxAllowedValue()));
}

void PKSlider::setMinimum(int val) { QSlider::setMinimum(val); }

void PKSlider::setMaximum(int val) { QSlider::setMaximum(val); }

void PKSlider::setValue(int val) { setValue(static_cast<float>(val)); }

void PKSlider::setValue(float val) {
	if (m_pScaleCalc) {
		float calcVal = m_pScaleCalc->encode(val);
		QSlider::setValue(static_cast<int>(calcVal));
	} else {
		QSlider::setValue(static_cast<int>(val));
	}
	std::cout << std::endl;
}

void PKSlider::onValueChange(float val) {
	if (m_pScaleCalc) {
		float decoded = m_pScaleCalc->decode(val);
		valueChanged(static_cast<int>(decoded));
	} else {
		valueChanged(static_cast<int>(val));
	}
}

void PKSlider::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	QPen pen;
	pen.setWidth(1);
	painter.setPen(pen);
	QFont font = painter.font();
	font.setPointSize(m_iFontSize);
	int fontOffsetY = font.pointSize() / 2 + 1;
	painter.setFont(font);
	QRect contents = this->contentsRect();
	contents.setY(contents.y() - 1 + m_iHalfSliderHeight);
	contents.setHeight(contents.height() - m_iHalfSliderHeight);

	paintLegend(painter, fontOffsetY, contents);
	QSlider::paintEvent(event);
}

int PKSlider::getFontSize() const { return m_iFontSize; }

void PKSlider::setFontSize(int iFontSize) { m_iFontSize = iFontSize; }

void PKSlider::paintLegend(QPainter &painter, int fontOffsetY, QRect contents) {

	if (m_pScaleCalc) {
		int xOffsetScale = contents.width() / 2 - 14;
		int xOffsetFont = contents.width() / 2 + 6;
		int margin = 3;
		int max = maximum();
		float range = m_pScaleCalc->decRange();
		float guiScale = (contents.height()) / range;
		if (m_bDebug)
			std::cout << "Max: " << max << ", Height: " << contents.height()
					  << ", guiScale: " << guiScale << std::endl;
		// painter.drawRect(contents);
		for (float val : m_pScaleCalc->vScaleValues()) {
			int dec = static_cast<int>(
				contents.height() - m_pScaleCalc->encodeScale(val) * guiScale +
				margin);
			painter.drawText(xOffsetFont, dec + margin + fontOffsetY,
							 QString::number(static_cast<double>(val)));
			if (!((val < 0) || (val > 0))) {
				QPen p = painter.pen();
				p.setWidth(2);
				painter.setPen(p);
			}
			painter.drawLine(xOffsetScale, dec + margin, xOffsetScale + 5,
							 dec + margin);
			if (!((val < 0) || (val > 0))) {
				QPen p = painter.pen();
				p.setWidth(1);
				painter.setPen(p);
			}
			if (m_bDebug)
				std::cout << "Values float (" << val << "): "
						  << ", pos: " << dec << std::endl;
		}
	}
}

bool PKSlider::getDebug() const { return m_bDebug; }

void PKSlider::setDebug(bool bDebug) { m_bDebug = bDebug; }
