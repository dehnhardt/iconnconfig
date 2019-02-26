#ifndef MIDICHANNELCOMBODELEGATE_H
#define MIDICHANNELCOMBODELEGATE_H

#include <QComboBox>
#include <QStyledItemDelegate>

class MidiChannelComboDelegate : public QStyledItemDelegate {
  public:
	MidiChannelComboDelegate(QObject *parent = nullptr);
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
						  const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
					  const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor,
							  const QStyleOptionViewItem &option,
							  const QModelIndex &index) const;

  private slots:
	void commitAndCloseEditor();

  private:
	QComboBox *createCbEditor(QWidget *parent) const;
};

#endif // MIDICHANNELCOMBODELEGATE_H
