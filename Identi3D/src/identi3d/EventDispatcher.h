//
// File: EventDispatcher.h
// =======================
// Process and dispatch system event.
//

#ifndef IDENTI3D_SRC_IDENTI3D_EVENTDISPATCHER_H
#define IDENTI3D_SRC_IDENTI3D_EVENTDISPATCHER_H

#include <src/identi3d/General.h>

#define MAX_EVENT_LISTENER	5

namespace Identi3D
{

	enum Event
	{
		Event_Rendering		= 0x00000001,
		Event_KeyPressed	= 0x00000002,

		Event_ForceDword	= 0xFFFFFFFF
	};

	struct EventPacket
	{
		//HWND target_window; [INVALID]	// Event target window, NULL specifies BOARDCAST.
		Event event_message;		// Event content
		UINT time_since_last_frame;	// Time since last frame
		DWORD param1;
		DWORD param2;
	};

	class EventDispatcher
	{
	private:
		EventListener *_hook[MAX_EVENT_LISTENER];
		UINT _hook_count;

	public:
		EventDispatcher(void);
		~EventDispatcher(void) {} ;

		HRESULT RegisterEventListener(EventListener *listener);
		HRESULT UnregisterEventListener(EventListener *listener);

		LRESULT postWindowMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) const;
		LRESULT postEvent(Event e, DWORD param1 = NULL, DWORD param2 = NULL) const;
	};

};

#endif // IDENTI3D_SRC_IDENTI3D_EVENTDISPATCHER_H
