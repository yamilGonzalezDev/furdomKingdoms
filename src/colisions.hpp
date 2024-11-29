#ifndef COLISIONS_HPP_INCLUDED
#define COLISIONS_HPP_INCLUDED

#include <box2d/box2d.h>
#include "observer.hpp"

class Colision : public b2ContactListener, public Observer
{
    public:
        void notify(ObserverEvents) override;
        void BeginContact(b2Contact* contact) override;
        void EndContact(b2Contact* contact) override;
};

#endif // COLISIONS_HPP_INCLUDED
