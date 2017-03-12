#ifndef MIOMAIN_H
#define MIOMAIN_H


#include <QMainWindow>
#include "libusb.h"
#include "RtMidi.h"


namespace Ui {
class MioMain;
}

class MioMain : public QMainWindow
{
    Q_OBJECT

public:
    RtMidiIn  *midiin = 0;
    RtMidiOut *midiout = 0;
    libusb_device **devs;

    explicit MioMain(QWidget *parent = 0);
    ~MioMain();

    void setupMidiPorts();
    bool setupUSB();
    void printUSBDevs( );
    void sentSysexMessage(std::vector<unsigned char> message);


private slots:
    void on_actionQuit_triggered();

private:
    Ui::MioMain *ui;
    void createMidiIn();
    void createMidiOut();
    static void mmcallback( double deltatime, std::vector< unsigned char > *message, void * /*userData*/ );
};

#endif // MIOMAIN_H
