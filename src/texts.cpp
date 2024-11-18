#include "texts.hpp"

DamageText::DamageText(std::string& text, sf::Vector2f pos)
    : lifeTime(1.5f), textSpeed(-20.f), textOpacity(200)
{
    if(!font.loadFromFile("Textures/font/textFont.ttf")) return;

    dmgText.setFont(font);
    dmgText.setString(text);
    dmgText.setCharacterSize(16);
    dmgText.setFillColor(sf::Color::White);
    dmgText.setPosition(pos);
}

void DamageText::show(float deltaTime)
{
    if(lifeTime > 0)
    {
        lifeTime =  lifeTime - deltaTime;
        dmgText.move(0, textSpeed * deltaTime);

        textOpacity = textOpacity - (200 * deltaTime / 1.5f);
        if(textOpacity < 0) textOpacity = 0;
        dmgText.setFillColor(sf::Color::White);

        if(lifeTime <= 0)
        {
            expired = true;
        }
    }
}

bool DamageText::isExpired()
{
    if(expired)
    {
        return true;
    }

    return false;
}

void DamageText::draw(sf::RenderWindow& window)
{
    if(lifeTime > 0)
    {
        window.draw(dmgText);
    }
}
