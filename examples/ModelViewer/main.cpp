#define FS_PRINT_DEBUG_MSGS
#include <../include/firesteel.hpp>
#include <../include/input/input.hpp>
using namespace Firesteel;

std::shared_ptr<Shader> shader;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0, 0, -90));
Entity entity;
Node* selectedNode=nullptr;
bool selectedRoot=false;
std::vector<std::string> filters{
    "Any 3D Model (.fbx, .gltf, .glb, .obj)","*.fbx *.gltf *.glb *.obj",
    "FBX Model (.fbx)","*.fbx",
    "GLTF Model (.gltf, .glb)","*.gltf *.glb",
    "OBJ Model (.obj)","*.obj",
    "All Files","*"
};

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
static void DropDownNodes(Node* tNode,std::string tPath) {
    if(ImGui::MenuItem((tPath+"/"+tNode->name).c_str())) {selectedNode=tNode;selectedRoot=false;}
    for(size_t n=0;n<tNode->children.size();n++)
        DropDownNodes(&tNode->children[n],tPath+"/"+tNode->name);
}

class ModelViewer : public Firesteel::App {
    virtual void onInitialize() override {
        shader=std::make_shared<Shader>("res\\ModelLoading\\shader.vs", "res\\ModelLoading\\shader.fs");
        auto val=OS::fileDialog(false, false, std::filesystem::current_path().string(),&filters);
        if(val.size()==0) val.push_back("res\\ModelLoading\\backpack.obj");
        entity.load(val[0]);
        entity.setMaterialsShader(shader);
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
        glm::mat4 proj = camera.getProjection(), view = camera.getView();
        camera.aspect = window.aspect();
        //Draw the model.
        shader->enable();
        shader->setMat4("projection", proj);
        shader->setMat4("view", view);
        entity.draw();
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
            entity.replaceMaterialsShader(oldId,shader);
        }
        if(entity.hasModel()) {
            if(ImGui::CollapsingHeader(entity.model.getFilename().c_str())) {
                if(ImGui::MenuItem("[Root]")) {selectedNode=nullptr;selectedRoot=true;}
                for(size_t n=0;n<entity.model.nodes.size();n++)
                    DropDownNodes(&entity.model.nodes[n],"");
            } else {selectedNode=nullptr;selectedRoot=false;}
        }
        ImGui::End();
        if(selectedNode||selectedRoot) {
            ImGui::Begin("Inspector");
            if(selectedNode) {
                ImGui::Text(selectedNode->name.c_str());
                ImGui::Separator();
                ImGui::Text("Transform");
                DragFloat3("Position",&selectedNode->transform.position);
                DragFloat3("Rotation",&selectedNode->transform.rotation);
                DragFloat3("Size",&selectedNode->transform.size);
                ImGui::Separator();
                ImGui::Text("Material");
                if(entity.model.meshes[selectedNode->index].material) {
                    auto& mat=entity.model.meshes[selectedNode->index].material;
                    ImGui::Text("ID: %i", selectedNode->index);
                    ImGui::Text("Shader ID: %i", mat->getShader()->ID);
                    ImGui::Text("Parameters count: %i", mat->params.size());
                    ImGui::Text("Textures count: %i", mat->textures.size());
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
                            ImGui::Text("Shader ID: %i", mat->getShader()->ID);
                            ImGui::Text("Parameters count: %i", mat->params.size());
                            ImGui::Text("Textures count: %i", mat->textures.size());
                        }
                    }
                    
                }
            }
            ImGui::End();
        }
    }
    virtual void onShutdown() override {
        entity.remove();
    }
};

int main() {
    return ModelViewer{}.start();
}