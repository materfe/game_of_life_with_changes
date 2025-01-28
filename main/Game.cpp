#include "imgui.h"
#include "imgui_impl_sdl2.h"

#include "ivec2.h"

#include <cassert>
#include <chrono>
#include <SDL.h>

class GameEngine
{
private:
    void Init();
    void End() const;
    SDL_Window* window_ = nullptr;
public:
    void Run();
};

void GameEngine::Init() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
    // Set our OpenGL version.
#if true
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#endif

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    const auto windowSize = Vec2i(1200, 800);
    window_ = SDL_CreateWindow(
            "window for samples BOY",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            windowSize.x,
            windowSize.y,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
    );
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Keyboard Gamepad
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
}

void GameEngine::Run()
{
    Init();
    bool isOpen = true;

    std::chrono::time_point<std::chrono::system_clock> clock = std::chrono::system_clock::now();
    while (isOpen)
    {
        const auto start = std::chrono::system_clock::now();
        using seconds = std::chrono::duration<float, std::ratio<1, 1>>;
        const auto dt = std::chrono::duration_cast<seconds>(start - clock);
        clock = start;

        //Manage SDL event
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            switch (event.type)
            {
                case SDL_QUIT:
                    isOpen = false;
                    break;
                case SDL_WINDOWEVENT:
                {
                    switch (event.window.event)
                    {
                        case SDL_WINDOWEVENT_CLOSE:
                            isOpen = false;
                            break;
                        case SDL_WINDOWEVENT_RESIZED:
                        {
                            glm::uvec2 newWindowSize;
                            newWindowSize.x = event.window.data1;
                            newWindowSize.y = event.window.data2;
                            //TODO do something with the new size
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        isOpen = false;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if (!ImGui::GetIO().WantCaptureMouse) {
                            SDL_SetRelativeMouseMode(SDL_TRUE);
                        }
                        SDL_ShowCursor(SDL_DISABLE);
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if (!ImGui::GetIO().WantCaptureMouse) {
                            SDL_SetRelativeMouseMode(SDL_FALSE);
                        }
                        SDL_ShowCursor(SDL_ENABLE);
                    }
                    break;
                default:
                    break;
            }

        }
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::Render();
    }
    End();
}

void GameEngine::End() const {
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    GameEngine engine;
    engine.Run();

    return EXIT_SUCCESS;
}








