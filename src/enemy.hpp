#ifndef ENEMY_HPP_INCLUDED
#define ENEMY_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>
#include "texts.hpp"

class Enemy
{
    private:
        const float PPM = 30.f;
    public:
        ~Enemy() = default;

        virtual void update();
        virtual void render();

};

#endif // ENEMY_HPP_INCLUDED
