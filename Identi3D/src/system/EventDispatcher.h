//
// File: EventDispatcher.h
// =======================
// Process and dispatch system event.
//

#ifndef IDENTI3D_SRC_SYSTEM_EVENTDISPATCHER_H
#define IDENTI3D_SRC_SYSTEM_EVENTDISPATCHER_H

#include <src/identi3d/General.h>
#include <src/utils/DebugFrame.h>

namespace Identi3D
{

	enum Event
	{
		Event_Rendering		= 0x00000001,
		Event_KeyPressed	= 0x00000002,

		Event_RenderExceptionCaught	= 0x00010001,

		Event_ForceDword	= 0xFFFFFFFF
	};

	struct EventPacket
	{
		Event event_message;		// Event content
		UINT time_since_last_frame;	// Time since last frame
		DWORD param1;
		DWORD param2;
	};

	typedef std::vector<EventListener *> EventListenerList;

	class EventDispatcher : public DebugFrame
	{
	private:
		EventListenerList _hook;

	private:
		EventDispatcher(EventDispatcher &dispatcher);
		EventDispatcher &operator=(EventDispatcher &rhs);

	public:
		EventDispatcher(DebugManager *debugger = NULL);
		~EventDispatcher(void) {} ;

		bool RegisterEventListener(EventListener &listener);
		void UnregisterEventListener(EventListener &listener);

		int postWindowMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		int postEvent(Event e, DWORD param1 = NULL, DWORD param2 = NULL);
	};

};

#endif // IDENTI3D_SRC_SYSTEM_EVENTDISPATCHER_H
