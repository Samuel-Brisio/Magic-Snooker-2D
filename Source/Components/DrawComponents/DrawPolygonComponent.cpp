//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "DrawPolygonComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"

DrawPolygonComponent::DrawPolygonComponent(class Actor* owner, std::vector<Vector2> &vertices, int drawOrder)
        :DrawComponent(owner)
        ,mVertices(vertices)
        ,mDrawOrder(drawOrder)
{
}

void DrawPolygonComponent::Draw(SDL_Renderer *renderer)
{
    // Set draw color to green
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    // Render vertices as lines
    for(int i = 0; i < mVertices.size() - 1; i++) {
        SDL_RenderDrawLine(renderer, mVertices[i].x,
                                     mVertices[i].y,
                                     mVertices[i+1].x,
                                     mVertices[i+1].y);
    }

    // Close geometry
    SDL_RenderDrawLine(renderer, mVertices[mVertices.size() - 1].x,
                                 mVertices[mVertices.size() - 1].y,
                                 mVertices[0].x,
                                 mVertices[0].y);
}