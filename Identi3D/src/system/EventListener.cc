//
// File: EventListener.cc
// ======================
//

#include <src/system/EventListener.h>
#include <src/system/EventDispatcher.h>

namespace Identi3D
{

	bool EventListener::processRawPacket(EventPacket &packet, int &result)
	{
		switch(packet.event_message)
		{
		case Event_Rendering:
			onRendering(packet.time_since_last_frame);
			result = 0;
			return true;
		case Event_KeyPressed:
			onKeyPressed((KeyType)(packet.param1), (UINT)(packet.param2), 
				packet.time_since_last_frame);
			result = 0;
			return true;
		}
		return false;
	}

};
