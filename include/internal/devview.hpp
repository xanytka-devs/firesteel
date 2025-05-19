#ifndef FS_INTERNAL_DEVVIEW
#define FS_INTERNAL_DEVVIEW

#include <imgui.h>

namespace INTERNAL {
#ifdef WIN32
#include <windows.h>
	static void openURL(const char* tUrl) {
		ShellExecuteA(NULL, "open", tUrl, NULL, NULL, SW_SHOWNORMAL);
	}
#endif // !WIN32
#ifdef __linux__
	static void openURL(const char* tUrl) {
		system(tUrl);
	}
#endif // !__linux__
}

namespace Firesteel {
	namespace DEVVIEW {
		static bool drawDevView;
		static void draw(const float tDeltaTime, const unsigned int tFPS) {
			if(!drawDevView) return;
			ImGui::Begin("Dev View", &drawDevView);
			ImGui::Text(("FPS: " + std::to_string(tFPS)).c_str());
			static std::vector<float> frameTimes;
			frameTimes.push_back(tDeltaTime);
			if(frameTimes.size() > 128) frameTimes.erase(frameTimes.begin());
			ImGui::PlotLines("##frame_times_plot", frameTimes.data(), static_cast<int>(frameTimes.size()));
			if(ImGui::IsItemHovered()) {
				ImGui::BeginTooltip();
				ImGui::Text(("Delta time: " + std::to_string(tDeltaTime)).c_str());
				ImGui::EndTooltip();
			}
			ImGui::Separator();
			if(ImGui::MenuItem("Made with Firesteel")) INTERNAL::openURL("https://github.com/xanytka-devs/firesteel");
			ImGui::End();
		}
	}
}

#endif // !FS_INTERNAL_DEVVIEW