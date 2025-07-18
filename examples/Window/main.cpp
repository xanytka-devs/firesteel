#include <../include/app.hpp>
using namespace Firesteel;

class WindowTest : public Firesteel::App {
    virtual void onInitialize() override {
        LOG_INFO("Hello World!");
    }
};

int main() {
    return WindowTest{}.start();
}