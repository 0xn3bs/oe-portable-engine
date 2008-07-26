#include "Engine/Maths/Vector.h"

namespace OE
{
	namespace Networking
	{
		namespace Packets
		{
			static const int PACKET_UNKNOWN = 0x000000;
			static const int PACKET_PLAYER_CONNECT_REQUEST = 0x000001;
			static const int PACKET_PLAYER_STATE = 0x000002;

			struct _PACKET
			{
				int Type;
				char* Data;
			};

			struct _PACKET_PLAYER_CONNECT_REQUEST
			{
				static const int Type = PACKET_PLAYER_CONNECT_REQUEST;
				int PlayerID;	//	If the player has not been assigned an ID this is set to -1.
			};

			struct _PACKET_PLAYER_STATE
			{
				static const int Type = PACKET_PLAYER_STATE;
				int PlayerID;
				int PlayerMoveType;
				OE::Maths::Vec3<float> PlayerViewDirection;
			};
		}
	}
}
