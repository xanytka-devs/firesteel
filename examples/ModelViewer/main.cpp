#define FS_PRINT_DEBUG_MSGS
#include <../include/firesteel.hpp>
#include <../include/input/input.hpp>
using namespace Firesteel;

std::shared_ptr<Shader> shader;
std::shared_ptr<Shader> selected;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0, 0, -90));
Entity entity;
std::shared_ptr<Node> selectedNode=nullptr;
glm::mat4 nodeScaledMat(1);
bool selectedRoot=false;
std::vector<std::string> filters{
    "Any 3D Model (.fbx, .gltf, .glb, .obj)","*.fbx *.gltf *.glb *.obj",
    "FBX Model (.fbx)","*.fbx",
    "GLTF Model (.gltf, .glb)","*.gltf *.glb",
    "OBJ Model (.obj)","*.obj",
    "All Files","*"
};

const char* defaultVertShader="#version 330 core\
layout(location = 0) in vec3 aPos;\
layout(location = 2) in vec2 aUV;\
out vec3 frag_POS;\
out vec2 frag_UV;\
uniform mat4 model;\
uniform mat4 view;\
uniform mat4 projection;\
void main() {\
    frag_POS = vec3(model * vec4(aPos, 1.0));\
    frag_UV = aUV;\
    gl_Position = projection * view * vec4(frag_POS, 1.0);\
}";
const char* defaultFragShader="#version 330 core\
out vec4 frag_COLOR;\
in vec3 frag_POS;\
in vec2 frag_UV;\
struct Material {\
    sampler2D diffuse0;\
    bool diffuse0_isMonochrome;\
    vec3 diffuse;\
    bool opacityMask;\
    sampler2D opacity0;\
};\
uniform Material material;\
uniform bool noTextures;\
void main() {\
    // material values\
    vec4 diffMap = vec4(material.diffuse, 1);\
    vec4 opacMap = vec4(1);\
    float transparency = 1.0;\
    // texture values\
    if (!noTextures) {\
        diffMap = texture(material.diffuse0, frag_UV);\
        if (material.diffuse0_isMonochrome)\
            diffMap = vec4(vec3(diffMap.r), 1);\
        opacMap = texture(material.opacity0, frag_UV);\
        transparency = diffMap.a;\
        if (transparency < 0.1) discard;\
        if (material.opacityMask) transparency = opacMap.r;\
    }\
    frag_COLOR = vec4(diffMap.rgb, transparency);\
}";
const char* selectionFragShader="#version 330 core\
out vec4 frag_COLOR;\
in vec3 frag_POS;\
in vec2 frag_UV;\
void main() {\
    frag_COLOR = vec4(1);\
}";

float vec3[3]={0,0,0};
static bool DragFloat3(const char* tName, glm::vec3* tFloats, const float& tSpeed = 0.1f) {
    vec3[0] = tFloats->x;
    vec3[1] = tFloats->y;
    vec3[2] = tFloats->z;
    bool b = ImGui::DragFloat3(tName, vec3, tSpeed);
    if(b) {
        tFloats->x = vec3[0];
        tFloats->y = vec3[1];
        tFloats->z = vec3[2];
    }
    return b;
}
static void DropDownNodes(std::shared_ptr<Node> tNode,std::string tPath) {
    if(ImGui::MenuItem((tPath+tNode->name+"/").c_str())) {selectedNode=tNode;selectedRoot=false;}
    for(size_t n=0;n<tNode->children.size();n++)
        DropDownNodes(tNode->children[n],tPath+tNode->name+"/");
}

