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
	if (m_iResolution > 0)
		QSlider::setMinimum(val / m_iResolution);
	else {
		QSlider::setMinimum(val);
	}
	if (m_bDebug)
		std::cout << "Minimum: " << std::dec << (val / m_iResolution) << " Raw("
				  << m_iRawMinimum << ")" << std::endl;
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
	double val2 =
		m_iResolution ? val / static_cast<double>(m_iResolution) : val;
	if (m_iResolution) {
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

	double val2 = 0.0;
	if (m_scaleType == ScaleType::LINEAR)
		val2 = val;
	else {
		double scale = maximum() - minimum();
		val2 = sgn(val) * (pow(10, abs(val) * log10(scale) / scale) - 1);
	}
	if (m_iResolution) {
		if (m_bDebug)
			std::cout << "Value" << std::dec << val2 * m_iResolution
					  << " Raw Value: " << val << std::endl;
		emit valueChanged(static_cast<int>(round(val2 * m_iResolution)));
	} else {
		emit valueChanged(static_cast<int>(round(val2)));
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
	int scale = contents.height();

	int min = minimum();
	int max = maximum();

	int minimalDistance = 10;

	int diff = max - min;

	double factor =
		m_scaleType == ScaleType::LINEAR ? scale / diff : scale / log10(diff);

	double wScaleLast = 0;
	for (int val = min; val <= max;) {
		int wScale = 0;
		if (m_scaleType == ScaleType::LINEAR) {
			wScale = static_cast<int>(abs(val) * factor);
		} else {
			wScale = static_cast<int>(log10(abs(val) + 1) * factor);
		}

		int x_off = val < 0 ? 0 : 2;

		if ((val == min) || (abs(wScale - wScaleLast) >= minimalDistance) ||
			(val == max)) {
			wScaleLast = wScale;
			int yVal = wScale + m_iHalfSliderHeight;
			painter.drawText(contents.right() - 14 + x_off, yVal + offset,
							 QString::number(val));
			painter.drawLine(contents.x(), yVal, contents.x() + 5, yVal);
		}
		if (val < 10 && val > -11)
			val += 1;
		else {
			val += 10;
		}
	}
}

float PKSlider::dbToAmplitude(float db) { return pow(10.0f, db / 20.0f); }

float PKSlider::amplitudeToDb(float amplitude) {
	return 20.0f * log10(amplitude);
}

PKSlider::ScaleType PKSlider::getScaleType() const { return m_scaleType; }

void PKSlider::setScaleType(const ScaleType &scaleType) {
	m_scaleType = scaleType;
}

bool PKSlider::getDebug() const { return m_bDebug; }

void PKSlider::setDebug(bool bDebug) { m_bDebug = bDebug; }
