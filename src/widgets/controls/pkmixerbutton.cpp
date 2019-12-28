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

	ss << "QToolButton { border: 1px solid #eeeeee; border-radius: 3px; "
		  "background-color: rgb(255,255,255);}";
	ss << "QToolButton:checked { border:none; background: "
		  "qradialgradient(cx:0, cy:0, "
		  "radius: 1, fx:0.5, fy:0.5, stop:0 "
		  "white, stop:1 rgb("
	   << r << "," << g << "," << b << ") ) }";
	setStyleSheet(ss.str().c_str());
}

void PKMixerButton::paintEvent(QPaintEvent *event) {
	QToolButton::paintEvent(event);
}
