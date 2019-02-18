#ifndef MIDICONTROLLERCOMBODELEGATE_H
#define MIDICONTROLLERCOMBODELEGATE_H

#include <QComboBox>
#include <QItemDelegate>
#include <QObject>

#include <string>
#include <vector>

class QModelIndex;
class QWidget;
class QVariant;

class MidiControllerComboDelegate : public QItemDelegate {
  public:
	MidiControllerComboDelegate(QObject *parent = nullptr);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
						  const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
					  const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor,
							  const QStyleOptionViewItem &option,
							  const QModelIndex &index) const;
	QComboBox *comboBox = nullptr;

  private:
	QWidget *createCbEditor(QWidget *parent) const;
};

#endif // MIDICONTROLLERCOMBODELEGATE_H
