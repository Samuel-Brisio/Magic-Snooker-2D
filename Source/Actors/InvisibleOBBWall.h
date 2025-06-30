//
// Created by gabri on 6/12/2025.
//

#ifndef INVISIBLEWALL_H
#define INVISIBLEWALL_H
#include "Actor.h"
#include "../Game.h"

class InvisibleOBBWall : public Actor  {
public:
    InvisibleOBBWall(Game* game, Vector2 centerPosition, float width, float height, float rotation);
    ~InvisibleOBBWall();
    OBBColliderComponent* GetColliderComponent() {return mOBBColliderComponent;}
private:
    // class DrawPolygonComponent* mDrawComponent;
    class OBBColliderComponent* mOBBColliderComponent;
};



#endif //INVISIBLEWALL_H
