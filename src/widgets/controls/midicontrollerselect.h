#ifndef MIDICONTROLLERSELECT_H
#define MIDICONTROLLERSELECT_H

#include <QComboBox>
#include <QObject>

class MIDIControllerSelect : public QComboBox {
  public:
	MIDIControllerSelect();

  private:
	void addData();
};

#endif // MIDICONTROLLERSELECT_H
