#include "devicedockwidget.h"

DeviceDockWidget::DeviceDockWidget(QWidget *parent, Device *device)
    : QDockWidget(parent), device(device) {}

DeviceDockWidget::~DeviceDockWidget() {}
