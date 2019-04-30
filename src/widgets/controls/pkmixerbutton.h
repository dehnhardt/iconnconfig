#ifndef PKMIXERBUTTON_H
#define PKMIXERBUTTON_H

#include <QToolButton>

class PKMixerButton : public QToolButton {
  public:
	PKMixerButton(QWidget *parent = nullptr);
	void setColor(int r, int g, int b);

	// QWidget interface
  protected:
	void paintEvent(QPaintEvent *event) override;
};

#endif // PKMIXERBUTTON_H
