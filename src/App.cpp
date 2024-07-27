#include <App.hpp>

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/implot.h>

#include <Framebuffer.hpp>
#include <lenia/Lenia.hpp>
#include <lenia/LeniaTemplate.h>

#include <chrono>
#include <thread>

#include <vector>

void parse(std::string str, Grid& grid) {
    int x = 54;
    int y = 54;
    std::string cell = "";
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ',') {
            grid.set(x, y, std::stod(cell));
            cell = "";
            x++;
        } else if (str[i] == '/') {
            grid.set(x, y, std::stod(cell));
            cell = "";
            x = 54;
            y++;
        } else {
            cell += str[i];
        }
    }
}

void App::run() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1280, 720, "Lenia++", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return;
    }

    glViewport(0, 0, 1280, 720);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    LeniaTemplate leniaTemplate = LeniaTemplate();

    leniaTemplate.updateFrequency = 10;

    Lenia lenia(128, 128, leniaTemplate);

    lenia.parse("R=18;k=quad4(1,1/3);d=quad4(0.25,0.03)*0.1;cells=0,0,0,0,0,0,0,0,0,0,0,0,0.033424,0.419191,0.575492,0.605072,0.586909,0.409731,0,0,0/0,0,0,0,0,0,0,0,0,0.534431,0.71555,0.866719,0.954007,0.984165,0.978722,0.928526,0.83627,0.7551,0.703546,0.173241,0/0,0,0,0,0,0,0.3104,0.72981,0.803238,0.899622,0.980859,1,1,1,1,1,0.998296,0.877506,0.762396,0.705294,0.043431/0,0,0,0.331086,0.566097,0.86687,0.947779,0.992357,1,0.453233,0.36684,0.287955,0.185868,0.505089,0.575288,0.883505,1,1,0.890394,0.753339,0.441117/0,0,0.569776,0.844285,0.916773,1,1,1,0.294423,0.233356,0.19393,0.141131,0.055147,0,0.054776,0.159762,0.525324,1,1,0.842837,0.558735/0,0.373808,0.854214,0.900566,1,1,1,0.276,0.064531,0.106697,0.138736,0.140799,0.078627,0,0,0.016817,0.110743,0.666998,1,0.939588,0.546418/0,0.771822,0.873032,0.9588,1,0.948526,0.218024,0,0,0.079984,0.230533,0.326668,0.371327,0.621721,0.163791,0,0,0.305775,1,0.987251,0.743893/0.011195,0.858954,0.888521,0.995015,1,0.399933,0.041176,0,0,0.187902,0.444269,0.25343,0.354694,1,1,0,0,0.026414,1,0.99929,0.7418/0.086702,0.858515,0.898522,1,1,0.246327,0.017256,0,0.068599,0.836474,0.326515,0.094562,0.153829,0.430216,1,0.101059,0.017198,0.072865,0.608108,0.98997,0.691148/0.0137,0.851115,0.898572,1,1,0.313138,0.009582,0,0.214462,0.881106,0.280836,0.088129,0.060751,0.144847,0.735773,0.213928,0.064466,0.128274,0.358116,0.968505,0.59327/0,0.588157,0.87189,0.992236,1,0.709643,0.061128,0,0.081501,0.49421,0.082618,0.100734,0.124441,0.188233,0.678219,0.197909,0.092894,0.17204,0.401122,0.916357,0.382958/0,0.138097,0.606145,0.93976,1,1,0.090806,0.049936,0.120229,0.320113,0.576967,0.269626,0.627299,1,0.505278,0.109584,0.095644,0.218831,0.459262,0.841603,0/0,0,0.016688,0.837126,0.977401,1,0.337857,0.185401,0.144064,0.181348,0.289209,0.683256,1,0.970144,0.026589,0.026312,0.092497,0.282652,1,0.75307,0/0,0,0,0,0.807353,0.960297,1,0.330342,0.202457,0.12698,0.067806,0.009687,0.019599,0.019518,0,0,0.110157,1,0.980233,0.660666,0/0,0,0,0,0,0.723638,0.928295,0.505763,0.322371,0.155058,0.032689,0,0,0,0.034073,0.280279,1,1,0.899999,0,0/0,0,0,0,0,0,0.660266,0.928433,1,0.504147,0.099747,0.136311,0.069731,0.113926,0.331142,0.941586,1,0.975989,0.762454,0,0/0,0,0,0,0,0,0,0.66721,0.946846,1,1,1,0.823441,0.949414,1,1,0.97419,0.824723,0.185633,0,0/0,0,0,0,0,0,0,0,0.605209,0.925066,1,1,1,1,0.981979,0.898481,0.79604,0.620238,0,0,0/0,0,0,0,0,0,0,0,0,0.081142,0.811809,0.871226,0.874467,0.833333,0.776943,0.725314,0.695153,0.271033,0,0,0/0,0,0,0,0,0,0,0,0,0,0,0.384868,0.653007,0.709609,0.688523,0.53173,0.142483,0,0,0,0");

    grid = &lenia;

    Framebuffer framebuffer(1280, 720);

    double lastTime = glfwGetTime();
    int nbFrames = 0;

    int fpsCount = 0;
    double fpsSum = 0.0;


    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) { // If last print was more than 1 sec ago
            double fps = double(nbFrames) / (currentTime - lastTime);
            std::string windowTitle = "Lenia++ - FPS: " + std::to_string(fps);
            glfwSetWindowTitle(window, windowTitle.c_str());
            nbFrames = 0;
            lastTime = currentTime;

            fpsSum += fps;

            std::cout << "Average FPS: " << fpsSum / ++fpsCount << std::endl;
        }

        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        
        glClearColor(0.137, 0.09, 0.106, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::NewFrame();

        drawMenuBar();

        ImGui::Begin("Spork Viewer", nullptr, ImGuiWindowFlags_NoResize);

        ImGui::SetWindowSize(ImVec2(grid->getWidth() * 3, grid->getHeight() * 3));

        const float width = ImGui::GetContentRegionAvail().x;
        const float height = ImGui::GetContentRegionAvail().y;

        framebuffer.resize(width, height);
        glViewport(0, 0, width, height);

        ImVec2 pos = ImGui::GetCursorScreenPos();

        ImGui::GetWindowDrawList()->AddImage(
            (void*)(intptr_t)framebuffer.getTexture(),
            pos,
            ImVec2(pos.x + width, pos.y + height),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

        ImGui::Text("FPS: %.1f", io.Framerate);

        ImGui::End();

        grid->drawImGui();

        ImGui::Render();

        framebuffer.bind();
        framebuffer.clear();

        grid->draw();

        framebuffer.unbind();
        
        if (!paused) {
            grid->update();
            grid->updateStats(true);
        }

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwTerminate();
}

void App::drawMenuBar() {
    std::string menuAction = "";
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save")) {
                menuAction = "save";
            }

            if (ImGui::MenuItem("Quit")) {
                glfwSetWindowShouldClose(window, true);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Reset")) {
                grid->reset();
            }

            if (ImGui::MenuItem("Randomize")) {
                grid->randomize();
            }

            if (ImGui::MenuItem("Randomize Patches")) {
                grid->randomizePatches();
            }

            if (ImGui::MenuItem("Edit Random Patches")) {
                menuAction = "editRandomPatches";
            }
            
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Pause")) {
                paused = !paused;
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Simulation")) {
            if (ImGui::MenuItem("Step")) {
                grid->update();
            }
            
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (menuAction == "save") {
        ImGui::OpenPopup("Save Video");
        paused = true;
    }

    if (menuAction == "editRandomPatches") {
        ImGui::OpenPopup("Edit Random Patches");
        paused = true;
    }

    if (ImGui::BeginPopupModal("Edit Random Patches", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::SliderInt("Patches", &grid->randomPatches, 1, 10);
        ImGui::SliderInt("Patch Size", &grid->randomPatchSize, 1, 30);

        if (ImGui::Button("Close")) {
            paused = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupModal("Save Video", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char filename[128] = "out.mp4";
        static int frameCount = 120;
        static int fps = 60;

        ImGui::InputText("Filename", filename, 128);
        ImGui::InputInt("Frame Count", &frameCount);
        ImGui::InputInt("FPS", &fps);
        
        if (ImGui::Button("Save")) {
            grid->exportToVideo(filename, frameCount, fps);
            paused = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            paused = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}