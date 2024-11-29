#ifndef LIMITS_HPP_INCLUDED
#define LIMITS_HPP_INCLUDED

#include <box2d/box2d.h>
#include <iostream>
#include "entity.hpp"
#include "observer.hpp"

class Bounds : public Subject
{
    protected:
        const float PPM = 30.f;
        b2Body* body;
    public:
        virtual ~Bounds() {};
        bool sensorNotify(bool v) { return v; };
};

class Limits : public Bounds
{
    public:
        Limits(b2World*, float, float, float, float, Kind);
};

class Wall : public Bounds
{
    public:
        Wall(b2World*, float, float, float, float, Kind);
};

class Sensor : public Bounds
{
    public:
        Sensor(b2World*, float, float, float, float, Kind);
        void debugSensor()
        {
            std::cout << "Funciono" << std::endl;
        }
};

class BoundsFactory
{
    public:
        virtual Bounds* createBound(b2World*, float, float, float, float, Kind) const = 0;
        virtual Bounds* createWall(b2World*, float, float, float, float, Kind) const { return nullptr; };
};

class LimitsFactory : public BoundsFactory
{
    public:
        Bounds* createBound(b2World*, float, float, float, float, Kind) const override;
        Bounds* createWall(b2World*, float, float, float, float, Kind) const override;
};

class SensorFactory : public BoundsFactory
{
    public:
        Bounds* createBound(b2World*, float, float, float, float, Kind) const override;
};

#endif // LIMITS_HPP_INCLUDED
