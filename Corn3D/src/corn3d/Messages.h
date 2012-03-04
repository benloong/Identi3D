//
// File: Messages.h
// ================
// Defines system messages.
//

#ifndef CORN3D_SRC_CORN3D_MESSAGES_H
#define CORN3D_SRC_CORN3D_MESSAGES_H

// Errors		start with letter 'E'
// Warnings		start with letter 'W'
// Information	starts with	letter 'I'

// General
#define E_FATAL_ERROR						__T("System encountered a fatal error. Exiting...")
#define E_OUT_OF_MEMORY						__T("Memory insufficient.")
#define E_INVALID_PARAMETERS				__T("Invalid parameters.")
#define E_FILE_OPEN_FAILURE					__T("Unable to open \'%s\'.")

// Debug Manager
#define I_LOG_STARTED						__T("Corn3D Debugging System\nLog started.\n\n")

// Renderer
#define I_RENDERER_INITIALIZED				__T("Class Renderer initialized.")

#define E_RENDER_DEVICE_CREATE_FAILURE		__T("Failed to create render device \"%s\".")
#define E_INVALID_RENDER_PLUGIN_TYPE		__T("Invalid render plugin type: \"%d\".")
#define E_RENDER_PLUGIN_LOAD_FAILURE		__T("Render plugin \"%s\" failed to load.")

// Direct3D9 Render Plugin
#define E_DIRECT3D_INIT_FAILURE				__T("Failed to initialize Direct3D. Please make sure you have installed the latest Direct3D SDK.")
#define E_NO_DISPLAY_MODE_AVAILABLE			__T("No display modes found.")
#define I_BEGIN_ENUMERATE_DISPLAY_MODE		__T("Enumerating display modes, Total %d...")
#define I_DISPLAY_MODE_FORMAT				__T("	%d x %d @ %d")
#define I_DISPLAY_MODE_FOUND				__T("Suitable display mode detected.")
#define E_ENUMERATE_DISPLAY_MODE_FAILURE	__T("Failed to enumerate display mode.")
#define E_NO_SUITABLE_DISPLAY_MODE			__T("No suitable display mode found. Please check configuration.")
#define E_NOT_SUPPORTED_MODE				__T("The selected mode is not supported by the graphics card.")
#define E_GET_DEVICE_CAPABILITIES_FAILURE	__T("Failed to get device capabilities.")
#define E_FSAA_NOT_AVAILABLE				__T("FSAA not available on your hardware.")
#define E_CREATE_DIRECT3D_DEVICE_FAILURE	__T("Failed to create Direct3D device.")

#define E_CLEAR_BUFFER_FAILURE				__T("Failed to clear buffers.")
#define E_BEGIN_SCENE_FAILURE				__T("Failed to begin scene.")
#define E_END_SCENE_FAILURE					__T("Failed to end scene.")

// Render Window
#define E_CREATE_RENDER_WINDOW_FAILURE		__T("Failed to create render window.")
#define I_WINDOW_CLASS_NAME_ALLOCATED		__T("New render window: \"%s\".")

#define E_START_RENDERING_FAILURE			__T("Failed to start rendering.")

#endif // CORN3D_SRC_CORN3D_MESSAGES_H
