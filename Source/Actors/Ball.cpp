//
// Created by samuelbrisio on 29/05/25.
//

#include "Ball.h"

#include "Bucket.h"
#include "InvisibleAABBWall.h"
#include "InvisibleOBBWall.h"
#include "../Game.h"
#include "../Components/ColliderComponents/CircleColliderComponent.h"
#include "../Components/DrawComponents/DrawCircleComponent.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"

Ball::Ball(Game *game, float radius, float weight, BallColor color)
    : Actor(game)
    ,mRadius(radius)
    ,mWeight(weight)
    ,mColor(color)
    ,mIsMoving(false)
{
    game->AddBall(this);

    mRigidBodyComponent = new RigidBodyComponent(this, weight, 0.3);
    mColliderComponent = new CircleColliderComponent(this, mRadius);
    // mDrawCircleComponent = new DrawCircleComponent(this, 20, mRadius, 100);

    Vector2 drawPositionOffset(-radius, -radius);

    if (color == BallColor::White) {
        mDrawSpriteComponent = new DrawSpriteComponent(this, "../Assets/Sprites/WhiteBall/WhiteBall.png",
        mRadius*2, mRadius*2, 200, drawPositionOffset);
    }
    else if (color == BallColor::Red) {
        mDrawSpriteComponent = new DrawSpriteComponent(this, "../Assets/Sprites/RedBall/RedBall.png",
        mRadius * 2, mRadius * 2, 200, drawPositionOffset);
    }
    else if (color == BallColor::Blue) {
        mDrawSpriteComponent = new DrawSpriteComponent(this, "../Assets/Sprites/BlueBall/BlueBall.png",
        mRadius * 2, mRadius * 2, 200, drawPositionOffset);
    }
}

Ball::~Ball() {
    mGame->RemoveBall(this);
}

void Ball::OnUpdate(float deltaTime) {
    auto aabbWalls = mGame->GetInvisibleAABBWalls();
    for (auto aabbWall : aabbWalls) {
        bool isCollision = mColliderComponent->Intersect(aabbWall->GetColliderComponent());
        if (isCollision) SolveCollision(aabbWall);
    }

    auto balls = mGame->GetBalls();
    for (auto ball : balls) {
        if (ball != this) {
            bool isCollision = mColliderComponent->Intersect(ball->mColliderComponent);
            if (isCollision) SolveCollision(ball);
        }
    }

    auto obbWalls = mGame->GetInvisibleOBBWalls();
    for (auto obbWall : obbWalls) {
        bool isCollision = mColliderComponent->Intersect(obbWall->GetColliderComponent());
        if (isCollision) SolveCollision(obbWall);
    }

    auto buckets = mGame->GetBuckets();
    for (auto bucket : buckets) {
        bool isCollision = mColliderComponent->Intersect(bucket->GetColliderComponent());
        if (isCollision) SolveCollision(bucket);
    }

    // Check if the ball is moving
    if (mRigidBodyComponent->GetVelocity().Length() > 0.001) mIsMoving = true;
    else mIsMoving = false;
}

 float Ball::GetRadius() const
{
    return GetScale() * mRadius;
}

