//
// File: EventDispatcher.cc
// ========================
//

#include <src/system/EventDispatcher.h>
#include <src/system/EventListener.h>
#include <src/identi3d/IdentiExceptions.h>
#include <algorithm>

namespace Identi3D
{

	EventDispatcher::EventDispatcher(DebugManager *debugger) : DebugFrame(debugger)
	{
	}

	bool EventDispatcher::RegisterEventListener(EventListener &listener)
	{
		_hook.push_back(&listener);
		return true;
	}

	void EventDispatcher::UnregisterEventListener(EventListener &listener)
	{
		_hook.erase(std::find(_hook.begin(), _hook.end(), &listener));
	}

	int EventDispatcher::postWindowMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		EventPacket packet;
		PAINTSTRUCT ps;
		int retval;
		bool abandoned;
		HDC hdc;

		switch(msg)
		{
		case WM_KEYDOWN:
			packet.event_message = Event_KeyPressed;
			packet.param1 = wparam;
			packet.param2 = lparam & 0xffff;
			packet.time_since_last_frame = 0;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}

		abandoned = true;
		retval = 0;
		for(EventListenerList::iterator iter = _hook.begin(); iter != _hook.end(); ++iter) {
			if((*iter)->processRawPacket(packet, retval)) abandoned = false;
		}

		if(abandoned) return DefWindowProc(hwnd, msg, wparam, lparam);
		return retval;
	}

	int EventDispatcher::postEvent(Event e, DWORD param1, DWORD param2)
	{
		EventPacket packet;
		int retval;

		packet.event_message = e;
		packet.param1 = param1;
		packet.param2 = param2;
		packet.time_since_last_frame = 0;

		retval = 0;
		for(EventListenerList::iterator iter = _hook.begin(); iter != _hook.end(); ++iter) {
			(*iter)->processRawPacket(packet, retval);
		}
		return retval;
	}

};
