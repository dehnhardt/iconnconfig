#include "pkslider.h"
#include "math.h"

#include <QPaintEvent>
#include <QPainter>
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

void PKSlider::setMinimum(int val) {
	m_iRawMinimum = val;
	if (val < 0) {
		m_scaleSpan = ScaleSpan(m_scaleSpan | ScaleSpan::NEGATIV);
	}
	if ((m_iResolution < 1) || (m_iResolution > 1))
		QSlider::setMinimum(static_cast<int>(val / m_iResolution));
	else {
		QSlider::setMinimum(val);
	}
	if (m_bDebug)
		std::cout << "Minimum: " << std::dec << (val / m_iResolution) << " Raw("
				  << m_iRawMinimum << ")" << std::endl;
}

void PKSlider::setMaximum(int val) {
	m_iRawMaximum = val;
	if ((m_iResolution < 1) || (m_iResolution > 1))
		QSlider::setMaximum(static_cast<int>(val / m_iResolution));
	else {
		QSlider::setMaximum(val);
	}
	if (m_bDebug)
		std::cout << "Maximum: " << std::dec << (val / m_iResolution) << " Raw("
				  << m_iRawMaximum << ")" << std::endl;
}

void PKSlider::setTickInterval(float val) {
	m_iRawTickInterval = val;
	if ((m_iResolution < 1) || (m_iResolution > 1))
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
	double val2 = val;
	if ((m_iResolution < 1) || (m_iResolution > 1)) {
		val2 = val / static_cast<double>(m_iResolution);
		if (m_bDebug)
			std::cout << "value with resolution: " << std::dec << val2
					  << " Raw Value: " << val << std::endl;
	}
	if (m_scaleType == ScaleType::LINEAR) {
		QSlider::setValue(static_cast<int>(val2));
	} else {
		double scale = maximum() - minimum();
		val2 = log10(abs(round(val2)) + 1) * scale / log10(scale);
		if (m_bDebug)
			std::cout << "Log value: " << val2 << std::endl;
		QSlider::setValue(static_cast<int>(val2 * sgn(val)));
	}
}

void PKSlider::onValueChange(int val) {

	float val2 = 0.0;
	if (m_scaleType == ScaleType::LINEAR)
		val2 = val;
	else {
		float scale = maximum() - minimum();
		val2 = static_cast<float>(
			sgn(val) * (pow(10, abs(val) * log10(scale) / scale) - 1));
		// val2 = dbToAmplitude(val2);
	}
	if ((m_iResolution < 1) || (m_iResolution > 1)) {
		if (m_bDebug)
			std::cout << "Value" << std::dec
					  << static_cast<int>(val2 * m_iResolution)
					  << " Raw Value: " << val << std::endl;
		emit valueChanged(static_cast<int>(round(val2 * m_iResolution)));
	} else {
		emit valueChanged(static_cast<int>(round(val2)));
	}
}

void PKSlider::setResulution(float val) {
	if ((m_iResolution > val) || (m_iResolution < val)) {
		if (m_iRawMinimum != 0)
			setMinimum(m_iRawMinimum);
		if (m_iRawMaximum != 0)
			setMaximum(m_iRawMaximum);
		if (m_iRawTickInterval != 0.0f)
			setTickInterval(m_iRawTickInterval);
	}
	m_iResolution = val;
}

void PKSlider::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	QPen pen;
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

	paintLegend(painter, font_y_offset, contents);
	QSlider::paintEvent(event);
}

