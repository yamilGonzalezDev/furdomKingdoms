#ifndef NPC_HPP
#define NPC_HPP

#include <vector>
#include <memory>
#include <string>
#include "entity.hpp"
#include "player.hpp"
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class Npc
{
    protected:
        const float PPM=30.f;
        sf::Sprite npcSprite;
        sf::Texture npcTexture;
        int currentFrame;
        float frameDuration=0.f;
        float elapsedTime=0;

    public:
        virtual ~Npc()=default;
        virtual void update(float) = 0;
        virtual void render(sf::RenderWindow& window)= 0;

};

/**TAVERNA**/

class Barman : public Npc
{
    public:
        Barman(float,float);
        void update(float) override;
        void render(sf::RenderWindow& window) override;

    private:
        b2Vec2 velocity;
        void animate();
        sf::Texture textureRect;
        sf::Sprite npcSprite;
        sf::IntRect animation[5];

        const float frameDuration = .30f;

};

class Marcianito : public Npc
{
    public:
        Marcianito(float,float);
        void update(float) override;
        void render(sf::RenderWindow& window) override;
        private:
        void animate();
        sf::Texture textureRect;
        sf::Sprite npcSprite;
        sf::IntRect animation[3];


        const float frameDuration = .19f;


};

/**CIUDAD**/

class Dog : public Npc
{
    public:
    Dog (float,float);
    void update(float) override;
    void render(sf::RenderWindow& window) override;

    private:
    void animate();
    sf::Texture textureRect;
    sf::Sprite npcSprite;
    sf::IntRect animation[4];

    const float frameDuration = 0.20f;
};

class Farmer : public Npc
{
    public:
    Farmer (float,float);
    void update(float) override;
    void render(sf::RenderWindow& window) override;

    private:
    void animate();
    sf::Texture textureRect;
    sf::Sprite npcSprite;
    sf::IntRect animation[5];

    const float frameDuration = 0.20f;
};

class FarmerGirl : public Npc
{
    public:
    FarmerGirl (float,float);
    void update(float) override;
    void render(sf::RenderWindow& window) override;

    private:
    void animate();
    sf::Texture textureRect;
    sf::Sprite npcSprite;
    sf::IntRect animation[5];

    const float frameDuration = 0.20f;
};

class Villager : public Npc
{
    public:
    Villager (float,float);
    void update(float) override;
    void render(sf::RenderWindow& window) override;

    private:
    void animate();
    sf::Texture textureRect;
    sf::Sprite npcSprite;
    sf::IntRect animation[4];

    const float frameDuration = 0.20f;
};

#endif // NPC_H
