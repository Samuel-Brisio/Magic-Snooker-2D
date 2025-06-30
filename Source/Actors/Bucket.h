//
// Created by gabri on 6/14/2025.
//

#ifndef BUCKET_H
#define BUCKET_H
#include "Actor.h"
#include "../Math.h"
#include "../Components/ColliderComponents/CircleColliderComponent.h"


class Bucket : public Actor {
public:
    Bucket(Game* game, Vector2 position, int radius);
    ~Bucket();

    CircleColliderComponent* GetColliderComponent() {return mColliderComponent;}

private:
    // class DrawCircleComponent* mDrawCircleComponent;
    class CircleColliderComponent* mColliderComponent;
};



#endif //BUCKET_H