void PKSlider::paintLegend(QPainter &painter, int offset, QRect contents) {

	int min = minimum();
	int max = maximum();
	int scale = contents.height();

	float ampMax = 0;
	float ampMin = 0;
	float diff = 0;
	if (m_scaleType == ScaleType::DECIBEL) {
		if (max > 0)
			ampMax = 1 / dbToAmplitude(max);
		else {
			ampMax = dbToAmplitude(max);
		}
		if (min > 0)
			ampMin = 1 / dbToAmplitude(min);
		else {
			ampMin = dbToAmplitude(min);
		}
		diff = 2 - (ampMax + ampMin);
	} else {
		ampMax = max;
		ampMin = min;
		diff = max - min;
	}

	if (m_bDebug2)
		std::cout << "Amp-Max2: " << ampMax << ", ampMin2: " << ampMin
				  << " = diff:" << diff << std::endl;

	float factor = scale / diff;

	int posmin = static_cast<int>(ampMin * factor);
	int pos0 = m_scaleType == ScaleType::DECIBEL
				   ? static_cast<int>(dbToAmplitude(0) * factor)
				   : 0;

	if (m_bDebug2)
		std::cout << std::endl
				  << std::endl
				  << "min: " << min << ", max: " << max << ", diff: " << diff
				  << ", pos0: " << pos0 << ", factor: " << factor << std::endl;

	int lastPos = -10;

	for (int i = 0; i > min;) {
		if (m_scaleType == ScaleType::DECIBEL) {
			if (i > -5)
				i -= 1;
			else
				i -= 5;
		} else {
			if (diff > 100) {
				i -= 10;
			} else {
				i -= 5;
			}
		}
		if (i < min)
			i = min;
		float val = m_scaleType == ScaleType::DECIBEL ? dbToAmplitude(i) : i;
		int pos = scale - (static_cast<int>(val * factor) - posmin);
		if ((i == min) || (i == max) ||
			((abs(lastPos - pos) >= m_iMinimalLegendDistance) &&
			 (abs(pos) > m_iMinimalLegendDistance) &&
			 (abs(scale - pos) > m_iMinimalLegendDistance))) {
			lastPos = pos;
			if (m_bDebug2)
				std::cout << "Val(" << i << "): " << val << ", pos: " << pos
						  << std::endl;
			paintLegendLine(painter, contents, offset, i, pos);
		}
	}
	for (int i = 0; i <= max;) {
		float val =
			m_scaleType == ScaleType::DECIBEL ? 1 / dbToAmplitude(i) : i;
		int pos =
			m_scaleType == ScaleType::DECIBEL
				? scale - 2 * pos0 + static_cast<int>(val * factor) + posmin
				: scale - (static_cast<int>(val * factor) - posmin);
		if ((i == min) || (i == max) ||
			((abs(lastPos - pos) >= m_iMinimalLegendDistance) &&
			 (abs(pos) > m_iMinimalLegendDistance) &&
			 (abs(scale - pos) > m_iMinimalLegendDistance))) {
			lastPos = pos;
			if (m_bDebug2)
				std::cout << "Val(" << i << "): " << val << ", pos: " << pos
						  << std::endl;
			paintLegendLine(painter, contents, offset, i, pos);
		}
		if (i >= max)
			break;
		if (m_scaleType == ScaleType::DECIBEL) {
			if (i < 5)
				i += 1;
			else
				i += 5;
		} else {
			if (diff > 100) {
				i += 10;
			} else {
				i += 5;
			}
		}
		if (i > max)
			i = max;
	}
}

void PKSlider::paintLegendLine(QPainter &painter, QRect contents, int offset,
							   int val, int xPos) {
	int x_off = val < 0 ? 0 : 2;
	int yVal = xPos + m_iHalfSliderHeight;
	painter.drawText(contents.right() - 14 + x_off, yVal + offset,
					 QString::number(val));
	painter.drawLine(contents.x(), yVal, contents.x() + 5, yVal);
}

float PKSlider::dbToAmplitude(float db) { return pow(10.0f, db / 20.0f); }

float PKSlider::amplitudeToDb(float amplitude) {
	return 20.0f * log10(amplitude);
}

void PKSlider::setScaleSpan() {
	int iScaleSpan = 0;
	if (minimum() >= 0) {
		iScaleSpan |= ScaleSpan::POSITIV;
	} else {
		iScaleSpan |= ScaleSpan::NEGATIV;
	}
	if (maximum() <= 0) {
		iScaleSpan |= ScaleSpan::NEGATIV;
	} else {
		iScaleSpan |= ScaleSpan::POSITIV;
	}
}

PKSlider::ScaleType PKSlider::getScaleType() const { return m_scaleType; }

void PKSlider::setScaleType(const ScaleType &scaleType) {
	m_scaleType = scaleType;
}

bool PKSlider::getDebug() const { return m_bDebug; }

void PKSlider::setDebug(bool bDebug) { m_bDebug = bDebug; }
