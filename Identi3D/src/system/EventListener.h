//
// File: EventListener.h
// =====================
// Listener of system event.
//

#ifndef IDENTI3D_SRC_IDENTI3D_EVENTLISTENER_H
#define IDENTI3D_SRC_IDENTI3D_EVENTLISTENER_H

#include <src/identi3d/General.h>

namespace Identi3D
{

	class EventListener
	{
	public:
		EventListener(void) {} ;
		virtual ~EventListener(void) {} ;

		virtual bool processRawPacket(EventPacket &packet, LRESULT &result);

		virtual void onRendering(UINT timeSinceLastFrame) {} ;
		virtual void onKeyPressed(KeyType key, UINT repeat_times, UINT timeSinceLastFrame) {} ;
	};

};

#endif // IDENTI3D_SRC_IDENTI3D_EVENTLISTENER_H