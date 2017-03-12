#ifndef SYSEXEVENT_H
#define SYSEXEVENT_H

#include <vector>

class SysExEvent
{
public:
    SysExEvent();
    ~SysExEvent();

    virtual std::vector<unsigned char> getMIDISysExMessage();
    std::vector<unsigned char> getUSBSysExMessage();

private:
    int calculateChecksum();

    unsigned char header[];
    unsigned char operation;
    unsigned char length[2];
    std::vector<unsigned char> midiSysExMessage;
};

#endif // SYSEXEVENT_H
