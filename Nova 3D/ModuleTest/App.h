//
// File: App.h
// ===========
// Sample Nova 3D application.
//

#ifndef NOVA3D_MODULETEST_APP_H
#define NOVA3D_MODULETEST_APP_H

#include "src/nova-3d/Nova.h"

class App : private Nova3D::RenderWindow
{
private:
	Nova3D::RenderDevice	*device;

public:
	App(void);
	~App(void);

	HRESULT	init(void);
	int		run(void);

	void onKeyboardInput(Nova3D::KeyType key, UINT repeat_times, bool previous_key_pressed);
	void onRendering(void);
};

#endif
