//
// Created by samuelbrisio on 6/1/25.
//

#include "InvisibleCollider.h"

InvisibleCollider::InvisibleCollider(Game *game, std::vector<Vector2> &vertices) :
    Actor(game)
{
    mVertices = &vertices;

    // Cria o colider para a mesa
    mRigidBodyComponent = new RigidBodyComponent(this, 1.0, 5.0);
    mRigidBodyComponent->SetApplyGravity(false);

    // Desenho o bloco de colisão
    mDrawComponent = new DrawPolygonComponent(this, vertices);

}

void InvisibleCollider::OnUpdate(float deltaTime) {
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