#ifndef FS_INTERNAL_DEVVIEW_H
#define FS_INTERNAL_DEVVIEW_H

#include <firesteel/internal/config.hpp>
#include <firesteel/renderer.hpp>

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
			ImGui::Text("FPS: %d", tFPS);
			static std::vector<float> frameTimes;
			frameTimes.push_back(tDeltaTime);
			if(frameTimes.size() > 128) frameTimes.erase(frameTimes.begin());
			ImGui::PlotLines("##frame_times_plot", frameTimes.data(), static_cast<int>(frameTimes.size()));
			if(ImGui::IsItemHovered()) {
				ImGui::BeginTooltip();
				ImGui::Text("Delta time: %.5f", tDeltaTime);
				ImGui::EndTooltip();
			}
			ImGui::Text(("Renderer: "+Enviroment::getRenderer()->name()).c_str());
#if defined(FS_INCLUDE_NVTX)||defined(FS_PRINT_DEBUG_MSGS)||!defined(FS_NO_COMPONENTS)||!defined(FS_NO_JSON)
			ImGui::Separator();
#endif
#ifdef FS_INCLUDE_NVTX
			ImGui::Text("NVTX enabled");
#endif // FS_INCLUDE_NVTX
#ifdef FS_PRINT_DEBUG_MSGS
			ImGui::Text("Debug printing enabled");
#endif // FS_INCLUDE_NVTX
#ifndef FS_NO_COMPONENTS
			ImGui::Text("Component system enabled");
#endif // FS_INCLUDE_NVTX
#ifndef FS_NO_JSON
			ImGui::Text("JSON modules enabled");
#endif // FS_INCLUDE_NVTX

			ImGui::Separator();
			if(ImGui::MenuItem("Made with Firesteel")) INTERNAL::openURL("https://github.com/xanytka-devs/firesteel");
			ImGui::Text("v.%s",FS_VERSION);
			ImGui::End();
		}
	}
}

#endif // !FS_INTERNAL_DEVVIEW_H