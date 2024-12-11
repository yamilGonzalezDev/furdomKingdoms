#include "npc.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <thread>
#include "player.hpp"

void Barman::update(float deltaTime)
{
 elapsedTime += deltaTime;

    if(elapsedTime >= frameDuration)
    {
        currentFrame = (currentFrame + 1) % 5;
        npcSprite.setTextureRect(animation[currentFrame]);
        elapsedTime = 0.f;
    }
}

void Barman::render(sf::RenderWindow& window)
{
        window.draw(npcSprite);

}


Barman::Barman(float x, float y)
{

    if (!npcTexture.loadFromFile("Textures/Taverna/tabernero1.png")) {
        std::cerr << "No se pudo cargar la textura del NPC Barman" << std::endl;
        return;
    }

     animation[0] = {0, 0, 40, 48};
    animation[1] = {40, 0, 40, 48};
    animation[2] = {80, 0, 40, 48};
    animation[3] = {120, 0, 40, 48};
    animation[4] = {160, 0, 40, 48};

    npcSprite.setTexture(npcTexture);
    npcSprite.setOrigin(npcSprite.getLocalBounds().width / 2.f, npcSprite.getLocalBounds().height / 2.f);;
    npcSprite.setTextureRect(animation[0]);
    npcSprite.setPosition(x,y);

    currentFrame = 0;
    elapsedTime = 0.f;

}





void Marcianito::update(float deltaTime)
{
    elapsedTime += deltaTime;

    if(elapsedTime >= frameDuration)
    {
        currentFrame = (currentFrame + 1) % 3;
        npcSprite.setTextureRect(animation[currentFrame]);
        elapsedTime = 0.f;
    }
}


void Marcianito::render(sf::RenderWindow& window)
{
    window.draw(npcSprite);
}

Marcianito::Marcianito(float x, float y)
{


        if (!npcTexture.loadFromFile("Textures/Taverna/marci.png")) {
        std::cerr << "No se pudo cargar la textura del NPC Barman" << std::endl;
        return;
    }

    animation[0] = {0, 0, 173, 115};
    animation[1] = {173, 0, 173, 115};
    animation[2] = {346, 0, 173, 115};

    npcSprite.setOrigin(npcSprite.getLocalBounds().width / 2.f, npcSprite.getLocalBounds().height / 2.f);;
    npcSprite.setTexture(npcTexture);
    npcSprite.setTextureRect(animation[1]);
    npcSprite.setPosition(x,y);
    npcSprite.setScale(0.50f, 0.45f);

    currentFrame = 0;
    elapsedTime = 0.f;



}

void Dog::update(float deltaTime)
{
     elapsedTime += deltaTime;

    if(elapsedTime >= frameDuration)
    {
        currentFrame = (currentFrame + 1) % 4;
        npcSprite.setTextureRect(animation[currentFrame]);
        elapsedTime = 0.f;
    }
}

void Dog::render(sf::RenderWindow& window)
{
     window.draw(npcSprite);
}

Dog::Dog(float x,float y)
{

    if (!npcTexture.loadFromFile("Textures/Npcs/dog.png")) {
        std::cerr << "No se pudo cargar la textura del NPC Barman" << std::endl;
        return;
    }

    animation[0] = {0, 0, 32, 32};
    animation[1] = {32, 0, 32, 32};
    animation[2] = {64, 0, 32, 32};
    animation[3] = {96, 0, 32, 32};

    npcSprite.setTexture(npcTexture);
    npcSprite.setOrigin(npcSprite.getLocalBounds().width / 2.f, npcSprite.getLocalBounds().height / 2.f);;
    npcSprite.setTextureRect(animation[0]);
    npcSprite.setPosition(x,y);

    currentFrame = 0;
    elapsedTime = 0.f;
}

void Farmer::update(float deltaTime)
{
     elapsedTime += deltaTime;

    if(elapsedTime >= frameDuration)
    {
        currentFrame = (currentFrame + 1) % 5;
        npcSprite.setTextureRect(animation[currentFrame]);
        elapsedTime = 0.f;
    }
}

void Farmer::render(sf::RenderWindow& window)
{
     window.draw(npcSprite);
}

Farmer::Farmer(float x,float y)
{

    if (!npcTexture.loadFromFile("Textures/Npcs/farmer_01.png")) {
        std::cerr << "No se pudo cargar la textura del NPC Barman" << std::endl;
        return;
    }

    animation[0] = {0, 0, 32, 32};
    animation[1] = {32, 0, 32, 32};
    animation[2] = {64, 0, 32, 32};
    animation[3] = {96, 0, 32, 32};
    animation[4] = {128, 0, 32, 32};

    npcSprite.setTexture(npcTexture);
    npcSprite.setOrigin(npcSprite.getLocalBounds().width / 2.f, npcSprite.getLocalBounds().height / 2.f);;
    npcSprite.setTextureRect(animation[0]);
    npcSprite.setPosition(x,y);
    npcSprite.setScale(1.5f,1.5f);

    currentFrame = 0;
    elapsedTime = 0.f;
}

void FarmerGirl::update(float deltaTime)
{
     elapsedTime += deltaTime;

    if(elapsedTime >= frameDuration)
    {
        currentFrame = (currentFrame + 1) % 5;
        npcSprite.setTextureRect(animation[currentFrame]);
        elapsedTime = 0.f;
    }
}

void FarmerGirl::render(sf::RenderWindow& window)
{
     window.draw(npcSprite);
}

FarmerGirl::FarmerGirl(float x,float y)
{

    if (!npcTexture.loadFromFile("Textures/Npcs/farmer_02.png")) {
        std::cerr << "No se pudo cargar la textura del NPC Barman" << std::endl;
        return;
    }

    animation[0] = {0, 0, 32, 32};
    animation[1] = {32, 0, 32, 32};
    animation[2] = {64, 0, 32, 32};
    animation[3] = {96, 0, 32, 32};
    animation[4] = {128, 0, 32, 32};

    npcSprite.setTexture(npcTexture);
    npcSprite.setOrigin(npcSprite.getLocalBounds().width / 2.f, npcSprite.getLocalBounds().height / 2.f);;
    npcSprite.setTextureRect(animation[0]);
    npcSprite.setPosition(x,y);
    npcSprite.setScale(1.5f,1.5f);

    currentFrame = 0;
    elapsedTime = 0.f;
}
