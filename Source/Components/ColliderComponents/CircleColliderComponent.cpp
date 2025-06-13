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

const Vector2& CircleColliderComponent::GetCenter() const
{
    return mOwner->GetPosition();
}

float CircleColliderComponent::GetRadius() const
{
    return mOwner->GetScale() * mRadius;
}

bool CircleColliderComponent::Intersect(const CircleColliderComponent* c) const
{
    // Calculate amount squared
    Vector2 diff = GetCenter() - c->GetCenter();
    float distSq = diff.LengthSq();

    // Calculate sum of radii squared
    float radiiSq = GetRadius() + c->GetRadius();
    radiiSq *= radiiSq;

    return distSq <= radiiSq;
}

bool CircleColliderComponent::Intersect(const AABBColliderComponent* b) const {
    Vector2 min = b->GetMin();
    Vector2 max = b->GetMax();

    Vector2 center = GetCenter();
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

void CircleColliderComponent::SolveCollision(const AABBColliderComponent* collider) {
    RigidBodyComponent* ownerRigidBody = mOwner->GetComponent<RigidBodyComponent>();

    Vector2 velocity = ownerRigidBody->GetVelocity();
    Vector2 center = this->GetCenter();
    float radius = this->GetRadius();

    Vector2 min = collider->GetMin();
    Vector2 max = collider->GetMax();

    // 1. Find the closest point on the AABB to the ball's center
    float closestX = std::max(min.x, std::min(center.x, max.x));
    float closestY = std::max(min.y, std::min(center.y, max.y));

    // 2. Calculate collision normal (direction from closest point to center)
    Vector2 collisionNormal = Vector2::Normalize(Vector2(center.x - closestX, center.y - closestY));


    // 3. Bounce the ball based on collision normal (reflect velocity)
    float dotProduct = velocity.x * collisionNormal.x + velocity.y * collisionNormal.y;
    Vector2 reflectedVelocity = velocity - 2.0f * dotProduct * collisionNormal;

    // 4. Apply restitution (optional: adds bounciness)
    float restitution = 0.8f; // Adjust for desired bounce effect
    reflectedVelocity *= restitution;

    // 5. Update velocity
    ownerRigidBody->SetVelocity(reflectedVelocity);

    // 6. Optional: Move the ball outside the collider to prevent sticking
    Vector2 penetration = collisionNormal * (radius - (center - Vector2(closestX, closestY)).Length());
    Vector2 newPosition = center + penetration;
    mOwner->SetPosition(newPosition);
}

void CircleColliderComponent::SolveCollision(const CircleColliderComponent* other) {
    SDL_Log("Ball-Ball Collision");

    // Get positions and radii
    Vector2 posA = GetCenter();
    Vector2 posB = other->GetCenter();
    float radiusA = GetRadius();
    float radiusB = other->GetRadius();

    // Get velocities
    Vector2 velA = mOwner->GetComponent<RigidBodyComponent>()->GetVelocity();
    Vector2 velB = other->mOwner->GetComponent<RigidBodyComponent>()->GetVelocity();

    // 1. Calculate collision normal (direction from B to A)
    Vector2 collisionNormal = Vector2::Normalize((posA - posB));

    // 2. Calculate relative velocity
    Vector2 relativeVelocity = velA - velB;

    // 3. Calculate velocity along collision normal
    float velocityAlongNormal = Vector2::Dot(relativeVelocity, collisionNormal);

    // 4. Don't resolve if separating
    if (velocityAlongNormal > 0) return;

    // 5. Simplified physics with equal properties
    const float restitution = 0.8f; // Equal bounciness for both balls
    const float massFactor = 0.5f;  // Equal mass distribution

    // 6. Calculate impulse
    float impulse = -(1 + restitution) * velocityAlongNormal * massFactor;

    // 7. Apply impulses (equal and opposite)
    Vector2 impulseVec = impulse * collisionNormal;

    mOwner->GetComponent<RigidBodyComponent>()->ApplyForce(impulseVec);
    other->mOwner->GetComponent<RigidBodyComponent>()->ApplyForce(-1 * impulseVec);

    // 8. Simple position correction to prevent sticking
    Vector2 correction = Vector2::Normalize(posA - posB) *
                        ((radiusA + radiusB) - (posA - posB).Length()) * 0.5f;

    mOwner->SetPosition(posA + correction);
    other->mOwner->SetPosition(posB - correction);
}

bool CircleColliderComponent::Intersect(const OBBColliderComponent* obb) const
{
    // Transform circle center to OBB's local space
    Vector2 center = GetCenter();
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

void CircleColliderComponent::SolveCollision(const OBBColliderComponent* obb)
{
    RigidBodyComponent* rigidBody = mOwner->GetComponent<RigidBodyComponent>();
    if (!rigidBody) return;

    Vector2 center = GetCenter();
    Vector2 obbCenter = obb->GetCenter();
    Vector2 axisX = obb->GetAxisX();
    Vector2 axisY = obb->GetAxisY();
    Vector2 extents = obb->GetExtents();

    // 1. Find closest point on OBB to circle (same as in Intersect)
    Vector2 dir = center - obbCenter;
    float xProj = Vector2::Dot(dir, axisX);
    float yProj = Vector2::Dot(dir, axisY);
    xProj = std::max(-extents.x, std::min(xProj, extents.x));
    yProj = std::max(-extents.y, std::min(yProj, extents.y));
    Vector2 closestPoint = obbCenter + axisX * xProj + axisY * yProj;

    // 2. Calculate collision normal
    Vector2 collisionNormal = Vector2::Normalize(center - closestPoint);

    // 3. Skip if separating (shouldn't happen since we detected collision)
    Vector2 velocity = rigidBody->GetVelocity();
    float velocityAlongNormal = Vector2::Dot(velocity, collisionNormal);
    if (velocityAlongNormal > 0) return;

    // 4. Calculate and apply impulse (similar to AABB version)
    float restitution = 0.8f;
    float dotProduct = velocity.x * collisionNormal.x + velocity.y * collisionNormal.y;
    Vector2 reflectedVelocity = velocity - (1.0f + restitution) * dotProduct * collisionNormal;
    rigidBody->SetVelocity(reflectedVelocity);

    // 5. Position correction to prevent sticking
    float radius = GetRadius();
    float penetrationDepth = radius - (center - closestPoint).Length();
    if (penetrationDepth > 0) {
        Vector2 correction = collisionNormal * penetrationDepth * 0.5f;
        mOwner->SetPosition(mOwner->GetPosition() + correction);
    }
}