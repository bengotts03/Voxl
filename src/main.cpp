#include "Core/Application.h"
#include "GLFW/glfw3.h"

int main() {
    Application app;

    app.Start();

    while (!glfwWindowShouldClose(app.GetWindow().GetNativeWindow()))
    {
        app.Update();
    }

    app.Shutdown();

    return 0;
}
