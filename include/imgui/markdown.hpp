#ifndef FS_IMGUI_MARKDOWN_H
#define FS_IMGUI_MARKDOWN_H

#include "base.hpp"
#include "../external/imgui_markdown.h"

namespace FSImGui {
    class MD {
    public:
        static void LoadFonts(const std::string& tFontPath, const float tFontSize = 13.2f) {
            ImGuiIO& io = ImGui::GetIO();
            H3 = H2 = H1 = io.Fonts->AddFontFromFileTTF(tFontPath.c_str(), tFontSize, nullptr, io.Fonts->GetGlyphRangesCyrillic());
        }

        static void LoadFonts(const std::string& tH2FontPath, const std::string& tH1FontPath,
            const float tFontSize = 12.0f, const float tH1FontSize = 13.2f) {
            ImGuiIO& io = ImGui::GetIO();
            H1 = io.Fonts->AddFontFromFileTTF(tH1FontPath.c_str(), tH1FontSize, nullptr, io.Fonts->GetGlyphRangesCyrillic());
            H3 = H2 = io.Fonts->AddFontFromFileTTF(tH2FontPath.c_str(), tFontSize,   nullptr, io.Fonts->GetGlyphRangesCyrillic());
        }

        static void Text(const std::string& tMD) {
            mdConfig.linkCallback = LinkCallback;
            mdConfig.tooltipCallback = NULL;
            mdConfig.imageCallback = ImageCallback;
            mdConfig.linkIcon = "";
            mdConfig.headingFormats[0] = { H1, true };
            mdConfig.headingFormats[1] = { H2, true };
            mdConfig.headingFormats[2] = { H3, false };
            mdConfig.userData = NULL;
            ImGui::Markdown(tMD.c_str(), tMD.length(), mdConfig);
        }
    private:
        static ImFont* H1;
        static ImFont* H2;
        static ImFont* H3;

        static ImGui::MarkdownConfig mdConfig;

        static void LinkCallback(ImGui::MarkdownLinkCallbackData data_) {
            std::string url(data_.link, data_.linkLength);
            if (!data_.isImage) openURL(url.c_str());
        }

        static inline ImGui::MarkdownImageData ImageCallback(ImGui::MarkdownLinkCallbackData data_) {
            // In your application you would load an image based on data_ input. Here we just use the imgui font texture.
            ImTextureID image = ImGui::GetIO().Fonts->TexID;
            // > C++14 can use ImGui::MarkdownImageData imageData{ true, false, image, ImVec2( 40.0f, 20.0f ) };
            ImGui::MarkdownImageData imageData;
            imageData.isValid = true;
            imageData.useLinkCallback = false;
            imageData.user_texture_id = image;
            imageData.size = ImVec2(40.0f, 20.0f);

            // For image resize when available size.x > image width, add
            ImVec2 const contentSize = ImGui::GetContentRegionAvail();
            if (imageData.size.x > contentSize.x) {
                float const ratio = imageData.size.y / imageData.size.x;
                imageData.size.x = contentSize.x;
                imageData.size.y = contentSize.x * ratio;
            }

            return imageData;
        }
    };
}

ImFont* FSImGui::MD::H1 = NULL;
ImFont* FSImGui::MD::H2 = NULL;
ImFont* FSImGui::MD::H3 = NULL;
ImGui::MarkdownConfig FSImGui::MD::mdConfig{};

#endif // !FS_IMGUI_MARKDOWN_H