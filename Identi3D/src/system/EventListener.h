//
// File: EventListener.h
// =====================
// Listener of system event.
//

#ifndef IDENTI3D_SRC_SYSTEM_EVENTLISTENER_H
#define IDENTI3D_SRC_SYSTEM_EVENTLISTENER_H

#include <src/identi3d/General.h>

namespace Identi3D
{

	class EventListener
	{
	public:
		EventListener(void) {} ;
		virtual ~EventListener(void) {} ;

		virtual bool processRawPacket(EventPacket &packet, int &result);

		virtual void onRendering(UINT timeSinceLastFrame) throw () {} ;
		virtual void onKeyPressed(KeyType key, UINT repeat_times, UINT timeSinceLastFrame) throw() {} ;
	};

};

#endif // IDENTI3D_SRC_SYSTEM_EVENTLISTENER_H