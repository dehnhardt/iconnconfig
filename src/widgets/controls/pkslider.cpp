#include "pkslider.h"
#include "math.h"

#include <QPaintEvent>
#include <QPainter>
#include <iostream>
#include <unistd.h>

PKSlider::PKSlider(QWidget *parent) : QSlider(parent) {
	connect(this, &QAbstractSlider::valueChanged, this,
			&PKSlider::onValueChange);
}

void PKSlider::setMinimum(int val) {
	m_iRawMinimum = val;
	if (m_iResolution > 0)
		QSlider::setMinimum(val / m_iResolution);
	else {
		QSlider::setMinimum(val);
	}
	if (m_bDebug)
		std::cout << "Minimum: " << std::dec << (val / m_iResolution) << " Raw("
				  << m_iRawMinimum << ")" << std::endl;
	calculateScale();
}

void PKSlider::setMaximum(int val) {
	m_iRawMaximum = val;
	if (m_iResolution > 0)
		QSlider::setMaximum(val / m_iResolution);
	else {
		QSlider::setMaximum(val);
	}
	if (m_bDebug)
		std::cout << "Maximum: " << std::dec << (val / m_iResolution) << " Raw("
				  << m_iRawMaximum << ")" << std::endl;
	calculateScale();
}

void PKSlider::setTickInterval(float val) {
	m_iRawTickInterval = val;
	if (m_iResolution > 0)
		QSlider::setTickInterval(static_cast<int>(val * m_iResolution));
	else {
		QSlider::setTickInterval(static_cast<int>(val));
	}
	if (m_bDebug) {
		std::cout.precision(10);
		std::cout << "Tick Interval: " << std::fixed << (val * m_iResolution)
				  << " Raw(" << m_iRawTickInterval << ")" << std::endl;
	}
}

void PKSlider::setValue(int val) {
	if (m_iResolution) {
		if (m_bDebug)
			std::cout << "value with resolution: " << val / m_iResolution
					  << " Raw Value: " << val << std::endl;
		QSlider::setValue(val / m_iResolution);
	} else {
		QSlider::setValue(val);
	}
}

void PKSlider::onValueChange(int val) {
	if (m_iResolution) {
		if (m_bDebug)
			std::cout << "value with resolution: " << val * m_iResolution
					  << " Raw Value: " << val << std::endl;
		emit valueChanged(val * m_iResolution);
	} else {
		emit valueChanged(val);
	}
}

void PKSlider::setResulution(int val) {
	if (m_iResolution != val) {
		if (m_iRawMinimum != 0)
			setMinimum(m_iRawMinimum);
		if (m_iRawMaximum != 0)
			setMaximum(m_iRawMaximum);
		if (m_iRawTickInterval != 0.0f)
			setTickInterval(m_iRawTickInterval);
	}
	m_iResolution = val;
}

void PKSlider::paintLegendStep(QPainter &painter, int minVal, int val,
							   int offset, QRect contents) {
	int x_off = val < 0 ? 0 : 2;

	int y_val =
		(contents.height() + 2 * m_iHalfSliderHeight) -
		((val - minVal) * contents.height() / m_iScaleDiff + contents.y());
	painter.drawText(contents.right() - 14 + x_off, y_val + offset,
					 QString::number(val));
	painter.drawLine(contents.x(), y_val, contents.x() + 5, y_val);
}

void PKSlider::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	QPen pen;
	int minVal = minimum();
	pen.setWidth(1);
	pen.setColor(Qt::black);

	painter.setPen(pen);
	QFont font("Times", 6);
	int font_y_offset = font.pointSize() / 2;
	painter.setFont(font);
	QRect contents = this->contentsRect();
	// correct the size of the handle in the conents rect
	contents.setY(contents.y() + m_iHalfSliderHeight);
	contents.setHeight(contents.height() - m_iHalfSliderHeight);

	for (int i = 0; i <= m_iScaleTicks; i++) {
		int val = m_pScales[i];
		paintLegendStep(painter, minVal, val, font_y_offset, contents);
	}
	QSlider::paintEvent(event);
}

float PKSlider::dbToAmplitude(float db) { return pow(10.0f, db / 20.0f); }

float PKSlider::amplitudeToDb(float amplitude) {
	return 20.0f * log10(amplitude);
}

void PKSlider::calculateScale() {
	m_iScaleDiff = maximum() - minimum();
	if (m_pScales)
		delete[] m_pScales;
	m_pScales = new int[m_iScaleTicks + 1];
	int maxValue = maximum();
	int minValue = minimum();
	int diff = maxValue - minValue;
	int step = diff / m_iScaleTicks;
	for (int i = 0; i < m_iScaleTicks; i++) {
		m_pScales[i] = step * i + minValue;
		std::cout << "Step " << i << ": " << m_pScales[i] << std::endl;
	}
	m_pScales[m_iScaleTicks] = maxValue;
	if (m_bDebug)
		std::cout << "Step " << m_iScaleTicks << ": " << maxValue << std::endl;
}

int PKSlider::translate(float dec, int x1, int height) {
	int offset = static_cast<int>(height - (x1 + height * dec / 1000));
	// std::cout << "Dec: " << dec << " Scale x: " << offset << std::endl;
	return offset;
}

PKSlider::ScaleType PKSlider::getScaleType() const { return m_scaleType; }

void PKSlider::setScaleType(const ScaleType &scaleType) {
	m_scaleType = scaleType;
}

bool PKSlider::getDebug() const { return m_bDebug; }

void PKSlider::setDebug(bool bDebug) { m_bDebug = bDebug; }
