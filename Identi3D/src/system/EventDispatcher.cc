//
// File: EventDispatcher.cc
// ========================
//

#include <src/identi3d/EventDispatcher.h>
#include <src/identi3d/EventListener.h>
#include <src/identi3d/IdentiExceptions.h>

namespace Identi3D
{

	EventDispatcher::EventDispatcher(DebugManager *debugger) : DebugFrame(debugger)
	{
		memset(_hook, 0, sizeof(_hook));
		_hook_count = 0;
	}

	HRESULT EventDispatcher::RegisterEventListener(EventListener *listener)
	{
		if(listener == NULL) return E_FAIL;
		if(_hook_count >= MAX_EVENT_LISTENER) return E_FAIL;

		_hook[_hook_count++] = listener;
		return S_OK;
	}

	HRESULT EventDispatcher::UnregisterEventListener(EventListener *listener)
	{
		// TODO: unregister the listener.
		return S_OK;
	}

	LRESULT EventDispatcher::postWindowMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) const
	{
		EventPacket packet;
		PAINTSTRUCT ps;
		LRESULT retval;
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

		try
		{
			abandoned = true;
			retval = 0;
			for(UINT i = 0; i < _hook_count; i++) {
				if(_hook[i]->processRawPacket(packet, retval)) abandoned = false;
			}
		} catch(std::exception &e) {
			if(_debugger) _debugger->print(__FILE__, __LINE__, e);
		} catch(...) {
			if(_debugger) _debugger->print(__FILE__, __LINE__, UnknownException());
		}

		if(abandoned) return DefWindowProc(hwnd, msg, wparam, lparam);
		return retval;
	}

	LRESULT EventDispatcher::postEvent(Event e, DWORD param1, DWORD param2) const
	{
		EventPacket packet;
		LRESULT retval;

		packet.event_message = e;
		packet.param1 = param1;
		packet.param2 = param2;
		packet.time_since_last_frame = 0;

		retval = 0;
		for(UINT i = 0; i < _hook_count; i++) {
			_hook[i]->processRawPacket(packet, retval);
		}
		return retval;
	}

};
