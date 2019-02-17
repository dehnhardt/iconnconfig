#include "midicontrollerselect.h"

MIDIControllerSelect::MIDIControllerSelect() { addData(); }

void MIDIControllerSelect::addData() {
	addItem("Bank Select", 0);
	addItem("Modulation", 1);
	addItem("Breath Controller", 2);
	addItem("Undefined", 3);
	addItem("Foot Controller", 4);
	addItem("Portamento Time", 5);
	addItem("Data Entry Most Significant Bit(MSB)", 6);
	addItem("Volume", 7);
	addItem("Balance", 8);
	addItem("Undefined", 9);
	addItem("Pan", 10);
	addItem("Expression", 11);
	addItem("Effect Controller 1", 12);
	addItem("Effect Controller 2", 13);
	addItem("Undefined", 14);
	addItem("Undefined", 15);
	addItem("General Purpose", 16);
	addItem("General Purpose", 17);
	addItem("General Purpose", 18);
	addItem("General Purpose", 19);
	addItem("Damper Pedal / Sustain Pedal", 64);
	addItem("Portamento On/Off Switch", 65);
	addItem("Sostenuto On/Off Switch", 66);
	addItem("Soft Pedal On/Off Switch", 67);
	addItem("Legato FootSwitch", 68);
	addItem("Hold 2", 69);
	addItem("Sound Controller 1", 70);
	addItem("Sound Controller 2", 71);
	addItem("Sound Controller 3", 72);
	addItem("Sound Controller 4", 73);
	addItem("Sound Controller 5", 74);
	addItem("Sound Controller 6", 75);
	addItem("Sound Controller 7", 76);
	addItem("Sound Controller 8", 77);
	addItem("Sound Controller 9", 78);
	addItem("Sound Controller 10", 79);
	addItem("General Purpose MIDI CC Controller", 80);
	addItem("General Purpose MIDI CC Controller", 81);
	addItem("General Purpose MIDI CC Controller", 82);
	addItem("General Purpose MIDI CC Controller", 83);
	addItem("Portamento CC Control", 84);
	addItem("Effect 1 Depth", 91);
	addItem("Effect 2 Depth", 92);
	addItem("Effect 3 Depth", 93);
	addItem("Effect 4 Depth", 94);
	addItem("Effect 5 Depth", 95);
	addItem("(+1) Data Increment", 96);
	addItem("(-1) Data Decrement", 97);
	addItem("Non-Registered Parameter Number LSB (NRPN)", 98);
	addItem("Non-Registered Parameter Number MSB (NRPN)", 99);
	addItem("Registered Parameter Number LSB (RPN)", 100);
	addItem("Registered Parameter Number MSB (RPN)", 101);
	addItem("All Sound Off", 120);
	addItem("Reset All Controllers", 121);
	addItem("Local On/Off Switch", 122);
	addItem("All Notes Off", 123);
	addItem("Omni Mode Off", 124);
	addItem("Omni Mode On", 125);
	addItem("Mono Mode", 126);
	addItem("Poly Mode", 127);
}
