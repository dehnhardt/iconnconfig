#include "midicontrollercombodelegate.h"

#include <QApplication>
#include <QComboBox>
#include <QModelIndex>
#include <QString>
#include <QWidget>

#include <iostream>

MidiControllerComboDelegate::MidiControllerComboDelegate(QObject *parent)
	: QItemDelegate(parent) {
	comboBox = static_cast<QComboBox *>(createCbEditor(nullptr));
}

QWidget *MidiControllerComboDelegate::createEditor(
	QWidget *parent, const QStyleOptionViewItem & /* option */,
	const QModelIndex & /* index */) const {

	std::cout << "create Editor" << std::endl;

	return createCbEditor(parent);
}

void MidiControllerComboDelegate::setEditorData(
	QWidget *editor, const QModelIndex &index) const {
	QComboBox *comboBox = static_cast<QComboBox *>(editor);
	int value = index.model()->data(index, Qt::EditRole).toInt();
	comboBox->setCurrentIndex(comboBox->findData(value));
}

void MidiControllerComboDelegate::setModelData(QWidget *editor,
											   QAbstractItemModel *model,
											   const QModelIndex &index) const {
	QComboBox *comboBox = static_cast<QComboBox *>(editor);
	QVariant cd = comboBox->currentData();
	model->setData(index, comboBox->currentData(), Qt::EditRole);
}

void MidiControllerComboDelegate::updateEditorGeometry(
	QWidget *editor, const QStyleOptionViewItem &option,
	const QModelIndex & /* index */) const {
	editor->setGeometry(option.rect);
}

QWidget *MidiControllerComboDelegate::createCbEditor(QWidget *parent) const {
	QComboBox *editor = new QComboBox(parent);

	editor->addItem("Bank Select", 0);
	editor->addItem("Modulation", 1);
	editor->addItem("Breath Controller", 2);
	editor->addItem("Undefined", 3);
	editor->addItem("Foot Controller", 4);
	editor->addItem("Portamento Time", 5);
	editor->addItem("Data Entry Most Significant Bit(MSB)", 6);
	editor->addItem("Volume", 7);
	editor->addItem("Balance", 8);
	editor->addItem("Undefined", 9);
	editor->addItem("Pan", 10);
	editor->addItem("Expression", 11);
	editor->addItem("Effect Controller 1", 12);
	editor->addItem("Effect Controller 2", 13);
	editor->addItem("Undefined", 14);
	editor->addItem("Undefined", 15);
	editor->addItem("General Purpose", 16);
	editor->addItem("General Purpose", 17);
	editor->addItem("General Purpose", 18);
	editor->addItem("General Purpose", 19);
	editor->addItem("Damper Pedal / Sustain Pedal", 64);
	editor->addItem("Portamento On/Off Switch", 65);
	editor->addItem("Sostenuto On/Off Switch", 66);
	editor->addItem("Soft Pedal On/Off Switch", 67);
	editor->addItem("Legato FootSwitch", 68);
	editor->addItem("Hold 2", 69);
	editor->addItem("Sound Controller 1", 70);
	editor->addItem("Sound Controller 2", 71);
	editor->addItem("Sound Controller 3", 72);
	editor->addItem("Sound Controller 4", 73);
	editor->addItem("Sound Controller 5", 74);
	editor->addItem("Sound Controller 6", 75);
	editor->addItem("Sound Controller 7", 76);
	editor->addItem("Sound Controller 8", 77);
	editor->addItem("Sound Controller 9", 78);
	editor->addItem("Sound Controller 10", 79);
	editor->addItem("General Purpose MIDI CC Controller", 80);
	editor->addItem("General Purpose MIDI CC Controller", 81);
	editor->addItem("General Purpose MIDI CC Controller", 82);
	editor->addItem("General Purpose MIDI CC Controller", 83);
	editor->addItem("Portamento CC Control", 84);
	editor->addItem("Effect 1 Depth", 91);
	editor->addItem("Effect 2 Depth", 92);
	editor->addItem("Effect 3 Depth", 93);
	editor->addItem("Effect 4 Depth", 94);
	editor->addItem("Effect 5 Depth", 95);
	editor->addItem("(+1) Data Increment", 96);
	editor->addItem("(-1) Data Decrement", 97);
	editor->addItem("Non-Registered Parameter Number LSB (NRPN)", 98);
	editor->addItem("Non-Registered Parameter Number MSB (NRPN)", 99);
	editor->addItem("Registered Parameter Number LSB (RPN)", 100);
	editor->addItem("Registered Parameter Number MSB (RPN)", 101);
	editor->addItem("All Sound Off", 120);
	editor->addItem("Reset All Controllers", 121);
	editor->addItem("Local On/Off Switch", 122);
	editor->addItem("All Notes Off", 123);
	editor->addItem("Omni Mode Off", 124);
	editor->addItem("Omni Mode On", 125);
	editor->addItem("Mono Mode", 126);
	editor->addItem("Poly Mode", 127);
	return editor;
}
