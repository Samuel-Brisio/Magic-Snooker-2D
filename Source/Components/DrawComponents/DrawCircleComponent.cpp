//
// Created by gabri on 6/9/2025.
//

#include "DrawCircleComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"

DrawCircleComponent::DrawCircleComponent(class Actor* owner, int numVertices, float radius, int drawOrder)
        :DrawComponent(owner)
        ,mDrawOrder(drawOrder)
        ,mNumVertices(numVertices)
        ,mRadius(radius)
{
}


void DrawCircleComponent::Draw(SDL_Renderer *renderer)
{

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        std::vector<Vector2> vertices;

        float angle = 0.0f;

        for (int i = 0; i < mNumVertices; i++) {
                float x = mRadius * cos(angle);
                float y = mRadius * sin(angle);
                Vector2 p = Vector2(x, y);
                vertices.push_back(p);
                angle += 2*Math::Pi / mNumVertices;
        }

        this->DrawPolygon(renderer, vertices);
}

void DrawCircleComponent::DrawPolygon(SDL_Renderer *renderer, std::vector<Vector2>& vertices)
{
        Vector2 pos = mOwner->GetPosition();

        for (int i = 0; i < vertices.size() - 1; i++) {
                Vector2 p1 = vertices[i] + pos;
                Vector2 p2 = vertices[i + 1] + pos;
                SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
        }

        SDL_RenderDrawLine(renderer, vertices[0].x + pos.x, vertices[0].y + pos.y, vertices[vertices.size()-1].x + pos.x, vertices[vertices.size()-1].y + pos.y);
}