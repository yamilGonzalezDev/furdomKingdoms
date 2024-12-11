#ifndef WITCH_HPP_INCLUDED
#define WITCH_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class Witch
{
    public:
        Witch(float, float);
        void update(float);
        void render(sf::RenderWindow&);
    private:
        sf::Texture texture;
        sf::Sprite sprite;
        sf::IntRect animations[9];
        int currentFrame = 0;
        float elapsedTime = 0.f;
        const float cooldownAnimation = 0.2f;
};

#endif // WITCH_HPP_INCLUDED
