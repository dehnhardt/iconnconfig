#include "midicontrollercombodelegate.h"

#include <QApplication>
#include <QComboBox>
#include <QModelIndex>
#include <QString>
#include <QWidget>

#include <iostream>

MidiControllerComboDelegate::MidiControllerComboDelegate(QObject *parent)
	: QStyledItemDelegate(parent) {
	hashedData.insert(0, "Bank Select");
	hashedData.insert(1, "Modulation");
	hashedData.insert(2, "Breath Controller");
	hashedData.insert(3, "Undefined");
	hashedData.insert(4, "Foot Controller");
	hashedData.insert(5, "Portamento Time");
	hashedData.insert(6, "Data Entry Most Significant Bit(MSB)");
	hashedData.insert(7, "Volume");
	hashedData.insert(8, "Balance");
	hashedData.insert(9, "Undefined");
	hashedData.insert(10, "Pan");
	hashedData.insert(11, "Expression");
	hashedData.insert(12, "Effect Controller 1");
	hashedData.insert(13, "Effect Controller 2");
	hashedData.insert(14, "Undefined");
	hashedData.insert(15, "Undefined");
	hashedData.insert(16, "General Purpose");
	hashedData.insert(17, "General Purpose");
	hashedData.insert(18, "General Purpose");
	hashedData.insert(19, "General Purpose");
	hashedData.insert(64, "Damper Pedal / Sustain Pedal");
	hashedData.insert(65, "Portamento On/Off Switch");
	hashedData.insert(66, "Sostenuto On/Off Switch");
	hashedData.insert(67, "Soft Pedal On/Off Switch");
	hashedData.insert(68, "Legato FootSwitch");
	hashedData.insert(69, "Hold 2");
	hashedData.insert(70, "Sound Controller 1");
	hashedData.insert(71, "Sound Controller 2");
	hashedData.insert(72, "Sound Controller 3");
	hashedData.insert(73, "Sound Controller 4");
	hashedData.insert(74, "Sound Controller 5");
	hashedData.insert(75, "Sound Controller 6");
	hashedData.insert(76, "Sound Controller 7");
	hashedData.insert(77, "Sound Controller 8");
	hashedData.insert(78, "Sound Controller 9");
	hashedData.insert(79, "Sound Controller 10");
	hashedData.insert(80, "General Purpose MIDI CC Controller");
	hashedData.insert(81, "General Purpose MIDI CC Controller");
	hashedData.insert(82, "General Purpose MIDI CC Controller");
	hashedData.insert(83, "General Purpose MIDI CC Controller");
	hashedData.insert(84, "Portamento CC Control");
	hashedData.insert(91, "Effect 1 Depth");
	hashedData.insert(92, "Effect 2 Depth");
	hashedData.insert(93, "Effect 3 Depth");
	hashedData.insert(94, "Effect 4 Depth");
	hashedData.insert(95, "Effect 5 Depth");
	hashedData.insert(96, "(+1) Data Increment");
	hashedData.insert(97, "(-1) Data Decrement");
	hashedData.insert(98, "Non-Registered Param. Num. LSB (NRPN)");
	hashedData.insert(99, "Non-Registered Param. Num. MSB (NRPN)");
	hashedData.insert(100, "Registered Param. Num. LSB (RPN)");
	hashedData.insert(101, "Registered Param. Num. MSB (RPN)");
	hashedData.insert(120, "All Sound Off");
	hashedData.insert(121, "Reset All Controllers");
	hashedData.insert(122, "Local On/Off Switch");
	hashedData.insert(123, "All Notes Off");
	hashedData.insert(124, "Omni Mode Off");
	hashedData.insert(125, "Omni Mode On");
	hashedData.insert(126, "Mono Mode");
	hashedData.insert(127, "Poly Mode");
}

QWidget *MidiControllerComboDelegate::createEditor(
	QWidget *parent, const QStyleOptionViewItem & /* option */,
	const QModelIndex & /* index */) const {

	std::cout << "create Editor" << std::endl;
	QComboBox *comboBox = createCbEditor(parent);
	connect(comboBox, QOverload<int>::of(&QComboBox::activated), this,
			&MidiControllerComboDelegate::commitAndCloseEditor);
	return comboBox;
}

void MidiControllerComboDelegate::setEditorData(
	QWidget *editor, const QModelIndex &index) const {
	QComboBox *comboBox = static_cast<QComboBox *>(editor);
	int value = index.model()->data(index, Qt::DisplayRole).toInt();
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

void MidiControllerComboDelegate::commitAndCloseEditor() {
	QComboBox *editor = qobject_cast<QComboBox *>(sender());
	emit commitData(editor);
	emit closeEditor(editor);
}

QComboBox *MidiControllerComboDelegate::createCbEditor(QWidget *parent) const {
	QComboBox *editor = new QComboBox(parent);
	QMap<int, QString>::const_iterator it;
	for (it = hashedData.begin(); it != hashedData.end(); ++it) {
		editor->addItem(it.value(), it.key());
	}
	return editor;
}
