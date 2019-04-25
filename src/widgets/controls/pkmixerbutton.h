#ifndef PKMIXERBUTTON_H
#define PKMIXERBUTTON_H

#include <QToolButton>

class PKMixerButton : public QToolButton {
  public:
	PKMixerButton(QWidget *parent = nullptr);
	void setColor(unsigned int r, unsigned int g, unsigned int b);

	// QWidget interface
  protected:
	void paintEvent(QPaintEvent *event) override;
};

#endif // PKMIXERBUTTON_H
