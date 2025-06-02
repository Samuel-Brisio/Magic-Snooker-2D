//
// Created by samuelbrisio on 6/1/25.
//

#pragma once
#include "Actor.h"
#include "../Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"


class InvisibleCollider : public Actor {
public:
    explicit InvisibleCollider(Game* game, std::vector<Vector2> &vertices);
    void OnUpdate(float deltaTime) override;
private:
    std::vector<Vector2> *mVertices;
    class RigidBodyComponent* mRigidBodyComponent;
    class DrawPolygonComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;
};

