#include <iostream>
#include "Engine.cpp"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

int main()
{
    

    Engine game;
    game.setup();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(game.getWindow()->getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 430");

    ImGui::StyleColorsDark();

    while (game.isOn()) {
        game.Update();
    }
    game.close();

    return 0;
}



