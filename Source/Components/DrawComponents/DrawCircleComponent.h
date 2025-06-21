//
// Created by gabri on 6/9/2025.
//

#ifndef DRAWCIRCLECOMPONENT_H
#define DRAWCIRCLECOMPONENT_H
#include "DrawComponent.h"


class DrawCircleComponent : public DrawComponent
{
public:
    // (Lower draw order corresponds with further back)
    DrawCircleComponent(class Actor* owner, int numVertices, float radius, int drawOrder = 100);

    void Draw(SDL_Renderer* renderer) override;

private:
    int mDrawOrder;
    int mNumVertices;
    float mRadius;
    void DrawPolygon(SDL_Renderer *renderer, std::vector<Vector2>& vertices);
};

#endif