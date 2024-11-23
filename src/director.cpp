#include <iostream>
#include "director.hpp"
#include "collisions.hpp"
#include "entity.hpp"

void debugA(std::string a)
{
    std::cout << "Mensaje de " + a << std::endl;
}

Director::Director() : WIDTH(1366), HEIGHT(768), window(sf::VideoMode(WIDTH, HEIGHT), "Furdom Kingdoms"), fooDrawInstance(window)
{
    window.setFramerateLimit(60);
    nextScene = SceneState::MainMenu;
    drawNpcs = false;
    cargando = false;
    aclarando = false;
    drawPlayer = false;
    oscureciendo = true;
    drawEnemies = false;
    currentScene = new MenuScene;
    view.setSize(/*window.getSize().x, window.getSize().y*/window.getSize().x * 0.5f, window.getSize().y * 0.5f);
    alpha = 0;
    fadeRectangle.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    fadeRectangle.setFillColor(sf::Color(0, 0, 0, alpha));
    subject.addObserver(currentScene);
}

void Director::run() ///buclePrincipal();
{
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {

        }

        debugA("Comienzo");
        updateScene(deltaTime);
        debugA("UpdateScene");
        update(deltaTime);
        debugA("update");        gameEvents(deltaTime);
        debugA("gameEvents");
        render();
        debugA("render");
    }
}

void Director::update(float deltaTime)
{
    if(drawPlayer && player != nullptr)
    {
        if(window.hasFocus() && player != nullptr && drawPlayer)
        {
            player->updatePhysics();
            player->updateAnimation(deltaTime);
            player->keyboardInput();
        }
    }
    debugA("player");
    if(drawNpcs)
    {
    }
    debugA("npcs");
    if(drawEnemies)
    {
    }
    debugA("enemy");
}

void Director::updateScene(float deltaTime)
{
    debugA("Comienzo updateScene");
    if(currentScene != nullptr && currentScene->shouldTransition()) changeScene = true;
    debugA("transition");
    if(changeScene)
    {
        switch(/*currentScene->*/ nextScene)
        {
            case SceneState::MainMenu:
                setScene(new MenuScene);
                break;
            case SceneState::House:
                setScene(new HouseScene);
                break;
            case SceneState::City:
                setScene(new CityScene);
                break;
            case SceneState::Default:
                break;
        }
    }
    else if(currentScene != nullptr)
    {
        currentScene->update(window, deltaTime);
    }
}

void Director::gameEvents(float deltaTime)
{
    if(world != nullptr)
    {
        world->Step(timeStep, velocityIterations, positionIterations);
    }
}

void Director::render()
{
    window.clear();
    currentScene->render(window);
    if(drawNpcs)
    {
    }
    debugA("currentScene");
    if(drawPlayer)
    {
        view.setCenter(player->getPos().x + 33.f, 570);
        window.setView(view);
        player->draw(window);
    }
    debugA("Player");
    if(world!=nullptr)
    {
        world->DebugDraw();
    }
    debugA("debugDraw");
    if(drawEnemies)
    {
        //enemy->render(window);
    }
    debugA("enemy");
    window.display();
}

void Director::initMenuScene()
{
    if(world != nullptr) world = nullptr;
    if(player != nullptr) player = nullptr;
    drawNpcs = false;
    drawPlayer = false;
    drawEnemies = true;

    setScene(new MenuScene);
    currentScene->init();

    sf::Texture texture;
    sf::Sprite sprite;

    if(!texture.loadFromFile("Textures/menu/scroll.png"))
    {
        return;
    }

    sprite.setTexture(texture);
    sprite.setScale(5.f, 5.f);
}

