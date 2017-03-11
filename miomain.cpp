#include "miomain.h"
#include "ui_miomain.h"

#include <unistd.h>
#include <QtDebug>

void mycallback( double deltatime, std::vector< unsigned char > *message, void * /*userData*/ )
{
  unsigned int nBytes = message->size();
  for ( unsigned int i=0; i<nBytes; i++ )
    std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
  if ( nBytes > 0 )
    std::cout << "stamp = " << deltatime << std::endl;
}

MioMain::MioMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MioMain)
{
    ui->setupUi(this);
}

MioMain::~MioMain()
{
    libusb_exit(NULL);
    delete midiin;
    delete midiout;
    delete ui;
}

bool MioMain::setupUSB(){
    int r;

    r = libusb_init(NULL);
    if (r < 0)
        return false;

    return true;
}

void MioMain::printUSBDevs(){
    ssize_t cnt;
    libusb_device **devs;
    libusb_device *dev;
    int i = 0, j = 0;
    uint8_t path[8];

    cnt = libusb_get_device_list(NULL, &devs);
    if (cnt < 0)
        return;


    while ((dev = devs[i++]) != NULL) {
        struct libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0) {
            fprintf(stderr, "failed to get device descriptor");
            return;
        }

        printf("%04x:%04x (bus %d, device %d)",
            desc.idVendor, desc.idProduct,
            libusb_get_bus_number(dev), libusb_get_device_address(dev));

        r = libusb_get_port_numbers(dev, path, sizeof(path));
        if (r > 0) {
            printf(" path: %d", path[0]);
            for (j = 1; j < r; j++)
                printf(".%d", path[j]);
        }
        printf("\n");
    }

    libusb_free_device_list(devs, 1);

}

void MioMain::setupMidiPorts(){
    createMidiIn();
    createMidiOut();

    midiin->openPort(2);
    midiout->openPort(2);
    std::vector<unsigned char> message;

    sentSysexMessage(message);
}

void MioMain::mmcallback( double deltatime, std::vector< unsigned char > *message, void * /*userData*/ )
{
  unsigned int nBytes = message->size();
  for ( unsigned int i=0; i<nBytes; i++ )
    std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
  if ( nBytes > 0 )
    std::cout << "stamp = " << deltatime << std::endl;
}


void MioMain::on_actionQuit_triggered()
{
    close();
}

void MioMain::createMidiIn(){
    // RtMidiIn constructor
    try {
      midiin = new RtMidiIn(RtMidi::LINUX_ALSA);
    }
    catch (RtMidiError &error) {
      // Handle the exception here
      error.printMessage();
    }
    midiin->ignoreTypes( false, true, true );
    midiin->setCallback(&mmcallback, this);
    unsigned int nPorts = midiin->getPortCount();
    qDebug() << "\nThere are " << nPorts << " MIDI input sources available.\n";
    std::string portName;
    for ( unsigned int i=0; i<nPorts; i++ ) {
      try {
        portName = midiin->getPortName(i);
      }
      catch ( RtMidiError &error ) {
        error.printMessage();
        close();
      }
      qDebug() << "  Input Port #" << i+1  << ": " << portName.c_str() << "\n";
    }
}

void MioMain::createMidiOut(){
    // RtMidiOut constructor
    try {
      midiout = new RtMidiOut(RtMidi::LINUX_ALSA);
    }
    catch ( RtMidiError &error ) {
      error.printMessage();
      exit( EXIT_FAILURE );
    }
    // Check outputs.
    unsigned int nPorts = midiout->getPortCount();
    qDebug() << "\nThere are " << nPorts << " MIDI output ports available.\n";
    std::string portName;
    for ( unsigned int i=0; i<nPorts; i++ ) {
      try {
        portName = midiout->getPortName(i);
      }
      catch (RtMidiError &error) {
        error.printMessage();
      }
      qDebug() << "  Output Port #" << i+1 << ": " << portName.c_str() << "\n";
    }
    std::cout << '\n';
}

void MioMain::sentSysexMessage( std::vector<unsigned char> message ){
    std::vector<unsigned char> message1;

//    message1.push_back( 0xF6 );
//    midiout->sendMessage( &message1 );
//    usleep( 500 ); // pause a little

    message.clear();
    message1.push_back( 0xF0 );
    message1.push_back( 0x00 );
    message1.push_back( 0x01 );
    message1.push_back( 0x73 );
    message1.push_back( 0x7e );
    message1.push_back( 0x00 );
    message1.push_back( 0x02 );
    message1.push_back( 0x00 );
    message1.push_back( 0x00 );
    message1.push_back( 0x06 );
    message1.push_back( 0x5d );
    message1.push_back( 0x00 );
    message1.push_back( 0x01 );
    message1.push_back( 0x40 );
    message1.push_back( 0x22 );
    message1.push_back( 0x00 );
    message1.push_back( 0x02 );
    message1.push_back( 0x00 );
    message1.push_back( 0x01 );
    message1.push_back( 0x35 );
    message1.push_back( 0xF7 );
    midiout->sendMessage( &message1 );
    usleep( 500 ); // pause a little

}
