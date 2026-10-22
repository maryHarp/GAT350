
#include "Engine.h"
#include "Core/File.h"
#include "Math/Transform.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Pipeline.h"

#include "Resources/ResourceManager.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>


using namespace nu;

int main()
{
    SetWorkingDirectory("Assets");
   
    //INITIAL

    Engine::Get().Initialize();




    auto pipeline = std::make_shared<Pipeline>();

    pipeline->AddVertexBuffer(sizeof(Model::Vertex));
    pipeline->AddVertexAttribute(0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, position));
    pipeline->AddVertexAttribute(1, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offsetof(Model::Vertex, texcoord));
    pipeline->AddVertexAttribute(2, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, normal));
    pipeline->AddVertexAttribute(3, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, tangent));

    auto model = Resources().Get<Model>("models/spot.obj", Engine::Get().GetRenderer());
    auto texture = Resources().Get<Texture>("textures/spot_diffuse.png", Engine::Get().GetRenderer());

    auto vshader = Resources().Get<nu::Shader>("shaders/lit.vert", Engine::Get().GetRenderer(), 2);
    auto fshader = Resources().Get<nu::Shader>("shaders/lit.frag", Engine::Get().GetRenderer(), 1, 1);

    pipeline->Create(*vshader.get(), *fshader.get(),
        Engine::Get().GetRenderer().GetGPUDevice(),
        Engine::Get().GetRenderer().GetWindow());


    //uniforms
    glm::vec4 color { 1,1,1,1 };

    struct VertexUniform
    {

        glm::mat4 mvp;
        glm::mat4 model;
    };

    VertexUniform vertexUniform;

    struct LightUnifrom 
    {
        glm::vec3 lightDirection;
        float padding1;
        glm::vec3 lightColor;
        float lightIntensity;
        glm::vec3 ambient;
        float padding2;


    };

    LightUnifrom lightUniform{
        .lightDirection{0, -1, 0},
        .lightColor{0,0,1},
        .lightIntensity{1},
        .ambient{0.1f}
    };

    glm::vec3 cameraPosition{ 0,0,5 };

    Transform transform; 




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

        //color.r = sin(Engine::Get().GetTime().GetTime() * 3.0f) * 0.5f + 0.5f;

        glm::vec3 direction{ 0 };
        if ((Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A))) direction.x = -1;
        if ((Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D))) direction.x = +1;
        if ((Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W))) direction.y = -1;
        if ((Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S))) direction.y = +1;
        if ((Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q))) direction.z = -1;
        if ((Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_E))) direction.z = +1;

        cameraPosition += direction * 4.0f * dt;

        lightUniform.lightIntensity = std::sin(time) * 0.5f + 0.5f;

        //transform
        transform.rotation.y = time * 90;


        // view matrix
        auto view = glm::lookAtRH(cameraPosition, cameraPosition + glm::vec3{ 0 , 0, -1}, glm::vec3{ 0,1,0 });

        // projection matrix
        float aspectRatio = Engine::Get().GetRenderer().GetWidth() / (float)Engine::Get().GetRenderer().GetHeight();
        auto projection = glm::perspective(glm::radians(70.0f), aspectRatio, 0.01f, 20.0f);

        //mvp
        vertexUniform.model = transform.GetMatrix();
        vertexUniform.mvp = projection * view * vertexUniform.model;


        //render
        
        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetRenderer().SetPipeline(*pipeline);

        Engine::Get().GetRenderer().setVertexUniform(0, &vertexUniform, sizeof(vertexUniform));
        Engine::Get().GetRenderer().setVertexUniform(1, &lightUniform, sizeof(lightUniform));
        Engine::Get().GetRenderer().setFragmentUniform(0, &color, sizeof(color));
        Engine::Get().GetRenderer().SetFragmentTexture(0, *texture);


        model->Draw(Engine::Get().GetRenderer());

        Engine::Get().GetRenderer().EndFrame();
    }


    //shutdown 
    Engine::Get().Shutdown();

    return 0;
}

