//
// File: App.h
// ===========
// Sample Identi3D application.
//

#ifndef IDENTI3D_UNITTESTS_RENDERTEST_APP_H
#define IDENTI3D_UNITTESTS_RENDERTEST_APP_H

#include <src/identi3d/Identi3D.h>
#include <src/identi3d/EventListener.h>

#include "Listener.h"

class App
{
private:
	Identi3D::DebugManager	*_debugger;
	Identi3D::RenderDevice	*_device;
	Identi3D::Renderer		*_renderer;
	Identi3D::RenderWindow	*_window;

	Listener *listener;

public:
	App(void);
	~App(void);

	HRESULT	init(void);
	int		run(void);
};

#endif // IDENTI3D_UNITTESTS_RENDERTEST_APP_H
