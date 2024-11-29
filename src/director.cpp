#include <iostream>
#include "director.hpp"
#include "colisions.hpp"
#include "entity.hpp"

void debugA(std::string a)
{
    //std::cout << "Mensaje de " + a << std::endl;
}

Director::Director() : WIDTH(1366), HEIGHT(768), fooDrawInstance(window)
{
    window.create(sf::VideoMode(WIDTH, HEIGHT), "Furdom Kingdoms");
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

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            std::cout << "Estoy apretando espacio, creeme" << std::endl;
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
        if(window.hasFocus())
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
    if(currentScene != nullptr && currentScene->shouldTransition())
    {
        changeScene = true;
        std::cout << "ChangeScene primero " << changeScene << std::endl;
        nextScene = currentScene->nextSceneState();
    }
    debugA("transition");
    if(changeScene)
    {
        fadeIn(deltaTime);
        switch(nextScene)
        {
            case SceneState::MainMenu:
                initMenuScene();
                break;
            case SceneState::House:
                initHouseScene();
                break;
            case SceneState::City:
                initCityScene();
                break;
            case SceneState::Default:
                break;
        }
        fadeOut(deltaTime);
        changeScene = false;
        std::cout << "ChangeScene después " << changeScene << std::endl;
    }
    else if(currentScene != nullptr)
    {
        if (currentScene)
            {
                currentScene->update(window, deltaTime);
            }
            else
            {
                debugA("currentScene es nullptr");
            }
    }
}

void Director::gameEvents(float deltaTime)
{
    debugA("comienzo gameEvents");
    if(world != nullptr)
    {
        debugA("entro");
        world->Step(timeStep, velocityIterations, positionIterations);
        debugA("worldStep");
    }
}

void Director::render()
{
    window.clear();
    if(changeScene)
    {
        window.draw(fadeRectangle);
    }
    if(currentScene != nullptr)
    {
        currentScene->render(window);
    }
    if(drawNpcs)
    {
    }
    debugA("currentScene");
    if(drawPlayer && player != nullptr)
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
    if(world != nullptr)
    {
        delete world;
        world = nullptr;
    }
    if(player != nullptr) player = nullptr;
    drawNpcs = false;
    drawPlayer = false;
    drawEnemies = true;

    setScene(new MenuScene);
    currentScene->init();

    if(currentScene != nullptr)
    {
        debugA("inicié correctamente");
    }
}

void Director::initHouseScene()
{
    world = new b2World(b2Vec2(0.f, 10.f));
    colisionCheck = new Colision();
    world->SetContactListener(colisionCheck);

    boundFactory = std::make_unique<SensorFactory>();

    sensor = boundFactory->createBound(world, 1000.f, 670.f, 100.f, 50.f, Kind::HOUSESENSOR);

    setScene(new HouseScene);

    currentScene->init();

    sensor->addObserver(currentScene);

    drawPlayer = true;

    fooDrawInstance.SetFlags( b2Draw::e_shapeBit );

    world->SetDebugDraw( &fooDrawInstance );

    boundFactory = std::make_unique<LimitsFactory>();

    boundFactory->createWall(world, 361.f, 656.f, 8.f, 64.f, Kind::WALLS);

    boundFactory->createWall(world, 791.f, 622.f, 8.f, 32.f, Kind::WALLS);

    boundFactory->createWall(world, 576.f, 560.f, 272.f, 32.f, Kind::LIMITS);

    boundFactory->createBound(world, 577.f, 712.f, 223.f, 8.f, Kind::FLOOR);

    boundFactory->createBound(world, 0.f, 723.f, 3000.f, 0.f, Kind::FLOOR);

    player = new Player();
    player->createPlayer(world, 400.f, 658.f);
}

void Director::initCityScene()
{
    setScene(new CityScene);

    if(currentScene) std::cout << "CurrentScene " << std::endl;

    currentScene->init();

    drawNpcs = true;
    drawPlayer = true;

    debugA("inicio bien");
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
        initHouseScene();
        alpha = 255;
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
        changeScene = false;
    }
}

void Director::setScene(Scene* newScene)
{
    if(currentScene)
    {
        delete currentScene;
        currentScene = nullptr;
    }
    currentScene = newScene;
}

Director::~Director()
{
    if(player != nullptr)
    {
        delete player;
        player = nullptr;
    }

    if(colisionCheck != nullptr)
    {
        delete colisionCheck;
        colisionCheck = nullptr;
    }

    if(sensor != nullptr)
    {
        delete sensor;
        sensor = nullptr;
    }

    if(currentScene != nullptr)
    {
        delete currentScene;
        currentScene = nullptr;
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
