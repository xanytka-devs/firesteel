#define FS_PRINT_DEBUG_MSGS
#include <../include/firesteel.hpp>
#include <../include/input/input.hpp>
using namespace Firesteel;

Shader shader;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0, 0, -90));
Entity entity;

class ModelViewer : public Firesteel::App {
    virtual void onInitialize() override {
        shader=Shader("res\\ModelLoading\\shader.vs", "res\\ModelLoading\\shader.fs");
        std::vector<std::string> filters{
            "Any 3D Model (.fbx, .gltf, .glb, .obg)","*.fbx *.gltf *.glb *.obj",
            "FBX Model (.fbx)","*.fbx",
            "GLTF Model (.gltf, .glb)","*.gltf *.glb",
            "OBJ Model (.obj)","*.obj",
            "All Files","*"
        };
        auto val=OS::fileDialog(false, false, std::filesystem::current_path().string(),&filters);
        if(val.size()==0) val.push_back("res\\ModelLoading\\backpack.obj");
        entity.load(val[0]);
        camera.update();
        window.setResizability(false);
        window.setVSync(true);
    }
    virtual void onUpdate() override {
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
        //Get variables needed for a draw call.
        glm::mat4 proj = camera.getProjection(), view = camera.getView();
        camera.aspect = window.aspect();
        //Draw the model.
        shader.enable();
        shader.setMat4("projection", proj);
        shader.setMat4("view", view);
        entity.draw(&shader);
    }
    virtual void onShutdown() override {
        entity.remove();
        shader.remove();
    }
};

int main() {
    return ModelViewer{}.start();
}