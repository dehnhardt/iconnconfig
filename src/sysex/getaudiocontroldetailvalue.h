#ifndef GETAUDIOCONTROLDETAILVALUE_H
#define GETAUDIOCONTROLDETAILVALUE_H

#include "portsysexmessage.h"

class GetAudioControlDetailValue : public PortSysExMessage {

  public:
	GetAudioControlDetailValue(Device *m_pDevice);
	virtual ~GetAudioControlDetailValue() override;

	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
	                  Device *m_pDevice) override;

	unsigned char getControllerNumber() const;
	void setControllerNumber(unsigned char getControllerNumber);

	unsigned char getDetailNumber() const;
	void setDetailNumber(unsigned char getDetailNumber);

  protected:
	std::vector<unsigned char> *getMessageData() override;

  private:
	unsigned char m_iControllerNumber = 0;
	unsigned char m_iDetailNumber = 0;
};

#endif // GETAUDIOCONTROLDETAILVALUE_H
