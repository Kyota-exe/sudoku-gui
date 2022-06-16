#include "GUI.h"
#include "imgui.h"
#include "bindings/imgui_impl_glfw.h"
#include "bindings/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "style.h"
#include "State.h"
#include "Solver.h"

static GLFWwindow* window = nullptr;

void render_gui()
{
    const State render_state = State::copy_current();

    ImGui::SetNextWindowSize({527, 523}, ImGuiCond_Once);
    ImGui::SetNextWindowPos({50, 70}, ImGuiCond_Once);
    ImGui::Begin("Sudoku", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                                    ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus);

    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            int number = render_state.board[y][x];
            ImGui::Button(number == 0 ? "" : std::to_string(number).c_str(), {50, 50});
            if (x != 8)
            {
                float spacing = (x + 1) % 3 == 0 ? 15.0f : 5.0f;
                ImGui::SameLine(0.0f, spacing);
            }
        }
        if ((y + 1) % 3 == 0)
        {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
        }
    }
    ImGui::End();

    static bool started = false;
    if (started)
    {
        ImGui::SetNextWindowSize({500, 200}, ImGuiCond_Once);
        ImGui::SetNextWindowPos({145, 610});
        ImGui::Begin("Control", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                                         ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove |
                                         ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                         ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground);

        int original_delay = Solver::get_delay();
        int current_delay = original_delay;
        ImGui::SliderInt("", &current_delay, 0, 300);
        if (original_delay != current_delay)
        {
            Solver::set_delay(current_delay);
        }
        ImGui::End();
    }
    else
    {
        ImGui::SetNextWindowSize({500, 200}, ImGuiCond_Once);
        ImGui::SetNextWindowPos({210, 610});
        ImGui::Begin("Start", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                                       ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove |
                                       ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                       ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground);
        if (ImGui::Button("Start", {190, 50}))
        {
            Solver::start();
            started = true;
        }
        ImGui::End();
    }
}

void GUI::loop()
{
    if (!window)
    {
        throw std::runtime_error("GUI context not initialized");
    }

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Clear screen colour
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0352941f, 0.02745098f, 0.05078125f, 0.0f);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
        render_gui();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}

void error_callback(int error, const char* description)
{
    std::cerr << "Error (" << error << "): " << description << std::endl;
}

void GUI::init()
{
    if (!glfwInit())
    {
        throw std::runtime_error("GLFW init failed");
    }

    glfwSetErrorCallback(&error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(630, 720, "Sudoku", nullptr, nullptr);
    if (!window)
    {
        throw std::runtime_error("GLFW window init failed");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        throw std::runtime_error("Glew init failed");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGuiIO io = ImGui::GetIO();

    set_style();
    io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\corbel.ttf)", 40);
}

void GUI::cleanup()
{
    if (!window)
    {
        throw std::runtime_error("GUI context not initialized");
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}
