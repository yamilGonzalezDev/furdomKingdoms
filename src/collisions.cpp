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
        notify(ObserverEvents::Cinematic);
    }
    if(tagB->kind == Kind::PLAYER && tagA->kind == Kind::WALLS)
    {
        notify(ObserverEvents::Cinematic);
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
