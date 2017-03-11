#ifndef SYSEXEVENT_H
#define SYSEXEVENT_H


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
    unsigned char header[];
    std::vector<unsigned char> midiSysExMessage;
};

#endif // SYSEXEVENT_H
