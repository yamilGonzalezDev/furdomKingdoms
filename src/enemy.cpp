#include "enemy.hpp"
#include "texts.hpp"
#include <iostream>
///24x32

Enemy::Enemy()
    : _hp(100), lastDmg(0.0f), dmgTextF(0.f), textSpeed(-20.f), textOpacity(200), dmgCd(0.1f), alive(true), hitted(false)
{
    loadTextures();
    for(int i = 0; i < 20; i++)
    {
        damagePopUps[i] = nullptr;
    }

    enemySprite.setTexture(texture);
    enemySprite.setTextureRect(enemyIdle[0]);
    enemySprite.setScale(-1.5f, 1.5f);
    enemySprite.setPosition(450, 415);
}

void Enemy::loadTextures()
{
    if(!font.loadFromFile("Textures/font/textFont.ttf"))
    {
        return;
    }

    if(!texture.loadFromFile("Textures/enemy/Sprite Sheets/Skeleton Idle.png"))
    {
       return;
    }

    enemyIdle[0] = sf::IntRect(0, 0, 24, 32);
    enemyIdle[1] = sf::IntRect(24, 0, 24, 32);
    enemyIdle[2] = sf::IntRect(48, 0, 24, 32);
    enemyIdle[3] = sf::IntRect(72, 0, 24, 32);
    enemyIdle[4] = sf::IntRect(96, 0, 24, 32);
    enemyIdle[5] = sf::IntRect(120, 0, 24, 32);
    enemyIdle[6] = sf::IntRect(144, 0, 24, 32);
    enemyIdle[7] = sf::IntRect(168, 0, 24, 32);
    enemyIdle[8] = sf::IntRect(192, 0, 24, 32);
    enemyIdle[9] = sf::IntRect(216, 0, 24, 32);
    enemyIdle[10] = sf::IntRect(240, 0, 24, 32);
}

void Enemy::enemyAnimation(float deltaTime)
{
    elapsedIdleAnimation += deltaTime;
    if(elapsedIdleAnimation > frameDuration)
    {
        currentIdleAnimation = (currentIdleAnimation + 1) % 11;
        enemySprite.setTextureRect(enemyIdle[currentIdleAnimation]);
        elapsedIdleAnimation = 0.0f;
    }
}

void Enemy::takeDamage(int dmg, float deltaTime)
{
    if(alive)
    {
        std::cout << "Enemigo recibe daño: " << dmg << std::endl;
        _hp = _hp - dmg;

        if(popUpIndex < 20 && _hp > 0)
        {
            std::string dmgString = std::to_string(dmg);
            if(damagePopUps[popUpIndex] != nullptr)
            {
                delete damagePopUps[popUpIndex];
                damagePopUps[popUpIndex] = nullptr;
            }
            damagePopUps[popUpIndex] = new DamageText(dmgString, enemySprite.getPosition());
            popUpIndex++;
        }
        else
        {
            alive = false;
            _hp = 0;
            enemySprite.setColor(sf::Color::Transparent);
        }

        if(popUpIndex == 19)
        {
            popUpIndex = 0;
        }
    }
}
//
void Enemy::popUps(sf::RenderWindow& window, float deltaTime)
{
    if(!alive) return;

    for (int i = 19; i >= 0; i--)
    {
        if(damagePopUps[i] != nullptr)
        {
            damagePopUps[i]->show(deltaTime);

            if(damagePopUps[i]->isExpired())
            {
                delete damagePopUps[i];
                damagePopUps[i] = nullptr;

                if(i == popUpIndex - 1)
                {
                    popUpIndex--;
                }
            }
            else
            {
                damagePopUps[i]->draw(window);
            }
        }
    }
}

int Enemy::getHp(){ return _hp; }