class ModelViewer : public Firesteel::App {
    virtual void onInitialize() override {
        //Load base shader.
        if(std::filesystem::exists("res\\ModelLoading\\shader.vs")&&std::filesystem::exists("res\\ModelLoading\\shader.fs"))
            shader=std::make_shared<Shader>("res\\ModelLoading\\shader.vs", "res\\ModelLoading\\shader.fs");
        else shader=std::make_shared<Shader>(defaultVertShader,defaultFragShader,false,nullptr);
        //Load selection shader.
        if(std::filesystem::exists("res\\ModelLoading\\shader.vs")&&std::filesystem::exists("res\\ModelLoading\\selected.fs"))
            selected=std::make_shared<Shader>("res\\ModelLoading\\shader.vs", "res\\ModelLoading\\selected.fs");
        else selected=std::make_shared<Shader>(defaultVertShader,selectionFragShader,false,nullptr);
        //Load model.
        auto val=OS::fileDialog(false, false, std::filesystem::current_path().string(),&filters);
        if(val.size()>0) {
            entity.load(val[0]);
            entity.setMaterialsShader(shader);
        }
        //Additional usefulness.
        camera.update();
        window.setResizability(false);
        window.setVSync(true);
    }
    virtual void onUpdate() override {
        if(!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
            //Process input.
            if(Keyboard::keyDown(KeyCode::KEY_1)) enviroment.renderer->setDrawMode(Renderer::DM_FILL);
            if(Keyboard::keyDown(KeyCode::KEY_2)) enviroment.renderer->setDrawMode(Renderer::DM_WIRE);
            if(Mouse::getButton(2)) window.setCursor(Window::Cursor::CUR_POINTING_HAND);
            else window.setCursor(Window::Cursor::CUR_ARROW);
            //Viewport movement.
            float movement=0;
            //Horizontal.
            movement=-Input::getHorizontalAxis()+(Mouse::getButton(2)?Mouse::getCursorDX():0)*2;
            entity.transform.rotation+=glm::vec3(0,15*deltaTime*movement,0);
            //Vertical.
            movement=Input::getVerticalAxis()+(Mouse::getButton(2)?-Mouse::getCursorDY():0)*2;
            entity.transform.rotation+=glm::vec3(15*deltaTime*movement,0,0);
            //Zoom.
            entity.transform.size+=glm::vec3(Mouse::getWheelDY()*deltaTime*10);
            if(entity.transform.size.x<0) entity.transform.size*=-1;
        } else Mouse::getWheelDY();
        //Get variables needed for a draw call.
        glm::mat4 proj=camera.getProjection(), view=camera.getView();
        camera.aspect=window.aspect();
        //Draw the model.
        shader->enable();
        shader->setMat4("projection", proj);
        shader->setMat4("view", view);
        entity.draw();
        //Draw selection.
        if(selectedNode) {
            //Get scaled transform.
            glm::mat4 origMat=selectedNode->getMatrix(entity.transform.getMatrix());
            nodeScaledMat =glm::scale(origMat, glm::vec3(1.05f));
            nodeScaledMat=glm::translate(nodeScaledMat, glm::vec3(-selectedNode->transform.position*0.05f));
            //Draw selected mesh.
            glStencilFunc(GL_NOTEQUAL,1,0xFF);
            glStencilMask(0x00);
            glDisable(GL_DEPTH_TEST);
            selected->enable();
            selected->setMat4("projection", proj);
            selected->setMat4("view", view);
            selected->setMat4("model", nodeScaledMat);
            entity.drawNode(selectedNode, nodeScaledMat, true);
            //Disable selection mask.
            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS,1,0xFF);
            //Draw selection one more time to make so selection just highlights it.
            entity.drawNode(selectedNode, origMat);
            glEnable(GL_DEPTH_TEST);
        }
        ImGui::Begin("Scene");
        if(ImGui::Button("Change model")) {
            selectedNode=nullptr;selectedRoot=false;
            auto val=OS::fileDialog(false, false, std::filesystem::current_path().string(),&filters);
            if(val.size()>0) {
                entity.load(val[0]);
                entity.setMaterialsShader(shader);
            }
        }
        ImGui::SameLine();
        if(ImGui::Button("Reload shaders")) {
            const unsigned int oldId=shader->ID;
            shader->remove();
            shader=std::make_shared<Shader>("res\\ModelLoading\\shader.vs", "res\\ModelLoading\\shader.fs");
            selected=std::make_shared<Shader>("res\\ModelLoading\\shader.vs", "res\\ModelLoading\\selected.fs");
            entity.replaceMaterialsShader(oldId,shader);
        }
        if(entity.hasModel()) {
            if(ImGui::CollapsingHeader(entity.model.getFilename().c_str())) {
                if(ImGui::MenuItem("[Root]")) {selectedNode=nullptr;selectedRoot=true;}
                for(size_t n=0;n<entity.model.nodes.size();n++)
                DropDownNodes(entity.model.nodes[n],"");
            } else {selectedNode=nullptr;selectedRoot=false;}
        } else ImGui::Text("No model given");
        ImGui::End();
        if(entity.hasModel()&&(selectedNode||selectedRoot)) {
            ImGui::Begin("Inspector");
            if(selectedNode) {
                ImGui::Text(selectedNode->name.c_str());
                int meshId=selectedNode->index;
                ImGui::DragInt("Mesh ID:",&meshId,1,-1,static_cast<int>(entity.model.meshes.size()-1));
                if(selectedNode->index!=meshId) selectedNode->index=meshId;
                ImGui::Separator();
                ImGui::Text("Transform");
                DragFloat3("Position",&selectedNode->transform.position);
                DragFloat3("Rotation",&selectedNode->transform.rotation);
                DragFloat3("Size",&selectedNode->transform.size);
                ImGui::Separator();
                ImGui::Text("Material");
                if(selectedNode->index!=-1&&entity.model.materials.size()>0) {
                    if(entity.model.meshes[selectedNode->index].material) {
                        auto& mat=entity.model.meshes[selectedNode->index].material;
                        if(mat->getShader()) ImGui::Text("Shader ID: %i", mat->getShader()->ID);
                        if(ImGui::CollapsingHeader(Log::formatStr("Parameters (%i)", mat->params.size()).c_str()))
                            for(size_t p=0;p<mat->params.size();p++)
                                ImGui::Text("%s [%s]",mat->params[p].name(),mat->params[p].type().c_str());
                        if(ImGui::CollapsingHeader(Log::formatStr("Textures (%i)", mat->textures.size()).c_str()))
                            for(size_t t=0;t<mat->textures.size();t++)
                                ImGui::Text("%s [%i]",mat->textures[t].typeToString(),mat->textures[t].ID);
                    } else ImGui::Text("None");
                } else ImGui::Text("None");
            } else if(selectedRoot) {
                ImGui::Text("[Root]");
                ImGui::Separator();
                ImGui::Text("Transform");
                DragFloat3("Position",&entity.transform.position);
                DragFloat3("Rotation",&entity.transform.rotation);
                DragFloat3("Size",&entity.transform.size);
                ImGui::Separator();
                ImGui::Text("Materials");
                if(entity.model.materials.size()>0) {
                    for(size_t m=0;m<entity.model.materials.size();m++) {
                        if(ImGui::CollapsingHeader(Log::formatStr("Material (ID:%i)", m).c_str())) {
                            auto& mat=entity.model.meshes[m].material;
                            if(mat->getShader()) ImGui::Text("Shader ID: %i", mat->getShader()->ID);
                            if(ImGui::CollapsingHeader(Log::formatStr("Parameters (%i)", mat->params.size()).c_str()))
                                for(size_t p=0;p<mat->params.size();p++)
                                    ImGui::Text("%s [%s]",mat->params[p].name(),mat->params[p].type().c_str());
                            if(ImGui::CollapsingHeader(Log::formatStr("Textures (%i)", mat->textures.size()).c_str()))
                                for(size_t t=0;t<mat->textures.size();t++)
                                    ImGui::Text("%s [%i]",mat->textures[t].typeToString(),mat->textures[t].ID);
                        }
                    }
                    
                }
            }
            ImGui::End();
        }
    }
    virtual void onShutdown() override {
        entity.remove();
        shader->remove();
    }
};

int main() {
    return ModelViewer{}.start();
}