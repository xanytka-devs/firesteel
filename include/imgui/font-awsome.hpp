#ifndef FS_IMGUI_FA_H
#define FS_IMGUI_FA_H

#include "base.hpp"
#include <../external/imgui/IconsFontAwesome6.h>

namespace FSImGui {

	static void LoadFA(const std::string& tFontPath, const float tFontSize = 13) {
		ImGuiIO& io = ImGui::GetIO();
		float iconFontSize = tFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

		// merge in icons from Font Awesome
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		icons_config.GlyphMinAdvanceX = iconFontSize;

		io.Fonts->AddFontFromFileTTF(tFontPath.c_str(), iconFontSize, &icons_config, icons_ranges);
	}

}

#endif // !FS_IMGUI_FA_H