//
// File: Listener.h
// ================
// Application listener.
//

#ifndef UNITTESTS_RENDERTEST_LISTENER_H
#define UNITTESTS_RENDERTEST_LISTENER_H

#include <src/identi3d/Identi3D.h>
#include <src/identi3d/EventListener.h>

class Listener : public Identi3D::EventListener
{
public:
	void onRendering(UINT timeSinceLastFrame);
	void onKeyPressed(Identi3D::KeyType key, UINT repeat_times, UINT timeSinceLastFrame);
};

#endif // UNITTESTS_RENDERTEST_LISTENER_H
