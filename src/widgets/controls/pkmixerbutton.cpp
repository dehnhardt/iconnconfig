#include "pkmixerbutton.h"

#include <QPaintEvent>
#include <QPainter>
#include <iostream>
#include <math.h>
#include <sstream>

PKMixerButton::PKMixerButton(QWidget *parent) : QToolButton(parent) {
	setStyleSheet("QToolButton { border: 1px solid #ffffff; border-radius: "
				  "5px;}");
}

void PKMixerButton::setColor(int r, int g, int b) {
	std::stringstream ss;

	ss << "QToolButton { border: 1px solid #aaaaaa; border-radius: 5px; "
		  "background-color: rgb(255,255,255);}";
	ss << "QToolButton:checked { background-color: rgb(" << r << "," << g << ","
	   << b << ");}";
	// std::cout << ss.str() << std::endl;
	setStyleSheet(ss.str().c_str());
}

void PKMixerButton::paintEvent(QPaintEvent *event) {
	QToolButton::paintEvent(event);
}
