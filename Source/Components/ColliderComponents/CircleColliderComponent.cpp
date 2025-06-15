//
// Created by Lucas N. Ferreira on 10/09/23.
//

#include "CircleColliderComponent.h"

#include "../../Game.h"
#include "../../Actors/Actor.h"

CircleColliderComponent::CircleColliderComponent(class Actor* owner, const float radius, const int updateOrder)
        :Component(owner, updateOrder)
        ,mRadius(radius) {
    mOwner->GetGame()->AddCircleCollider(this);
}

CircleColliderComponent::~CircleColliderComponent() {
    mOwner->GetGame()->RemoveCircleCollider(this);
}

float CircleColliderComponent::GetRadius() const
{
    return mOwner->GetScale() * mRadius;
}

bool CircleColliderComponent::Intersect(const CircleColliderComponent* c) const
{
    // Calculate amount squared
    Vector2 diff = mOwner->GetPosition() - c->mOwner->GetPosition();
    float distSq = diff.LengthSq();

    // Calculate sum of radii squared
    float radiiSq = GetRadius() + c->GetRadius();
    radiiSq *= radiiSq;

    return distSq <= radiiSq;
}

bool CircleColliderComponent::Intersect(const AABBColliderComponent* b) const {
    Vector2 min = b->GetMin();
    Vector2 max = b->GetMax();

    Vector2 center = mOwner->GetPosition();
    float radius = mRadius;

    // Find the closest point on the AABB to the circle
    float closestX = std::max(min.x, std::min(center.x, max.x));
    float closestY = std::max(min.y, std::min(center.y, max.y));

    // Calculate the distance between the circle's center and this closest point
    float distanceX = center.x - closestX;
    float distanceY = center.y - closestY;

    // If the distance is less than the circle's radius, an intersection occurs
    float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
    return distanceSquared < (radius * radius);
}

bool CircleColliderComponent::Intersect(const OBBColliderComponent* obb) const
{
    // Transform circle center to OBB's local space
    Vector2 center = mOwner->GetPosition();
    Vector2 obbCenter = obb->GetCenter();
    Vector2 axisX = obb->GetAxisX();
    Vector2 axisY = obb->GetAxisY();

    // Vector from OBB center to circle center
    Vector2 dir = center - obbCenter;

    // Project circle center onto OBB axes
    float xProj = Vector2::Dot(dir, axisX);
    float yProj = Vector2::Dot(dir, axisY);

    // Clamp projections to OBB extents
    Vector2 extents = obb->GetExtents();
    xProj = std::max(-extents.x, std::min(xProj, extents.x));
    yProj = std::max(-extents.y, std::min(yProj, extents.y));

    // Find closest point on OBB to circle
    Vector2 closestPoint = obbCenter + axisX * xProj + axisY * yProj;

    // Check distance to circle
    Vector2 diff = center - closestPoint;
    float distSq = diff.LengthSq();
    float radius = GetRadius();

    return distSq <= (radius * radius);
}
