#ifndef SCENE_HPP_INCLUDED
#define SCENE_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <memory>
#include "observer.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "witch.hpp"
#include "npc.hpp"

class Scene : public Observer
{
    protected:
        const float WIDTH = 1366;
        const float HEIGHT = 768;
        PlayerAnimations playerAnimations;
        bool drawPlayer;
        SceneState nextScene;
    public:
        virtual ~Scene() = default;

        virtual void render(sf::RenderWindow&) = 0;

        virtual void update(sf::RenderWindow&, float) = 0;

        virtual void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) {};

        virtual void setEnemySprites(const std::vector<Enemy*>& enemiesReferences) {};

        virtual bool shouldTransition() const { return false; };

        virtual bool gameOverBool() { return false; };

        virtual SceneState nextSceneState() const = 0;
};

class MenuScene : public Scene
{
    public:
        MenuScene();

        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        bool shouldTransition() const override;
        SceneState nextSceneState() const override;
        sf::Texture backgroundTexture;
        sf::Sprite background;
    private:
        sf::Font font;
        sf::Text menuOptions[3];
        int currentOption = 0;
};

class HouseScene : public Scene
{
    public:
        HouseScene();

        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
        bool shouldTransition() const override;
        SceneState nextSceneState() const override;
        sf::Texture candleText, tableText, houseText, bedText, layerTexture1, layerTexture2, layerTexture3, layerTexture4, layerTexture5;
        sf::Sprite candle, table, house, bed, layer1, layer2, layer3, layer4, layer5;
        void startCinematic(bool);
    private:
        float parallaxFactor5 = .04f;
        float parallaxFactor4 = .08f;
        float parallaxFactor3 = .032f;
        float parallaxFactor2 = .018f;
        float parallaxFactor1 = .020f;
        bool transition = false;
};

class CityScene : public Scene
{
    public:
        CityScene();

        void sensorNotify(bool,SceneState)override;
        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
        bool shouldTransition() const override;
        SceneState nextSceneState() const override;
        sf::Texture backgroundTexture;
        sf::Sprite background;

        sf::Texture cartelTexture;
        sf::Sprite cartel;

        sf::Texture barrilTexture;
        sf::Sprite barril;
        sf::Sprite barril2;

        sf::Texture farolasTexture ;
        sf::Sprite farolas;
        sf::Sprite farolas2;

        sf::Texture cajasTexture ;
        sf::Sprite cajas;

        sf::Texture carretaTexture ;
        sf::Sprite carreta;

        sf::Texture fozaTexture ;
        sf::Sprite foza;



        std::vector<std::unique_ptr<Npc>> npcs;
    private:
        bool transition = false;
        bool sensorActive = false;

};

class ForestScene : public Scene
{
    public:
        ForestScene();

        void sensorNotify(bool,SceneState)override;
        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
        bool shouldTransition() const override;
        SceneState nextSceneState() const override;
        sf::Texture backgroundTexture;
        sf::Sprite background;

        sf::Texture plantaTexture;
        sf::Sprite planta;

        sf::Texture fondoTexture;
        sf::Sprite fondo;
        std::vector<std::unique_ptr<Npc>> npcs;
    private:
    bool sensorActive = false;
    bool transition = false;
};

class DungeonScene : public Scene
{
    public:
        DungeonScene();

        void sensorNotify(bool,SceneState)override;
        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
        bool shouldTransition() const override;
        SceneState nextSceneState() const override;
        sf::Texture backgroundTexture;
        sf::Sprite background;
        std::vector<std::unique_ptr<Npc>> npcs;
        private:
        bool transition = false;

};

class CastleScene : public Scene
{
    public:
        CastleScene();

        void sensorNotify(bool,SceneState)override;
        void notify(ObserverEvents) override;
        void update(sf::RenderWindow&, float) override;
        void render(sf::RenderWindow&) override;
        bool shouldTransition() const override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
        SceneState nextSceneState() const override;
        sf::Texture backgroundTexture;
        sf::Sprite background;
        std::vector<std::unique_ptr<Npc>> npcs;
        private:
        bool transition = false;

};

class BarScene : public Scene
{
    public:
        BarScene();
        void sensorNotify(bool,SceneState)override;
        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
        bool shouldTransition() const override;
        SceneState nextSceneState() const override;
        sf::Texture barTexture;
        sf::Sprite bar;
        sf::Texture arcadeTexture;
        sf::Sprite arcade;
        sf::Texture pisoTexture;
        sf::Sprite piso;
        sf::Texture nubesTexture;
        sf::Sprite nubes;
        sf::Texture castillosTexture;
        sf::Sprite castillos;
        sf::Texture mesaTexture;
        sf::Sprite mesa;
        sf::Music music;
        sf::Texture copaTexture;
        sf::Sprite copa;
        std::vector<std::unique_ptr<Npc>> npcs;
    private:
        bool transition = false;
        bool sensorActive = false;

};

class TestScene : public Scene
{
    public:
        TestScene();

        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
        void setEnemySprites(const std::vector<Enemy*>&) override;
        bool shouldTransition() const override;
        bool gameOverBool() override;
        SceneState nextSceneState() const override;
    private:
        std::vector<Enemy*> enemyReferences;
        Witch theWitch = Witch(700.f, 700.f);
};

#endif // SCENE_HPP_INCLUDED
