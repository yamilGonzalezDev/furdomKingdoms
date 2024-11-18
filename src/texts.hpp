#ifndef TEXTS_HPP_INCLUDED
#define TEXTS_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class DamageText
{
    public:
        DamageText(std::string&, sf::Vector2f);
        void show(float);
        void draw(sf::RenderWindow&);
        bool isExpired();
    private:
        sf::Text dmgText;
        sf::Font font;
        float lifeTime;
        float textSpeed;
        float textOpacity;
        bool expired = false;
};

#endif // TEXTS_HPP_INCLUDED
