#ifndef GETAUDIOCONTROLDETAIL_H
#define GETAUDIOCONTROLDETAIL_H

#include "portsysexmessage.h"

class GetAudioControlDetail : public PortSysExMessage {
  public:
	GetAudioControlDetail(Device *m_pDevice);
	virtual ~GetAudioControlDetail();

	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
	                  Device *m_pDevice);

	void setControllerNumber(unsigned int iControllerNumber);
	void setDetailNumber(unsigned int iDetailNumber);

  protected:
	std::vector<unsigned char> *m_pGetMessageData();

  private:
	unsigned int m_iControllerNumber = 0;
	unsigned int m_iDetailNumber = 0;
};

#endif // GETAUDIOCONTROLDETAIL_H
