#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED

#define CATEGORY_PLAYER 0x0001
#define CATEGORY_NPC    0x0002
#define CATEGORY_SENSOR 0x0004
#define CATEGORY_FLOOR  0X0006
#define CATEGORY_LIMITS 0x0008
#define CATEGORY_GROUND 0x0010

#include<SFML/Graphics.hpp>
#include<box2d/box2d.h>

typedef struct
{
    int kind;
    void* object;
} UserdataTag;

const int PLAYER = 1;
const int FLOOR = 2;
const int NPC = 3;
const int SENSOR = 4;
const int LIMITS = 5;
const int WALLS = 6;

void initBody(b2Body*, int, void*);

class Entity
{
    public:
        Entity();
        virtual ~Entity() = default;

        virtual void updateAnimation(float deltaTime);
        void switchAnimation(sf::IntRect* animationFrames, int numFrames);
        virtual void moveEntity(float deltaX, float deltaY);
        virtual void updatePhysics();
        void setHitbox(sf::RectangleShape* newHitbox);
        virtual void switchState(int newState);
};



#endif // ENTITY_HPP_INCLUDED
