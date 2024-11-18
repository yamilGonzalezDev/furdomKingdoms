#ifndef LIMITS_HPP_INCLUDED
#define LIMITS_HPP_INCLUDED

#include <box2d/box2d.h>

class Ground
{
    private:
        const float PPM;
    public:
        Ground() : PPM(30.f) {};
        void createGround(b2World*, float, float, float, float);
};

class Bounds
{
    private:
        const float PPM;
    public:
        Bounds() : PPM(30.f) {};
        void createWall(b2World*, float, float, float, float);
        void createFloor(b2World*, float, float, float, float);
};

#endif // LIMITS_HPP_INCLUDED
