#ifndef COLISIONS_HPP_INCLUDED
#define COLISIONS_HPP_INCLUDED

#include <box2d/box2d.h>
#include "observer.hpp"

class Colision : public b2ContactListener
{
    public:
        void BeginContact(b2Contact* contact) override;
        void EndContact(b2Contact* contact) override;
        void PreSolve(b2Contact*, const b2Manifold*) override;
        void PostSolve(b2Contact*, const b2ContactImpulse*) override;
};

#endif // COLISIONS_HPP_INCLUDED
