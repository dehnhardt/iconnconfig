#include "pkmixerbutton.h"

#include <QPaintEvent>
#include <QPainter>
#include <iostream>
#include <sstream>

PKMixerButton::PKMixerButton(QWidget *parent) : QToolButton(parent) {
	setStyleSheet("QToolButton { border: 1px solid #ffffff; border-radius: "
				  "3px;}");
}

void PKMixerButton::setColor(unsigned int r, unsigned int g, unsigned int b) {
	std::stringstream ss;
	ss << "QToolButton { border: 1px solid #ffffff; border-radius: 3px; "
		  "background-color: rgba("
	   << r << "," << g << "," << b << ", 0.1);}";
	ss << "QToolButton:checked { background-color: rgba(" << r << "," << g
	   << "," << b << ", 0.9);}";
	setStyleSheet(ss.str().c_str());
}

void PKMixerButton::paintEvent(QPaintEvent *event) {
	QToolButton::paintEvent(event);
}
