//
// Created by samuelbrisio on 6/1/25.
//

#include "InvisibleAABBWall.h"

InvisibleAABBWall::InvisibleAABBWall(Game *game, SDL_Rect position) :
    Actor(game)
{
    game->AddInvisibleAABBWall(this);

    SetPosition(Vector2(position.x + position.w/2, position.y + position.h/2));

    // Cria o colider para a mesa
    mRigidBodyComponent = new RigidBodyComponent(this, 1.0, 5.0);
    mRigidBodyComponent->SetApplyGravity(false);
    mColliderComponent = new AABBColliderComponent(this, -position.w/2, -position.h/2, position.w, position.h,
        ColliderLayer::Blocks);

    // Desenho o bloco de colisão
    // std::vector<Vector2> vertices;
    // vertices.emplace_back(Vector2(position.x, position.y));
    // vertices.emplace_back(Vector2(position.x, position.y + position.h));
    // vertices.emplace_back(Vector2(position.x + position.w, position.y + position.h));
    // vertices.emplace_back(Vector2( position.x + position.w,  position.y));
    // mDrawComponent = new DrawPolygonComponent(this, vertices);

}

InvisibleAABBWall::~InvisibleAABBWall() {
    mGame->RemoveInvisibleAABBWall(this);
}


void InvisibleAABBWall::OnUpdate(float deltaTime) {
    // --------------
    // TODO - PARTE 2
    // --------------

    // TODO 4 (~2 linhas): Para evitar que o jogador ultrapasse o limite inferior (esquerdo) da câmera,
    //  limite sua posição horizontal para ser sempre maior ou igual a posição horizontal da câmera.
    // auto acc = mRigidBodyComponent->GetAcceleration();
    // auto vel = mRigidBodyComponent->GetVelocity();
    // if (mPosition.x < GetGame()->GetCameraPos().x) {
    //
    //     mPosition.x = GetGame()->GetCameraPos().x ;
    // }

    // --------------
    // TODO - PARTE 5
    // --------------

    // TODO 7 (~2 linhas): Verifique se a posição vertical do jogador é maior do que o tamanho da tela.
    //  Se for, chame o método `Kill`.


    // ManageAnimations();
}