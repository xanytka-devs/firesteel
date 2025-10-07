#ifndef FS_IMGUI_UTILS
#define FS_IMGUI_UTILS

#include <glm/glm.hpp>
#include <imgui.h>

namespace ImGuiUtil {
    float vec4[4]={ 0,0,0,0 };
    float vec3[3]={ 0,0,0 };
    float vec2[2]={ 0,0 };
    static bool ColorEdit3(const char* tName, glm::vec3* tColor, ImGuiColorEditFlags tFlags=ImGuiColorEditFlags_None) {
        vec3[0]=tColor->r;
        vec3[1]=tColor->g;
        vec3[2]=tColor->b;
        bool b=ImGui::ColorEdit3(tName,vec3,tFlags);
        if(b) {
            tColor->x=vec3[0];
            tColor->y=vec3[1];
            tColor->z=vec3[2];
        }
        return b;
    }
    static bool ColorEdit4(const char* tName, glm::vec4* tColor, ImGuiColorEditFlags tFlags=ImGuiColorEditFlags_None) {
        vec4[0]=tColor->r;
        vec4[1]=tColor->g;
        vec4[2]=tColor->b;
        vec4[3]=tColor->a;
        bool b=ImGui::ColorEdit4(tName,vec4,tFlags);
        if(b) {
            tColor->r=vec4[0];
            tColor->g=vec4[1];
            tColor->b=vec4[2];
            tColor->a=vec4[3];
        }
        return b;
    }

    static bool DragFloat4(const char* tName, glm::vec4* tFloats, const float& tSpeed=0.1f) {
        vec4[0]=tFloats->x;
        vec4[1]=tFloats->y;
        vec4[2]=tFloats->z;
        vec4[3]=tFloats->w;
        bool b=ImGui::DragFloat4(tName, vec4, tSpeed);
        if(b) {
            tFloats->x=vec4[0];
            tFloats->y=vec4[1];
            tFloats->z=vec4[2];
            tFloats->w=vec4[3];
        }
        return b;
    }
    static bool DragFloat4(const char* tName, float& tF1, float& tF2, float& tF3, float& tF4, const float& tSpeed=0.1f) {
        vec4[0]=tF1;
        vec4[1]=tF2;
        vec4[2]=tF3;
        vec4[3]=tF4;
        bool b=ImGui::DragFloat4(tName, vec4, tSpeed);
        if(b) {
            tF1=vec4[0];
            tF2=vec4[1];
            tF3=vec4[2];
            tF4=vec4[3];
        }
        return b;
    }
    static bool DragFloat3(const char* tName, glm::vec3* tFloats, const float& tSpeed=0.1f) {
        vec3[0]=tFloats->x;
        vec3[1]=tFloats->y;
        vec3[2]=tFloats->z;
        bool b=ImGui::DragFloat3(tName, vec3, tSpeed);
        if(b) {
            tFloats->x=vec3[0];
            tFloats->y=vec3[1];
            tFloats->z=vec3[2];
        }
        return b;
    }
    static bool DragFloat3(const char* tName, float& tF1, float& tF2, float& tF3, const float& tSpeed=0.1f) {
        vec3[0]=tF1;
        vec3[1]=tF2;
        vec3[2]=tF3;
        bool b=ImGui::DragFloat3(tName, vec3, tSpeed);
        if(b) {
            tF1=vec3[0];
            tF2=vec3[1];
            tF3=vec3[2];
        }
        return b;
    }
    static bool DragFloat2(const char* tName, glm::vec2* tFloats, const float& tSpeed=0.1f) {
        vec2[0]=tFloats->x;
        vec2[1]=tFloats->y;
        bool b=ImGui::DragFloat2(tName, vec2, tSpeed);
        if(b) {
            tFloats->x=vec2[0];
            tFloats->y=vec2[1];
        }
        return b;
    }
    static bool DragFloat2(const char* tName, float& tF1, float& tF2, const float& tSpeed=0.1f) {
        vec2[0]=tF1;
        vec2[1]=tF2;
        bool b=ImGui::DragFloat2(tName, vec2, tSpeed);
        if(b) {
            tF1=vec2[0];
            tF2=vec2[1];
        }
        return b;
    }

    static bool SliderFloat4(const char* tName, glm::vec4* tFloats, const float& tMin, const float& tMax) {
        vec4[0]=tFloats->x;
        vec4[1]=tFloats->y;
        vec4[2]=tFloats->z;
        vec4[3]=tFloats->w;
        bool b=ImGui::SliderFloat4(tName, vec4, tMin, tMax);
        if(b) {
            tFloats->x=vec4[0];
            tFloats->y=vec4[1];
            tFloats->z=vec4[2];
            tFloats->w=vec4[3];
        }
        return b;
    }
    static bool SliderFloat4(const char* tName, float& tF1, float& tF2, float& tF3, float& tF4, const float& tMin, const float& tMax) {
        vec4[0]=tF1;
        vec4[1]=tF2;
        vec4[2]=tF3;
        vec4[3]=tF4;
        bool b=ImGui::SliderFloat4(tName, vec4, tMin, tMax);
        if(b) {
            tF1=vec4[0];
            tF2=vec4[1];
            tF3=vec4[2];
            tF4=vec4[3];
        }
        return b;
    }
    static bool SliderFloat3(const char* tName, glm::vec3* tFloats, const float& tMin, const float& tMax) {
        vec3[0]=tFloats->x;
        vec3[1]=tFloats->y;
        vec3[2]=tFloats->z;
        bool b=ImGui::SliderFloat3(tName, vec3, tMin, tMax);
        if(b) {
            tFloats->x=vec3[0];
            tFloats->y=vec3[1];
            tFloats->z=vec3[2];
        }
        return b;
    }
    static bool SliderFloat3(const char* tName, float& tF1, float& tF2, float& tF3, const float& tMin, const float& tMax) {
        vec3[0]=tF1;
        vec3[1]=tF2;
        vec3[2]=tF3;
        bool b=ImGui::SliderFloat3(tName, vec3, tMin, tMax);
        if(b) {
            tF1=vec3[0];
            tF2=vec3[1];
            tF3=vec3[2];
        }
        return b;
    }
    static bool SliderFloat2(const char* tName, glm::vec2* tFloats, const float& tMin, const float& tMax) {
        vec2[0]=tFloats->x;
        vec2[1]=tFloats->y;
        bool b=ImGui::SliderFloat2(tName, vec2, tMin, tMax);
        if(b) {
            tFloats->x=vec2[0];
            tFloats->y=vec2[1];
        }
        return b;
    }
    static bool SliderFloat2(const char* tName, float& tF1, float& tF2, const float& tMin, const float& tMax) {
        vec2[0]=tF1;
        vec2[1]=tF2;
        bool b=ImGui::SliderFloat2(tName, vec2, tMin, tMax);
        if(b) {
            tF1=vec2[0];
            tF2=vec2[1];
        }
        return b;
    }
    // Creates a dockspace for ImGui.
    // Use it every render frame before any other ImGui functions.
    void SetupDockspace(const char* tName, const bool& tEndDockWindow = true) {
        //Style failsafe.
        ImGui::PopStyleVar(3);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        //Setup viewport.
        const ImGuiViewport* viewport=ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGuiID dockspace_id=ImGui::GetID(tName);
        ImGui::Begin(tName, NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
            | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground);
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        //Reset styling.
        ImGui::PopStyleVar(1);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));
        if(tEndDockWindow) ImGui::End();
    }
}

#endif // !FS_IMGUI_UTILS