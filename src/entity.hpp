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

enum class Kind
{
    PLAYER,
    ENEMY,
    NPC,
    FLOOR,
    LIMITS,
    WALLS,
    SENSOR
};

typedef struct
{
    Kind kind;
    void* object;
} UserdataTag;

void initBody(b2Body*, Kind, void*);

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
