#include "miomain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MioMain w;
    w.setupUSB();
    w.printUSBDevs();
    w.show();
    //w.setupMidiPorts();
    return a.exec();
}
