//
// File: Listener.cc
// =================
//

#include "Listener.h"
#include <src/system/System.h>

using namespace Identi3D;

void Listener::onRendering(UINT timeSinceLastFrame)
{
}

void Listener::onKeyPressed(KeyType key, UINT repeat_times, UINT timeSinceLastFrame)
{
	switch(key)
	{
	case KeyType_Escape:
		System::instance().kill();
		return ;
	}
}
