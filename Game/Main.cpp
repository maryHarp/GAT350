
#include "Engine.h"
#include "Core/File.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/Pipeline.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Resources/ResourceManager.h"

using namespace nu;

namespace nu{
    struct Color4 {
        float r, g, b, a;
    };

}

struct Vertex
{
    Vector3 position;
    Color4 color;
};

std::vector<Vertex> vertices =
{
    Vertex{ Vector3{-0.8f, 0.0f, 0.0f }, Color4{1, 0, 0 ,1 }}, // Bottom-Left
    Vertex{ Vector3{0.8f, 0.0f, 0.0f }, Color4{0, 1, 0 ,1 }}, // Bottom-Right
    Vertex{ Vector3{0.0f,  0.5f, 0.0f }, Color4{0, 0, 1 ,1 }}, // Top-Middle
};


int main()
{
    SetWorkingDirectory("Assets");
   
    //INITIAL

    Engine::Get().Initialize();

    auto vb = std::make_shared<VertexBuffer>();
    vb->Create<Vertex>(vertices, Engine::Get().GetRenderer().GetGPUDevice());

    auto vshader = Resources().Get<nu::Shader>("shaders/position.vert", Engine::Get().GetRenderer(), 1);
    auto fshader = Resources().Get<nu::Shader>("shaders/color.frag", Engine::Get().GetRenderer(), 1);

    auto pipeline = std::make_shared<Pipeline>();
    pipeline->AddVertexBuffer(sizeof(Vertex));
    //position
    pipeline->AddVertexAttribute(
        0,
        SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
        offsetof(Vertex, position));
    //color
    pipeline->AddVertexAttribute(
        1,
        SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
        offsetof(Vertex, color));


    pipeline->Create(*vshader.get(), *fshader.get(),
        Engine::Get().GetRenderer().GetGPUDevice(),
        Engine::Get().GetRenderer().GetWindow());


    Color4 color{ 0,0,0,1 };

    struct vertexUniform
    {
        glm::mat4 model;
    };

    vertexUniform uniform;

    //main loop
    bool quit = false;
    
    while (!quit) {
        

        //update
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
                quit = true;
            }

        }

        Engine::Get().Update();
   
        float dt = Engine::Get().GetTime().GetDeltaTime();
        float time = Engine::Get().GetTime().GetTime();

        color.r = sin(Engine::Get().GetTime().GetTime() * 10.0f) * 0.5f + 0.5f;

        uniform.model = glm::mat4(1.0f);
        uniform.model = glm::translate(uniform.model, glm::vec3(0.5f, 0.0f, 0.0f));
        uniform.model = glm::rotate(uniform.model, time * 3.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        uniform.model = glm::scale(uniform.model, glm::vec3(0.5f, 0.5f, 0.5f));


        //render
        // RENDER
        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetRenderer().SetPipeline(*pipeline);
        Engine::Get().GetRenderer().SetVertexBuffer(*vb);
        Engine::Get().GetRenderer().setVertexUniform(0, &uniform, sizeof(uniform));
        Engine::Get().GetRenderer().setFragmentUniform(0, &color, sizeof(color));
       
        Engine::Get().GetRenderer().Draw(vb->GetVertexCount());

        Engine::Get().GetRenderer().EndFrame();
    }


    //shutdown 
    Engine::Get().Shutdown();

    return 0;
}

