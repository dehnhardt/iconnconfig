#include "midichannelcombodelegate.h"
#include "../../sysex/midi.h"

MidiChannelComboDelegate::MidiChannelComboDelegate(QObject *parent)
	: QStyledItemDelegate(parent) {}

QWidget *MidiChannelComboDelegate::createEditor(
	QWidget *parent, const QStyleOptionViewItem & /* option */,
	const QModelIndex & /* index */) const {

	QComboBox *comboBox = createCbEditor(parent);
	connect(comboBox, QOverload<int>::of(&QComboBox::activated), this,
			&MidiChannelComboDelegate::commitAndCloseEditor);
	return comboBox;
}

void MidiChannelComboDelegate::setEditorData(QWidget *editor,
											 const QModelIndex &index) const {
	QComboBox *comboBox = static_cast<QComboBox *>(editor);
	int value = index.model()->data(index, Qt::DisplayRole).toInt();
	comboBox->setCurrentIndex(value - 1);
}

void MidiChannelComboDelegate::setModelData(QWidget *editor,
											QAbstractItemModel *model,
											const QModelIndex &index) const {
	QComboBox *comboBox = static_cast<QComboBox *>(editor);
	QVariant cd = comboBox->currentData();
	model->setData(index, comboBox->currentIndex(), Qt::EditRole);
}

void MidiChannelComboDelegate::updateEditorGeometry(
	QWidget *editor, const QStyleOptionViewItem &option,
	const QModelIndex & /* index */) const {
	editor->setGeometry(option.rect);
}

void MidiChannelComboDelegate::commitAndCloseEditor() {
	QComboBox *editor = qobject_cast<QComboBox *>(sender());
	emit commitData(editor);
	emit closeEditor(editor);
}

QComboBox *MidiChannelComboDelegate::createCbEditor(QWidget *parent) const {
	QComboBox *editor = new QComboBox(parent);
	for (int i = 1; i <= MIDI_CHANNELS; i++) {
		editor->addItem(QString::number(i));
	}
	return editor;
}
