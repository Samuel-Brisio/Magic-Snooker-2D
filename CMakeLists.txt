cmake_minimum_required(VERSION 3.26)
# Configurando o padrão c++
set(CMAKE_CXX_STANDARD 17)

# Configurando o nome e executável do projeto
set(PROJECT_NAME "tp3-smb")
project(${PROJECT_NAME})

find_package(SDL2 2.32.2 EXACT REQUIRED)
find_package(SDL2_image REQUIRED)

# Caminhos manuais para SDL2_image instalado em /opt/SDL2
#set(SDL2_IMAGE_INCLUDE_DIR "/opt/SDL2/include/SDL2")
#set(SDL2_IMAGE_LIBRARY "/opt/SDL2/lib/libSDL2_image.so")



add_executable(${PROJECT_NAME}
        Source/Main.cpp
        Source/Math.cpp
        Source/Random.cpp
        Source/Actors/Actor.cpp
        Source/Components/Component.cpp
        Source/Game.cpp
        Source/Components/DrawComponents/DrawComponent.cpp
        Source/Components/DrawComponents/DrawComponent.h
        Source/Components/RigidBodyComponent.cpp
        Source/Components/RigidBodyComponent.h
        Source/Components/ColliderComponents/CircleColliderComponent.cpp
        Source/Components/ColliderComponents/CircleColliderComponent.h
        Source/Json.h
        Source/CSV.h
        Source/Components/DrawComponents/DrawSpriteComponent.cpp
        Source/Components/DrawComponents/DrawSpriteComponent.h
        Source/Components/DrawComponents/DrawPolygonComponent.cpp
        Source/Components/DrawComponents/DrawPolygonComponent.h
        Source/Actors/Mario.cpp
        Source/Actors/Mario.h
        Source/Components/ColliderComponents/AABBColliderComponent.cpp
        Source/Components/ColliderComponents/AABBColliderComponent.h
        Source/Components/DrawComponents/DrawAnimatedComponent.cpp
        Source/Components/DrawComponents/DrawAnimatedComponent.h
        Source/Actors/Block.cpp
        Source/Actors/Block.h
        Source/Actors/Spawner.cpp
        Source/Actors/Spawner.h
        Source/Actors/Ball.cpp
        Source/Actors/Ball.h
        Source/Actors/WhiteBall.cpp
        Source/Actors/WhiteBall.h
        Source/Actors/ColorBall.cpp
        Source/Actors/ColorBall.h
        Source/Actors/Table.cpp
        Source/Actors/Table.h
        Source/Actors/Background.cpp
        Source/Actors/Background.h
        Source/Actors/InvisibleCollider.cpp
        Source/Actors/InvisibleCollider.h
)

# Diretórios de inclusão e bibliotecas a linkar
#target_include_directories(${PROJECT_NAME} PRIVATE
#        ${SDL2_INCLUDE_DIRS}
#        ${SDL2_IMAGE_INCLUDE_DIR}
#)

#target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 ${SDL2_IMAGE_LIBRARY})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)

