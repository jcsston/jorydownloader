
#ifndef _J_NET_DIAL_H_
#define _J_NET_DIAL_H_

#include <wx/wx.h>

#define JNETDIAL_VERSION_STR "0.2.0"
#define JNETDIAL_VERSION 0x03

#define JNETDIAL_CONNECT 0x01
#define JNETDIAL_DISCONNECT 0x02
#define JNETDIAL_STATUS 0x03

//#pragma pack(push, 1)
// Packet sent from the client to the server
struct JNetDialClientPacket {
  wxUint8 version;
  char name[32];
  wxUint16 msg;
};

#define JNETDIAL_ONLINE 0x01
#define JNETDIAL_OFFLINE 0x02
#define JNETDIAL_NO_CONNECTION 0x03

// Packet sent from the server to the client
struct JNetDialServerPacket {
  wxUint8 version;
	char server_name[32];
  char name[32];
  wxUint16 status;
	char status_msg[128];
};
//#pragma pack(pop)

#endif // _J_NET_DIAL_H_

