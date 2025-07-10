#ifndef FS_INTERNAL_DEVVIEW
#define FS_INTERNAL_DEVVIEW

#include "config.hpp"

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
		static bool sDrawDevView;
		static void draw(const float tDeltaTime, const unsigned int tFPS) {
			if(!sDrawDevView || !CONFIG::sAllowDevView) return;
			ImGui::Begin("Dev View", &sDrawDevView);
			ImGui::Text(("FPS: " + std::to_string(tFPS)).c_str());
			static std::vector<float> frameTimes;
			frameTimes.push_back(tDeltaTime);
			if(frameTimes.size() > 128) frameTimes.erase(frameTimes.begin());
			ImGui::PlotLines("##frame_times_plot", frameTimes.data(), static_cast<int>(frameTimes.size()));
			ImGui::Text(("Delta time: " + std::to_string(tDeltaTime)).c_str());
			ImGui::Text("Renderer: OpenGL");
			ImGui::Separator();
			if(ImGui::MenuItem("Made with Firesteel")) INTERNAL::openURL("https://github.com/xanytka-devs/firesteel");
			ImGui::End();
		}
	}
}

#endif // !FS_INTERNAL_DEVVIEW