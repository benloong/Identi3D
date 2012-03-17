//
// File: App.h
// ===========
// Sample Identi3D application.
//

#ifndef IDENTI3D_UNITTESTS_RENDERTEST_APP_H
#define IDENTI3D_UNITTESTS_RENDERTEST_APP_H

#include <src/identi3d/Identi3D.h>

class App : private Identi3D::RenderWindow
{
private:
	Identi3D::DebugManager	*_debugger;
	Identi3D::RenderDevice	*_device;

public:
	App(void);
	~App(void);

	HRESULT	init(void);
	int		run(void);

	void onKeyboardInput(Identi3D::KeyType key, UINT repeat_times, bool previous_key_pressed);
	void onRendering(void);
};

#endif // IDENTI3D_UNITTESTS_RENDERTEST_APP_H
