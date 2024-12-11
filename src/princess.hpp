#ifndef PRINCESS_HPP_INCLUDED
#define PRINCESS_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class Princess
{
    public:
        Princess(float, float, int);
        void update(float);
        void render(sf::RenderWindow&);
    private:
        sf::Texture texture;
        sf::Sprite sprite;
};

#endif // PRINCESS_HPP_INCLUDED
