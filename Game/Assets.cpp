#include "Assets.h"

using namespace nu;

namespace assets {

    Mesh body{ { Vector2{2, 0},
                 Vector2{0, 1}, 
                 Vector2{ -2, 3},
                 Vector2{ -1, 0}, 
                 Vector2{ -3, -3}, Vector2{ 0, -1}, Vector2{2, 0}}, Color{ 1.0f, 0.0f, 1.0f } };
    Mesh wings{ { Vector2{1, 3}, Vector2{0, 1}, Vector2{ 0, -1}, Vector2{1, -3}, Vector2{1, 3}}, Color{1.0f, 0.5f, 1.0f} };

    std::shared_ptr<Model> playerModel = std::make_shared<Model> ( std::vector<Mesh>{ body, wings } );
    //model.AddMesh(wings);


    Mesh enemyBody{ { Vector2{2, 0},
                 Vector2{0, 1},
                 Vector2{ -2, 3},
                 Vector2{ -1, 0},
                 Vector2{ -3, -3}, Vector2{ 0, -1}, Vector2{2, 0}}, Color{ 1.0f, 0.0f, 1.0f } };
    Mesh enemyWings{ { Vector2{1, 3}, Vector2{0, 1}, Vector2{ 0, -1}, Vector2{1, -3}, Vector2{1, 3}}, Color{1.0f, 1.0f, 1.0f} };

    std::shared_ptr<Model> enemyModel = std::make_shared<Model> ( std::vector<Mesh>{ enemyBody, enemyWings } );


    Mesh bulletMesh{
    {
        Vector2{ -1, -1 },
        Vector2{ 1, 0 },
        Vector2{ -1, 1 },
        Vector2{ -1, -1 },
         },
        Color{ 1.0f, 0.0f, 0.0f }
    };

    std::shared_ptr<Model> bulletModel = std::make_shared<Model>( std::vector<Mesh>{ bulletMesh } );
}
