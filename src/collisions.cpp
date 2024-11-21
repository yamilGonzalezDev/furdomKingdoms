#include "collisions.hpp"
#include "player.hpp"
#include "entity.hpp"
#include <iostream>

void Collision::notify(ObserverEvents event)
{
    if(event == ObserverEvents::Default) std::cout << "Avise a las colisiones" << std::endl;
}

void Collision::BeginContact(b2Contact* contact)
{
    b2Fixture* fA = contact->GetFixtureA();
    b2Fixture* fB = contact->GetFixtureB();

    b2Body* bA = fA->GetBody();
    b2Body* bB = fB->GetBody();

    b2Body* pBody;

    if(bA != nullptr)
    {
        b2World* world = bA->GetWorld();
        for(b2Body* body = world->GetBodyList(); body != nullptr; body = body->GetNext())
        {
            UserdataTag* it = reinterpret_cast<UserdataTag*>(body->GetUserData().pointer);
            if(it->kind == Kind::PLAYER)
            {
                pBody = body;
            }
        }
    }

    UserdataTag* tagA = reinterpret_cast<UserdataTag*>(bA->GetUserData().pointer);
    UserdataTag* tagB = reinterpret_cast<UserdataTag*>(bB->GetUserData().pointer);

    if(tagA->kind == Kind::PLAYER && tagB->kind == Kind::FLOOR)
    {
        Player* player = reinterpret_cast<Player*>(tagA->object);
        player->setIsJumping(false);
        player->setIsOnGround(true);
    }
    if(tagB->kind == Kind::PLAYER && tagA->kind == Kind::FLOOR)
    {
        Player* player = reinterpret_cast<Player*>(tagB->object);
        player->setIsJumping(false);
        player->setIsOnGround(true);
    }
    if(tagA->kind == Kind::PLAYER && tagB->kind == Kind::WALLS)
    {
        b2Vec2 currentVelocity = pBody->GetLinearVelocity();
        pBody->SetLinearVelocity(b2Vec2(-currentVelocity.x * 0.5f, 4.f));
    }
    if(tagB->kind == Kind::PLAYER && tagA->kind == Kind::WALLS)
    {
        b2Vec2 currentVelocity = pBody->GetLinearVelocity();
        pBody->SetLinearVelocity(b2Vec2(-currentVelocity.x * 0.5f, 4.f));
    }

    if(tagA->kind == Kind::PLAYER && tagB->kind == Kind::ENEMY)
    {
        std::cout << "Enemigo detectado" << std::endl;
    }
    if(tagB->kind == Kind::PLAYER && tagA->kind == Kind::ENEMY)
    {
        std::cout << "Enemigo detectado" << std::endl;
    }
}

void Collision::EndContact(b2Contact* contact)
{
    b2Fixture* fA = contact->GetFixtureA();
    b2Fixture* fB = contact->GetFixtureB();

    b2Body* bA = fA->GetBody();
    b2Body* bB = fB->GetBody();

    UserdataTag* tagA = reinterpret_cast<UserdataTag*>(bA->GetUserData().pointer);
    UserdataTag* tagB = reinterpret_cast<UserdataTag*>(bB->GetUserData().pointer);

    if(tagA->kind == Kind::PLAYER && tagB->kind == Kind::FLOOR)
    {
        Player* player = reinterpret_cast<Player*>(tagA->object);
        player->setIsJumping(true);
        player->setIsOnGround(false);
    }
    if(tagB->kind == Kind::PLAYER && tagA->kind == Kind::FLOOR)
    {
        Player* player = reinterpret_cast<Player*>(tagB->object);
        player->setIsJumping(true);
        player->setIsOnGround(false);
    }
}