void Ball::SolveCollision(class Ball * other) {
    // SDL_Log("Ball-Ball Collision");

    auto vel = mRigidBodyComponent->GetVelocity().Length();
    if (vel > 1) GetGame()->GetAudio()->PlaySound("BallBall.mp3", false);

    // Get positions and radii
    Vector2 posA = GetPosition();
    Vector2 posB = other->GetPosition();
    float radiusA = GetRadius();
    float radiusB = other->GetRadius();

    // Get velocities
    Vector2 velA = GetComponent<RigidBodyComponent>()->GetVelocity();
    Vector2 velB = other->GetComponent<RigidBodyComponent>()->GetVelocity();

    // 1. Calculate collision normal (direction from B to A)
    Vector2 collisionNormal = Vector2::Normalize((posA - posB));

    // 2. Calculate relative velocity
    Vector2 relativeVelocity = velA - velB;

    // 3. Calculate velocity along collision normal
    float velocityAlongNormal = Vector2::Dot(relativeVelocity, collisionNormal);

    // 4. Don't resolve if separating
    if (velocityAlongNormal > 0) return;

    // 5. Simplified physics with equal properties
    const float restitution = 0.95f; // Equal bounciness for both balls
    // const float massFactor = 0.5f;  // Equal mass distribution
    //
    // // 6. Calculate impulse
    // float impulse = -(1 + restitution) * velocityAlongNormal * massFactor;
    //
    // // 7. Apply impulses (equal and opposite)
    // Vector2 impulseVec = impulse * collisionNormal;
    //
    // GetComponent<RigidBodyComponent>()->ApplyForce(impulseVec);
    // other->GetComponent<RigidBodyComponent>()->ApplyForce(-1 * impulseVec);
    //
    // // 8. Simple position correction to prevent sticking
    // Vector2 correction = Vector2::Normalize(posA - posB) *
    //                     ((radiusA + radiusB) - (posA - posB).Length()) * 0.5f;
    //
    // SetPosition(posA + correction);
    // other->SetPosition(posB - correction);


    // Massas
    float massA = mWeight;
    float massB = other->mWeight;

    // Cálculo do impulso escalar
    float impulseScalar = -(1 + restitution) * velocityAlongNormal;
    impulseScalar /= (1 / massA) + (1 / massB);

    // Vetor impulso
    Vector2 impulse = impulseScalar * collisionNormal;

    // Atualiza velocidades diretamente (colisão elástica)
    GetComponent<RigidBodyComponent>()->SetVelocity(velA + impulse * (1 /massA));
    other->GetComponent<RigidBodyComponent>()->SetVelocity(velB - (impulse * (1 / massB)));

    // Correção de posição para evitar sobreposição
    float penetration = (radiusA + radiusB) - (posA - posB).Length();
    if (penetration > 0) {
        Vector2 correction = collisionNormal * (penetration / (massA + massB));
        SetPosition(posA + correction * (massB / (massA + massB)));
        other->SetPosition(posB - correction * (massA / (massA + massB)));
    }
}

void Ball::SolveCollision(class InvisibleAABBWall * aabbWall) {
    RigidBodyComponent* ownerRigidBody = GetComponent<RigidBodyComponent>();
    GetGame()->GetAudio()->PlaySound("BallWall.mp3", false);

    Vector2 velocity = ownerRigidBody->GetVelocity();
    Vector2 center = GetPosition();
    float radius = this->GetRadius();

    Vector2 min = aabbWall->GetColliderComponent()->GetMin();
    Vector2 max = aabbWall->GetColliderComponent()->GetMax();

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
    SetPosition(newPosition);
}

void Ball::SolveCollision(class InvisibleOBBWall * obbWall) {
    RigidBodyComponent* rigidBody = GetComponent<RigidBodyComponent>();
    if (!rigidBody) return;
    GetGame()->GetAudio()->PlaySound("BallWall.mp3", false);

    Vector2 center = GetPosition();
    Vector2 obbCenter = obbWall->GetColliderComponent()->GetCenter();
    Vector2 axisX = obbWall->GetColliderComponent()->GetAxisX();
    Vector2 axisY = obbWall->GetColliderComponent()->GetAxisY();
    Vector2 extents = obbWall->GetColliderComponent()->GetExtents();

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
        SetPosition(GetPosition() + correction);
    }
}

void Ball::SolveCollision(class Bucket* bucket) {
    float centerDist = (bucket->GetPosition() - GetPosition()).Length();
    bool isBallIntoBucket = centerDist < bucket->GetColliderComponent()->GetRadius();

    if (isBallIntoBucket) {
        SDL_Log("Bucket-Ball Collision");
        this->SetState(ActorState::Destroy);
        mGame->GetScore()->BallFellIntoPocket(mColor);
    }
}


