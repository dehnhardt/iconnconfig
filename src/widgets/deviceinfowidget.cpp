#include "deviceinfowidget.h"
#include "ui_deviceinfowidget.h"

#include <QListWidgetItem>

DeviceInfoWidget::DeviceInfoWidget(QWidget *parent, Device *device)
    : MultiInfoWidget(parent, device) {}

DeviceInfoWidget::~DeviceInfoWidget() {}
