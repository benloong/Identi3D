//
// File: Messages.h
// ================
// Defines system messages.
//

#ifndef IDENTI3D_SRC_IDENTI3D_MESSAGES_H
#define IDENTI3D_SRC_IDENTI3D_MESSAGES_H

// Errors		start with letter 'E'
// Warnings		start with letter 'W'
// Information	starts with	letter 'I'

// General
#define E_FATAL_ERROR						("System encountered a fatal error. Exiting...")
#define E_OUT_OF_MEMORY						("Memory insufficient.")
#define E_INVALID_PARAMETERS				("Invalid parameters.")
#define E_FILE_OPEN_FAILURE					("Unable to open \'%S\'.")

// System
#define E_SYSTEM_INITIALIZED				("System has already initialized.")
#define I_SYSTEM_LOAD_CONFIGURATION			("Loading configuration from %S...")
#define I_SYSTEM_CONFIGURATION_LOAD_SUCCESS ("Configuration load completed.")
#define E_SYSTEM_CONFIGURATION_LOAD_FAIL	("Failed to load configuration.")
#define I_SYSTEM_CREATE_SUCCESS				("System has been initialized successfully.")
#define I_SYSTEM_RELEASE_SCHEDULED			("System is going to be released.")
#define E_SYSTEM_RENDERER_CREATE_FAILED		("Failed to create a renderer.")

// Debug Manager
#define I_LOG_STARTED						("Identi3D Debugging System\nLog started.\n\n")

// Settings Manager
#define I_SETTINGS_LOAD_COMPLETED			("Settings load complete. Source: %S, %d/%d accepted.")

// OptionTree
#define W_OPTION_ELEMENT_ALREADY_EXISTS		("Element \"%S\" already exists in OptionTree. Value will be overwritten.")
#define I_NEW_OPTION_ELEMENT_ADDED			("New element has been added: Name [%S], Value [%S].")
#define I_OPTION_ELEMENT_REMOVAL_SCHEDULED	("Will now remove: Name [%S].")
#define I_REMOVING_ENTIRE_OPTION_TREE		("Removing the entire option tree...")
#define E_OPTION_ELEMENT_STACK_OVERFLOW		("Layers of location exceeded %d ; aborting...")
#define I_OPTION_ELEMENT_LOCATION			("Location result: %S")
#define I_OPTION_VALUE_MODIFIED				("Element \"%S\" value modified: New value = [%S].")

// Renderer
#define I_RENDERER_INITIALIZED				("Class Renderer initialized.")
#define E_RENDER_WINDOW_ASSIGN_FAIL			("Failed to assign render target.")
#define E_RENDER_WINDOW_ASSIGN_SUCCESS		("Successfully assigned render target.")
#define E_RENDER_DEVICE_CREATE_FAILURE		("Failed to create render device \"%S\".")
#define E_NO_RENDER_DEVICE_AVAILABLE		("No available render device.")
#define E_INVALID_RENDER_PLUGIN_TYPE		("Invalid render plugin type: \"%d\".")
#define E_RENDER_PLUGIN_LOAD_FAILURE		("Render plugin \"%S\" failed to load.")

// Direct3D9 Settings Manager
#define E_D3D9_OPTION_GROUP_CREATE_FAILED	("Failed to create option group \"%S\"")
#define E_D3D9_OPTION_WRITE_FAILED			("Failed to write one or more options.")

// Direct3D9 Render Plugin
#define E_DIRECT3D_INIT_FAILURE				("Failed to initialize Direct3D. Please make sure you have installed the latest Direct3D SDK.")
#define E_NO_DISPLAY_MODE_AVAILABLE			("No display modes found.")
#define I_BEGIN_ENUMERATE_DISPLAY_MODE		("Enumerating display modes, Total %d...")
#define I_DISPLAY_MODE_FORMAT				("	%d x %d @ %d")
#define I_DISPLAY_MODE_FOUND				("Suitable display mode detected.")
#define E_ENUMERATE_DISPLAY_MODE_FAILURE	("Failed to enumerate display mode.")
#define E_NO_SUITABLE_DISPLAY_MODE			("No suitable display mode found. Please check configuration.")
#define E_NOT_SUPPORTED_MODE				("The selected mode is not supported by the graphics card.")
#define E_GET_DEVICE_CAPABILITIES_FAILURE	("Failed to get device capabilities.")
#define E_FSAA_NOT_AVAILABLE				("FSAA not available on your hardware.")
#define E_CREATE_DIRECT3D_DEVICE_FAILURE	("Failed to create Direct3D device.")

#define E_CLEAR_BUFFER_FAILURE				("Failed to clear buffers.")
#define E_BEGIN_SCENE_FAILURE				("Failed to begin scene.")
#define E_END_SCENE_FAILURE					("Failed to end scene.")

// Render Window
#define E_CREATE_RENDER_WINDOW_FAILURE		("Failed to create render window.")
#define I_WINDOW_CLASS_NAME_ALLOCATED		("New render window: \"%S\".")

#define E_START_RENDERING_FAILURE			("Failed to start rendering.")

#endif // IDENTI3D_SRC_IDENTI3D_MESSAGES_H
