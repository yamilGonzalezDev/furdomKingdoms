#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED

#define CATEGORY_PLAYER 0x0001
#define CATEGORY_NPC    0x0002
#define CATEGORY_ENEMY  0x0004
#define CATEGORY_SENSOR 0x0008
#define CATEGORY_FLOOR  0X0010
#define CATEGORY_LIMITS 0x0020
#define CATEGORY_GROUND 0x0040
//#define CATEGORY_  0x0080
//#define CATEGORY_   0x0100
//#define CATEGORY_   0x0200
//#define CATEGORY_    0x0400
//#define CATEGORY_   0x0800

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
    HOUSESENSOR,
    BARDOOR
};

typedef struct
{
    Kind kind;
    void* object;
} UserdataTag;

void initBody(b2Body*, Kind, void*);

#endif // ENTITY_HPP_INCLUDED
