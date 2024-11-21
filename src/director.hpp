#ifndef DIRECTOR_HPP_INCLUDED
#define DIRECTOR_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "enemy.hpp"
#include "npc.hpp"
#include "player.hpp"
#include "limits.hpp"
#include "collisions.hpp"
#include "scene.hpp"
#include "fooDraw.hpp"
#include "observer.hpp"

enum class SceneState
{
    MainMenu,
    House,
    City
};

class Director
{
    public:
        Director();
        ~Director();
        void run();
        void switchScene(SceneState newState);
    private:
        const float WIDTH;
        const float HEIGHT;
        const float timeStep = 1.f/60.f;
        const float FLOORHEIGHT = 528.f;
        const int velocityIterations = 6;
        const int positionIterations = 2;
        Subject subject;
        sf::Sprite test2S;
        sf::Texture test2;
        sf::RenderWindow window;
        sf::View view;
        sf::Clock clock;
        FooDraw fooDrawInstance;
        SceneState nextScene;
        SceneState currentSceneState;
        bool pause;
        bool start;
        bool drawNpcs;
        bool drawPlayer;
        bool transitioning;
        bool oscureciendo;
        bool aclarando;
        bool cargando;
        int alpha;
        float transitionTime;
        float maxTransitionTime;
        sf::RectangleShape fadeRectangle;

        b2World* world = nullptr;
        b2Vec2 gravity;

        EnemyFactory factory;
        Enemy* enemy = nullptr;
        Ground* ground = nullptr;
        Player* player = nullptr;
        Scene* currentScene = nullptr;
        Collision* collisionCheck = nullptr;

        void render();
        void fadeIn();
        void fadeOut();
        void update(float);
        void initCityScene();
        void initMenuScene();
        void initHouseScene();
        void transition(float);
        void gameEvents(float);
        void updateScene(float);
        void cleanScene(b2World*);
        void createSensor(b2World*, b2Body*&, float, float);
};

#endif // DIRECTOR_HPP_INCLUDED
