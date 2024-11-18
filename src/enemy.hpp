#ifndef ENEMY_HPP_INCLUDED
#define ENEMY_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>
#include "texts.hpp"

class Enemy
{
    private:
        DamageText* damagePopUps[20];
        int _hp;
        int popUpIndex = 0;
        int currentIdleAnimation = 0;
        float lastDmg;
        float dmgTextF;
        float textSpeed;
        float textOpacity;
        float dmgCd;
        float frameDuration = 0.1f;
        float elapsedIdleAnimation = 0.0f;
        bool alive;
        bool hitted;
    public:

        Enemy();

        void damageText(float);
        void loadTextures();
        void enemyAnimation(float);
        void takeDamage(int, float);
        void updateDamageText(float);
        void popUps(sf::RenderWindow&, float);
        int getHp();
        sf::Font font;
        sf::Text dmgText;
        sf::Texture texture;
        sf::Sprite enemySprite;
        sf::IntRect enemyIdle[11];
};

#endif // ENEMY_HPP_INCLUDED
