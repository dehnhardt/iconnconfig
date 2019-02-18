#ifndef MIDICONTROLLERCOMBODELEGATE_H
#define MIDICONTROLLERCOMBODELEGATE_H

#include <QComboBox>
#include <QObject>
#include <QStyledItemDelegate>

#include <string>
#include <vector>

class QModelIndex;
class QWidget;
class QVariant;

class MidiControllerComboDelegate : public QStyledItemDelegate {
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

  private:
	QComboBox *createCbEditor(QWidget *parent) const;
	QMap<int, QString> hashedData;
};

#endif // MIDICONTROLLERCOMBODELEGATE_H
