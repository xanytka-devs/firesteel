#include <../include/app.hpp>

class WindowTest : public Firesteel::App { };

int main() {
    return WindowTest{}.start();
}