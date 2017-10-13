#include "getgizmocount.h"

GetGizmoCount::GetGizmoCount(Device *device)
	: SysExMessage(Command::GET_GIZMO_COUNT, SysExMessage::QUERY, device) {}
