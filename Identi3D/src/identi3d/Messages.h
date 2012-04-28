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
#define E_FATAL_ERROR						"System encountered a fatal error. Exiting..."
#define E_OUT_OF_MEMORY						"Memory insufficient."
#define E_INVALID_PARAMETERS				"Invalid parameters."
#define E_FILE_OPEN_FAILURE					"Unable to open \'%S\'."

// System
#define I_SYSTEM_LOADING_CONFIGURATION		"Loading configuration from %S..."
#define I_SYSTEM_CONFIGURATION_LOAD_SUCCESS "Configuration load completed."
#define W_SYSTEM_CONFIGURATION_LOAD_FAILURE	"Unable to load configuration."
#define I_SYSTEM_CREATE_SUCCESS				"System has been initialized successfully."
#define I_SYSTEM_RELEASING					"Releasing system..."
#define E_SYSTEM_CREATE_RENDERER_FAILURE	"Failed to create a renderer."
#define E_SYSTEM_CREATE_DEBUGGER_FAILURE	"Failed to create a debugger."
#define E_SYSTEM_CREATE_DISPATCHER_FAILURE	"Failed to create a event dispatcher."
#define E_SYSTEM_CREATE_CONFMGR_FAILURE		"Failed to create a settings manager."
#define E_SYSTEM_LOAD_RESOURCES_FAILURE		"Failed to load resources."

// Settings Manager
#define I_SETTINGS_LOAD_COMPLETED			"Settings load complete. Source: %S, %d/%d accepted."

// OptionTree
#define W_OPTION_ELEMENT_ALREADY_EXISTS		"Element \"%S\" already exists in OptionTree. Value will be overwritten."
#define I_NEW_OPTION_ELEMENT_ADDED			"New element added: Name [%S], Value [%S]."
#define I_OPTION_TREE_REMOVED				"Option tree removed."
#define I_OPTION_ELEMENT_REMOVAL_SCHEDULED	("Will now remove: Name [%S].")
#define E_OPTION_ELEMENT_STACK_OVERFLOW		("Layers of location exceeded %d ; aborting...")
#define I_OPTION_ELEMENT_LOCATION			("Location result: %S")
#define I_OPTION_VALUE_MODIFIED				("Element \"%S\" value modified: New value = [%S].")

// Renderer
#define I_RENDERER_PLUGIN_LOADED			"Plugin \"%S\" loaded."
#define E_RENDERER_INVALID_PLUGIN_TYPE		"Invalid render plugin type: \"%d\"."
#define E_RENDERER_PLUGIN_LOAD_FAILURE		"Render plugin \"%S\" failed to load."
#define E_RENDERER_DEVICE_CREATE_FAILURE	"Failed to create render device \"%S\"."
#define W_RENDERER_SPECIFIED_NOT_AVAILABLE	"specified render plugin not available. Will retry with other plugins."
#define E_RENDERER_NO_PLUGIN_AVAILABLE		"No available render plugin."
#define E_RENDERER_ASSIGN_WINDOW_FAILURE	"Failed to assign a render target for device."
#define I_RENDERER_ASSIGN_WINDOW_SUCCESS	"Render target assigned successfully."

// Render Window
#define E_WINDOW_CREATE_FAILURE				"Failed to create render window. Last Error Code: %d"
#define I_WINDOW_CREATE_SUCCESS				"New render window created: \"%S\"."

#define E_START_RENDERING_FAILURE			("Failed to start rendering.")

// Direct3D9 Settings Manager
#define E_D3D9_OPTION_GROUP_CREATE_FAILED	("Failed to create option group \"%S\"")
#define E_D3D9_OPTION_WRITE_FAILED			("Failed to write one or more options.")

// Direct3D9 Render Plugin
#define E_D3D9_INITIALIZE_FAILURE			"Failed to initialize Direct3D. Please ensure your Direct3D version >= 9.0."
#define W_D3D9_HARDWARE_NOT_COMPATIBLE		"Hardwares not support current settings. Will retry using fallback."
#define E_D3D9_PREREQUISITE_NOT_SATISFIED	"Initialization failed: Prerequisite not satisfied."

#define E_GET_DEVICE_CAPABILITIES_FAILURE	"Failed to get device capabilities."
#define W_FSAA_NOT_AVAILABLE				"FSAA not available on your hardware."
#define E_CREATE_DIRECT3D_DEVICE_FAILURE	"Failed to create Direct3D device."
#define E_NO_DISPLAY_MODE_AVAILABLE			"No display modes found."
#define I_BEGIN_ENUMERATE_DISPLAY_MODE		"Enumerating display modes, Total %d..."
#define I_DISPLAY_MODE_FORMAT				"	%d x %d @ %d"
#define I_DISPLAY_MODE_FOUND				"Suitable display mode detected."
#define E_ENUMERATE_DISPLAY_MODE_FAILURE	"Failed to enumerate display mode."
#define E_NO_SUITABLE_DISPLAY_MODE			"No suitable display mode found. Please check configuration."
#define E_NOT_SUPPORTED_MODE				"The selected mode is not supported by the graphics card."

#endif // IDENTI3D_SRC_IDENTI3D_MESSAGES_H
