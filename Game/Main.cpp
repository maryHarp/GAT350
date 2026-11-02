
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

#define CHECK_UNIFORM_BOUNDARY(T) static_assert((sizeof(T) % 16) == 0);
#define MAX_LIGHTS 8

using namespace nu;

enum class LightType: uint32_t 
{
    Directional,
    Point,
    Spot
};

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

    auto model = Resources().Get<Model>("models/dragon.obj", Engine::Get().GetRenderer());
    auto texture = Resources().Get<Texture>("textures/lava_diffuse.png", Engine::Get().GetRenderer());

    auto vshader = Resources().Get<nu::Shader>("shaders/phong.vert", Engine::Get().GetRenderer(), 3);
    auto fshader = Resources().Get<nu::Shader>("shaders/phong.frag", Engine::Get().GetRenderer(), 3, 1);

    pipeline->Create(*vshader.get(), *fshader.get(),
        Engine::Get().GetRenderer().GetGPUDevice(),
        Engine::Get().GetRenderer().GetWindow());


    //uniforms
    
    struct VertexUniform
    {
        glm::mat4 mvp;
        glm::mat4 model;
        glm::vec3 cameraPosition;
        float padding1;

    };

    CHECK_UNIFORM_BOUNDARY(VertexUniform);

    VertexUniform vertexUniform;

    struct Light {
        glm::vec3 position;
        LightType type;
        glm::vec3 direction;
        float range;
        glm::vec3 color;
        float intensity;
        glm::vec3 ambient;
        float innerCutoff;
        float outerCutoff;
        glm::vec3 padding;
    };

    struct LightUniform {
        Light lights[MAX_LIGHTS];
        uint32_t numLights;
        glm::vec3 ambient;
    };


    CHECK_UNIFORM_BOUNDARY(LightUniform)
    LightUniform lightUniform;

    Light light1{
        .position{20, 10, 10},
        .type{ LightType::Directional },
        .direction{0, -1, 0},
        .range{200},
        .color{1,1,1},
        .intensity{0.8},
        .ambient{0.3f},
        .innerCutoff{0.8f},
        .outerCutoff{1.6f}
    };
    Light light2{
        .position{-20, 15, 10},
        .type{ LightType::Point },
        .direction{0, -1, 0},
        .range{200},
        .color{0,0,1},
        .intensity{0.8},
        .ambient{0.5f},
        .innerCutoff{0.3f},
        .outerCutoff{1.6f}
    };
    Light light3{
        .position{0, 60, 600},
        .type{ LightType::Directional},
        .direction{0, -1, 0},
        .range{200},
        .color{1,0,0},
        .intensity{0.8},
        .ambient{0.5f},
        .innerCutoff{0.3f},
        .outerCutoff{1.6f}
    };

    lightUniform.lights[0] = light1;
    lightUniform.lights[1] = light2;
    //lightUniform.lights[2] = light3;
    lightUniform.numLights = 2;
    lightUniform.ambient = glm::vec3{ 0.2f };

    struct MaterialUniform {
        glm::vec4 color;
        glm::vec2 tiling;
        glm::vec2 offset;
        float shininess;
        glm::vec3 padding;
    };

    CHECK_UNIFORM_BOUNDARY(MaterialUniform)

    MaterialUniform materialUniform{
        .color{1, 1, 1, 1},
        .tiling{10.0f, 10.0f },
        .offset{0.0f, 0.0f},
        .shininess{32.0f}
    };

    vertexUniform.cameraPosition = { 0,3,8 };

    Transform transform; 
        transform.scale = glm::vec3{ 2.0f };




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

        glm::vec3 direction{ 0 };
        if ((Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A))) direction.x = -1;
        if ((Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D))) direction.x = +1;
        if ((Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W))) direction.y = -1;
        if ((Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S))) direction.y = +1;
        if ((Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q))) direction.z = -1;
        if ((Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_E))) direction.z = +1;


        vertexUniform.cameraPosition += direction * 4.0f * dt;

        //lightUniform.lights[0].position = glm::eulerAngleY(time) * glm::vec4{ 0,0,10, 1};
        //lightUniform.lights[1].position = glm::eulerAngleX(time * 0.5f) * glm::vec4{ 0,0,10, 1};

        /*if ((Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_1))) light1.type = LightType::Directional;
        if ((Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_2))) light1.type = LightType::Point;
        if ((Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_3))) {
            light1.type = LightType::Spot;
            light1.position = glm::vec3{ 0, 10, 0 };
            light1.innerCutoff = glm::cos(glm::radians(10.0f));
            light1.outerCutoff = glm::cos(glm::radians(15.0f));
        }*/

        //materialUnifrom.offset.x = time * 0.2f;
        //materialUnifrom.tiling = glm::vec2{ 2.0f };

        //lightUniform.intensity = std::sin(time) * 0.5f + 0.5f;

        //transform
        //transform.rotation.y = time * 90;

        // view matrix
        auto view = glm::lookAtRH(vertexUniform.cameraPosition, vertexUniform.cameraPosition + glm::vec3{ 0 , 0, -1}, glm::vec3{ 0,1,0 });

        // projection matrix
        float aspectRatio = Engine::Get().GetRenderer().GetWidth() / (float)Engine::Get().GetRenderer().GetHeight();
        auto projection = glm::perspective(glm::radians(70.0f), aspectRatio, 0.01f, 200.0f);

        //mvp
        vertexUniform.model = transform.GetMatrix();
        vertexUniform.mvp = projection * view * vertexUniform.model;


        //render
        
        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetRenderer().SetPipeline(*pipeline);

        Engine::Get().GetRenderer().setVertexUniform(0, &vertexUniform, sizeof(vertexUniform));
        Engine::Get().GetRenderer().setVertexUniform(1, &lightUniform, sizeof(lightUniform));
        Engine::Get().GetRenderer().setVertexUniform(2, &materialUniform, sizeof(materialUniform));

        Engine::Get().GetRenderer().setFragmentUniform(0, &vertexUniform, sizeof(vertexUniform));
        Engine::Get().GetRenderer().setFragmentUniform(1, &lightUniform, sizeof(lightUniform));
        Engine::Get().GetRenderer().setFragmentUniform(2, &materialUniform, sizeof(materialUniform));

        Engine::Get().GetRenderer().SetFragmentTexture(0, *texture);


        model->Draw(Engine::Get().GetRenderer());

        Engine::Get().GetRenderer().EndFrame();
    }


    //shutdown 
    Engine::Get().Shutdown();

    return 0;
}

