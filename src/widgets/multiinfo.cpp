#include "multiinfo.h"

MultiInfo::MultiInfo(QWidget *parent) : QObject(parent) {}

QWidget *MultiInfo::createWidget(std::string widgetName) {
  /*QWidget *w = new QWidget(parent);
  if (widgetName == "Global") {
  }
  return w;*/
  return 0;
}
