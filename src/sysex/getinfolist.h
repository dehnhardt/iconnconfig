#ifndef GETINFOLIST_H
#define GETINFOLIST_H

#include "retinfolist.h"
#include "sysexmessage.h"

class GetInfoList : public SysExMessage {
  public:
	GetInfoList(Device *m_pDevice);
	GetInfoList(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}
	virtual ~GetInfoList() {}
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

  private:
	void createAnswer(Command cmd, BYTE_VECTOR *message, Device *device) {
		m_pAnswer = new RetInfoList(cmd, message, device);
		m_pAnswer->parseAnswerData();
	}
};

#endif // GETINFOLIST_H