void Director::initHouseScene()
{
    subject.removeObserver(currentScene);

    if(currentScene != nullptr) currentScene = nullptr;

    currentScene = new HouseScene;

    subject.addObserver(currentScene);

    currentScene->init();

    b2Body* sensorBody;

    drawPlayer = true;

    drawEnemies = true;

    world = new b2World(b2Vec2(0.f, 10.f));
    collisionCheck = new Collision();
    world->SetContactListener(collisionCheck);

    fooDrawInstance.SetFlags( b2Draw::e_shapeBit );

    world->SetDebugDraw( &fooDrawInstance );

    createSensor(world, sensorBody, 1000.f, 506.f);

    ground = new Ground;
    ground->createGround(world, 0.f, 723.f, 3000.f, 0.f);

    Limits bounds;

    bounds.createWall(world, 361.f, 656.f, 8.f, 64.f);

    bounds.createWall(world, 791.f, 622.f, 8.f, 32.f);

    bounds.createFloor(world, 577.f, 712.f, 223.f, 8.f);

    player = new Player;
    player->createPlayer(world, 400.f, 658.f);

    /*enemy = new Ghost(world, 600.f, 500.f, 27.f, 23.f);*/

    subject.addObserver(player);
    subject.addObserver(collisionCheck);
}

void Director::initCityScene()
{
    if(currentScene != nullptr) currentScene = nullptr;

    currentScene = new CityScene;

    currentScene->init();

    drawNpcs = true;
    drawPlayer = true;
}

void Director::createSensor(b2World* world, b2Body*& body, float x, float y)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x / 30.f, y / 30.f);
    body = world->CreateBody(&bodyDef);
    b2PolygonShape objectBox;
    objectBox.SetAsBox(100.f / 30.f, 200.f / 30.f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &objectBox;
    fixtureDef.isSensor = true;
    fixtureDef.filter.categoryBits = CATEGORY_SENSOR;
    fixtureDef.filter.maskBits = CATEGORY_PLAYER;

    body->CreateFixture(&fixtureDef);

    initBody(body, Kind::SENSOR, this);
}

void Director::cleanScene(b2World* world)
{
    if(world != nullptr)
    {
        for (b2Body* body = world->GetBodyList(); body != nullptr; body = body->GetNext())
        {
            UserdataTag* tag = reinterpret_cast<UserdataTag*>(body->GetUserData().pointer);

            delete tag;

            world->DestroyBody(body);
        }

        delete world;
        world = nullptr;
    }

    if(ground != nullptr)
    {
        delete ground;
        ground = nullptr;
    }
}

void Director::fadeOut(float deltaTime)
{
    if (alpha < 255)
    {
        alpha += static_cast<int>(speed * deltaTime);
        fadeRectangle.setFillColor(sf::Color(0, 0, 0, alpha));
    }
    else
    {
        oscureciendo = false;
        aclarando = true;
        initHouseScene();
        alpha = 255;
        std::cout << "Escena cambiada a House" << std::endl;
    }
}

void Director::fadeIn(float deltaTime)
{
    if(alpha > 0)
    {
        alpha -= 5;
        fadeRectangle.setFillColor(sf::Color(0, 0, 0, alpha));
    }
    else
    {
        aclarando = false;
        changeScene = false;
    }
}

void Director::setScene(Scene* newScene)
{
    if (currentScene != nullptr)
    {
        delete currentScene;
        currentScene = nullptr;
    }
    currentScene = newScene;
}

Director::~Director()
{
    if(collisionCheck != nullptr)
    {
        delete collisionCheck;
        collisionCheck = nullptr;
    }

    if(player != nullptr)
    {
        delete player;
        player = nullptr;
    }

    if(ground != nullptr)
    {
        delete ground;
        ground = nullptr;
    }

    if(world != nullptr)
    {
        for (b2Body* body = world->GetBodyList(); body != nullptr; body = body->GetNext())
        {
            UserdataTag* tag = reinterpret_cast<UserdataTag*>(body->GetUserData().pointer);

            delete tag;

            world->DestroyBody(body);
        }
        delete world;
        world = nullptr;
    }
}
