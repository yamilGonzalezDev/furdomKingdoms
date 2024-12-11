#include "witch.hpp"
#include <iostream>

Witch::Witch(float x, float y)
{
    if(!texture.loadFromFile("Textures/enemy/witch/witchSheet.png"))
    {
        std::cerr << "Error al cargar las texturas de la bruja" << std::endl;
    }

    sprite.setTexture(texture);

    animations[0] = {0, 0, 64, 64};
    animations[1] = {64, 0, 64, 64};
    animations[2] = {128, 0, 64, 64};
    animations[3] = {192, 0, 64, 64};
    animations[4] = {256, 0, 64, 64};
    animations[5] = {320, 0, 64, 64};
    animations[6] = {384, 0, 64, 64};
    animations[7] = {448, 0, 64, 64};
    animations[8] = {512, 0, 64, 64};
    animations[9] = {576, 0, 64, 64};

    sprite.setTextureRect(animations[0]);
    sprite.setScale(1.f, 1.f);
    sprite.setPosition(x, y);
}

void Witch::update(float deltaTime)
{
    elapsedTime += deltaTime * 0.5f;

    if(elapsedTime >= cooldownAnimation)
    {
        currentFrame = (currentFrame + 1) % 10;
        sprite.setTextureRect(animations[currentFrame]);
        elapsedTime = 0.f;
    }
}

void Witch::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}
