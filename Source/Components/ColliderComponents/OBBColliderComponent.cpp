//
// Created by gabri on 6/12/2025.
//

#include "OBBColliderComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"
#include <vector>

OBBColliderComponent::OBBColliderComponent(class Actor *owner, float width, float height):
    Component(owner)
    ,mWidth(width)
    ,mHeight(height)
{
    mOwner->GetGame()->AddOBBCollider(this);
}

OBBColliderComponent::~OBBColliderComponent()
{
    mOwner->GetGame()->RemoveOBBCollider(this);
}

std::vector<Vector2> OBBColliderComponent::GetCorners() const
{
    Vector2 center = GetCenter();
    Vector2 extents = GetExtents();
    Vector2 axisX = GetAxisX();
    Vector2 axisY = GetAxisY();

    std::vector<Vector2> corners;
    corners.push_back( center + axisX * extents.x + axisY * extents.y);  // Top-right
    corners.push_back(center - axisX * extents.x + axisY * extents.y);  // Top-left
    corners.push_back(center - axisX * extents.x - axisY * extents.y);  // Bottom-left
    corners.push_back(center + axisX * extents.x - axisY * extents.y);  // Bottom-right

    return corners;
}

Vector2 OBBColliderComponent::GetCenter() const
{
    return mOwner->GetPosition();
}

Vector2 OBBColliderComponent::GetExtents() const
{
    return Vector2(mWidth * 0.5f * mOwner->GetScale(),
                 mHeight * 0.5f * mOwner->GetScale());
}

// Helper function to get the axes of the OBB
Vector2 OBBColliderComponent::GetAxisX() const
{
    float rotation = mOwner->GetRotation();
    return Vector2(cos(rotation), sin(rotation));
}

Vector2 OBBColliderComponent::GetAxisY() const
{
    float rotation = mOwner->GetRotation();

    return Vector2(-sin(rotation), cos(rotation));
}
