//
// File: NovaNotification.h
// ========================
//

#ifndef NOVA3D_SRC_NOVA3D_NOVANOTIFICATION_H
#define NOVA3D_SRC_NOVA3D_NOVANOTIFICATION_H

// Errors		start with letter 'E'
// Warnings		start with letter 'W'
// Information	starts with	letter 'I'

// General
#define E_OUT_OF_MEMORY						__T("Memory insufficient.")
#define E_INVALID_PARAMETERS				__T("Invalid parameters.")
#define E_FILE_OPEN_FAILURE					__T("Unable to open \'%s\'.")

// Renderer
#define I_RENDERER_INITIALIZED				__T("Class Renderer initialized.")

#define E_RENDER_DEVICE_CREATE_FAILURE		__T("Failed to create render device \"%s\".")
#define E_INVALID_RENDER_PLUGIN_TYPE		__T("Invalid render plugin type: \"%d\".")
#define E_RENDER_PLUGIN_LOAD_FAILURE		__T("Render plugin \"%s\" failed to load.")

#endif