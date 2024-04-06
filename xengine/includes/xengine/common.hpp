///								  ///
/// XENGINE COMMON ATRIBUTES FILE ///
///								  ///
//
//	This file contains basic rendering attributes
// and common includes (currently only log).
// 
//	 NOTE:
// Be warned, that by adding ANYTHING to this file,
// it will be added to EVERY file in XEngine.
//

/// COMMON INCLUDE ///
// Includes, that are distributed across all engine headers.
#include <xengine/log.hpp>
#include <xengine/math.hpp>
#include <../external/nvtx/c/include/nvtx3/nvtx3.hpp>


/// RENDERING ///
// Select overall rendering engines
// (that are downloaded).
#define OPENGL_RENDERING // PHASE: ADVANCED
//#define VULKAN_RENDERING // PHASE: WIP

// Currently unavailable rendering SDKs (engines).
//#define METAL_RENDERING // TODO
//#define WEB_RENDERING // TODO
//#define PSVITA_RENDERING // TODO
//#define ANDROID_RENDERING // TODO


/// ENGINE FLAGS ///
// Additional engine flags.
#define XE_IMGUI // Flag to activate/disable ImGui.
