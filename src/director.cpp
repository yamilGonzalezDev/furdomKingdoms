#include <iostream>
#include "director.hpp"
#include "colisions.hpp"
#include "entity.hpp"

Director::Director() : WIDTH(1366), HEIGHT(768), fooDrawInstance(window)
{
    window.create(sf::VideoMode(WIDTH, HEIGHT), "Furdom Kingdoms");
    window.setFramerateLimit(60);
    nextScene = SceneState::CITY;
    gameOver = false;
    drawPlayer = false;
    drawEnemies = false;
    transitioning = true;
    transitionState = TransitionState::LOADING;
    view.setSize(window.getSize().x * 0.5f, window.getSize().y * 0.5f);
    alpha = 0;
    fadeRectangle.setSize(sf::Vector2f(WIDTH, HEIGHT));
    fadeRectangle.setFillColor(sf::Color(0, 0, 0, alpha));
    fadeRectangle.setOrigin(WIDTH / 2.f, HEIGHT / 2.f);

    text.setFont(font);
    text.setString("Game Over");
    text.setCharacterSize(100);
    text.setFillColor(sf::Color::Red);
    text.setOutlineThickness(2.f);
    text.setOutlineColor(sf::Color::White);
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

            if(gameOver && cooldownElapsed >= 3.f && event.type == sf::Event::KeyPressed)
            {
                transitioning = true;
                transitionState = TransitionState::LOADING;
                player->setHp(100.f);
            }
        }

        float deltaTime = clock.restart().asSeconds();

        updateScene(deltaTime);
        if(!gameOver)
        {
            if(!transitioning)
            {
                update(deltaTime);
                gameEvents();
            }
        }
        render();
    }
}

void Director::update(float deltaTime)
{
    if(drawPlayer && player != nullptr)
    {
        if(window.hasFocus())
        {
            player->timers(world, deltaTime);
            player->keyboardInput(world);
        }
    }
    if(drawEnemies)
    {
        for(auto it = enemies.begin(); it != enemies.end();)
        {
            Enemy* enemy = *it;

            enemy->logic(player);
            enemy->update(deltaTime);
            enemy->render(window);

            if(!enemy->getIsAlive())
            {
                enemy->destroy(world);
                it = enemies.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void Director::updateScene(float deltaTime)
{
    if(!transitioning && currentScene != nullptr && currentScene->shouldTransition())
    {
        transitioning = true;
        nextScene = currentScene->nextSceneState();
        transitionState = TransitionState::FADINGOUT;
    }

    if(transitioning || gameOver)
    {
        switch(transitionState)
        {
            case TransitionState::FADINGOUT:
                fadeOut(deltaTime);
                break;
            case TransitionState::LOADING:
                switch(nextScene)
                {
                    case SceneState::MENU:
                        initMenuScene();
                        break;
                    case SceneState::HOUSE:
                        initHouseScene();
                        break;
                    case SceneState::CITY:
                        initCityScene();
                        break;
                    case SceneState::BAR:
                        initBarScene();
                        break;
                    case SceneState::FOREST:
                        initForestScene();
                        break;
                    case SceneState::CASTLE:
                        initCastleScene();
                        break;
                    case SceneState::DUNGEON:
                        initDungeonScene();
                        break;
                    case SceneState::TEST:
                        initTestScene();
                        break;
                    case SceneState::DEFAULT:
                        break;
                }
                transitionState = TransitionState::FADINGIN;
                break;
            case TransitionState::FADINGIN:
                fadeIn(deltaTime);
                break;
            case TransitionState::GAMEOVER:
                {
                    if(!font.loadFromFile("Textures/font/menuFont.ttf"))
                    {
                        std::cerr << "Error al cargar la fuente del menú" << std::endl;
                        return;
                    }
                    fadeOut(deltaTime);
                    if(cooldownElapsed <= 3.f) cooldownElapsed += deltaTime;
                }
                break;
            case TransitionState::NONE:
                break;
            default:
                break;
        }
    }
    else if(currentScene != nullptr)
    {
        if(drawPlayer) currentScene->updatePlayer(deltaTime, player->getPos(), player->getScale(), player->getPlayerState());
        if(currentScene->gameOverBool())
        {
            cooldownElapsed = 0.f;
            gameOver = true;
            transitionState = TransitionState::GAMEOVER;
        }
        currentScene->update(window, deltaTime);
    }
}

void Director::gameEvents()
{
    if(world != nullptr)
    {
        world->Step(timeStep, velocityIterations, positionIterations);
    }
}

void Director::render()
{
    window.clear();
    if(currentScene != nullptr)
    {
        currentScene->render(window);
    }
    if(drawPlayer && player != nullptr)
    {
        view.setCenter(player->getPos().x * PPM, (player->getPos().y * PPM) - 50);
        window.setView(view);
    }
    if(world)
    {
        world->DebugDraw();
    }

    if(transitioning)
    {
        fadeRectangle.setPosition(view.getCenter());
        window.draw(fadeRectangle);
    }

    if(gameOver)
    {
        fadeRectangle.setPosition(view.getCenter());
        text.setPosition(view.getCenter().x - (text.getLocalBounds().width / 2.f), view.getCenter().y - text.getLocalBounds().height);
        window.draw(fadeRectangle);
        window.draw(text);
    }

    window.display();
}

void Director::initMenuScene()
{
    if(world != nullptr)
    {
        delete world;
        world = nullptr;
    }
    gameOver = false;
    drawPlayer = false;
    drawEnemies = true;

    setScene(new MenuScene);
}

void Director::initHouseScene()
{
    gameOver = false;

    initWorld(10.f);

    setScene(new HouseScene);

    boundFactory = std::make_unique<SensorFactory>();

    sensor = boundFactory->createBound(world, 1000.f, 670.f, 100.f, 50.f, Kind::HOUSESENSOR);

    sensor->addObserver(currentScene);

    boundFactory->createBound(world, 576.f, 712.f, 221.f, 8.f, Kind::FLOOR);

    drawPlayer = true;

    boundFactory = std::make_unique<LimitsFactory>();

    boundFactory->createWall(world, 361.f, 656.f, 8.f, 64.f, Kind::WALLS);

    boundFactory->createWall(world, 791.f, 622.f, 8.f, 32.f, Kind::WALLS);

    boundFactory->createBound(world, 577.f, 712.f, 223.f, 8.f, Kind::FLOOR);

    boundFactory->createWall(world, 576.f, 560.f, 272.f, 32.f, Kind::LIMITS);

    boundFactory->createBound(world, 0.f, 723.f, 3000.f, 0.f, Kind::FLOOR);

    player = new Player();
    player->createPlayer(world, 400.f, 680.f);
}

void Director::initCityScene()
{
    gameOver = false;

    initWorld(10.f);

    boundFactory = std::make_unique<SensorFactory>();
    sensor = boundFactory->createBound(world, 1370.f, 670.f, 5.f, 100.f, Kind::NEXTSCENE);

    setScene(new CityScene);
    sensor->addObserver(currentScene);

    boundFactory = std::make_unique<SensorFactory>();

    sensor = boundFactory->createBound(world, 400.f, 672.f, 20.f, 28.f, Kind::BARDOOR);

    boundFactory = std::make_unique<LimitsFactory>();

    boundFactory->createBound(world, 100.f, 100.f, 100.f, 100.f, Kind::LIMITS);

    boundFactory->createBound(world, 666.f, 704.f, 1000.f, 0.f, Kind::FLOOR);

    drawPlayer = true;

    if(player != nullptr)
    {
        player->playerBody->SetTransform(b2Vec2(0.f / 30.f, 676.f / 30.f), 0.0f);
    }
    else
    {
        player = new Player;
        player->createPlayer(world, 100.f, 680.f);
    }

    sensor->addObserver(currentScene);
}

void Director::initBarScene()
{
    gameOver = false;

    initWorld(10.f);

    setScene(new BarScene);

    boundFactory = std::make_unique<SensorFactory>();

    sensor = boundFactory->createBound(world, 50.f, 672.f, 20.f, 28.f, Kind::BARDOOR);
    sensor->addObserver(currentScene);

    boundFactory = std::make_unique<LimitsFactory>();

    boundFactory->createBound(world, 100.f, 695.f, 798.f, 1.f, Kind::FLOOR);

    boundFactory->createWall(world, 695.f, 630.f, 8.f, 80.f, Kind::WALLS);

    boundFactory->createWall(world, 50.f, 570.f, 8.f, 65.f, Kind::WALLS);

    boundFactory->createWall(world, 500.f, 540.f, 230.f, 10.f, Kind::WALLS);

    boundFactory->createWall(world, 16.f, 630.f, 8.f, 80.f, Kind::WALLS);

    boundFactory = std::make_unique<SensorFactory>();
    sensor = boundFactory->createBound(world, 280.f, 672.f, 20.f, 28.f, Kind::ARCADE);

    drawPlayer = true;


    if(player != nullptr)
    {
        player->playerBody->SetTransform(b2Vec2(50.f / PPM, 670.f / PPM), 0.0f);
    }
    else
    {
        player = new Player;
        player->createPlayer(world, 50.f, 670.f);
    }
}

void Director::initForestScene()
{
    gameOver = false;

    initWorld(10.f);

    boundFactory = std::make_unique<SensorFactory>();

    sensor = boundFactory->createBound(world, 2999.f, 688.f, 40.f,2.f, Kind::ADUNGEON);

    setScene(new ForestScene);

    sensor->addObserver(currentScene);

    drawPlayer = true;

    boundFactory = std::make_unique<SensorFactory>();
    boundFactory->createBound(world, 0.f, 723.f, 3000.f, 0.f, Kind::FLOOR);

    boundFactory->createBound(world, 185.f, 576.f, 25.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 40.f, 593.f, 40.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 120.f, 670.f, 40.f, 2.f, Kind::WALLS);

    boundFactory->createBound(world, 263.f, 576.f, 23.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 320.f, 540.f, 14.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 432.f, 525.f, 78.f, 2.f, Kind::FLOOR);

    boundFactory->createBound(world, 327.f, 732.f, 40.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 386.f, 690.f, 50.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 483.f, 642.f, 50.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 592.f, 560.f, 65.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 654.f, 624.f, 65.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 878.f, 688.f, 110.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 850.f, 448.f, 33.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 824.f, 528.f, 88.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 995.f, 655.f, 35.f, 2.f, Kind::FLOOR);

    boundFactory->createBound(world, 1192.f, 588.f, 23.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1270.f, 605.f, 38.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1328.f, 557.f, 14.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1362.f, 525.f, 14.f, 2.f, Kind::FLOOR);

    boundFactory->createBound(world, 1080.f, 623.f, 73.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1464.f, 480.f, 72.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1583.f, 496.f, 32.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1609.f, 560.f, 40.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1745.f, 593.f, 145.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1865.f, 560.f, 25.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1930.f, 576.f, 25.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1993.f, 624.f, 24.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2159.f, 688.f, 208.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2049.f, 652.f, 12.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2768.f, 482.f, 15.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2768.f, 513.f, 48.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2868.f, 525.f, 65.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2390.f, 637.f, 23.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2505.f, 467.f, 40.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2593.f, 606.f, 145.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2582.f, 523.f, 22.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2546.f, 492.f, 13.f,2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2662.f, 556.f, 22.f,2.f, Kind::FLOOR);

    boundFactory = std::make_unique<LimitsFactory>();
    boundFactory->createBound(world, 40.f, 655.f, 40.f, 59.f, Kind::WALLS);
    boundFactory->createBound(world, 120.f, 690.f, 40.f, 20.f, Kind::WALLS);
    boundFactory->createBound(world, 185.f, 648.f, 25.f, 70.f, Kind::WALLS);


    boundFactory->createBound(world, 263.f, 578.f, 23.f, 2.f, Kind::WALLS);
    boundFactory->createBound(world, 320.f, 542.f, 14.f, 2.f, Kind::WALLS);
    boundFactory->createBound(world, 432.f, 527.f, 78.f, 2.f, Kind::WALLS);

    boundFactory->createBound(world, 327.f, 780.f, 40.f, 38.f, Kind::WALLS);
    boundFactory->createBound(world, 386.f, 768.f, 50.f, 78.f, Kind::WALLS);
    boundFactory->createBound(world, 483.f, 725.f, 50.f, 78.f, Kind::WALLS);
    boundFactory->createBound(world, 592.f, 640.f, 65.f, 78.f, Kind::WALLS);
    boundFactory->createBound(world, 654.f, 715.f, 65.f, 90.f, Kind::WALLS);
    boundFactory->createBound(world, 878.f, 765.f, 110.f, 78.f, Kind::WALLS);
    boundFactory->createBound(world, 850.f, 528.f, 33.f, 77.f, Kind::WALLS);
    boundFactory->createBound(world, 824.f, 540.f, 88.f, 13.f, Kind::WALLS);

    boundFactory->createBound(world, 1192.f, 588.f, 23.f, 2.f, Kind::WALLS);
    boundFactory->createBound(world, 1270.f, 605.f, 38.f, 2.f, Kind::WALLS);
    boundFactory->createBound(world, 1328.f, 557.f, 14.f, 2.f, Kind::WALLS);
    boundFactory->createBound(world, 1362.f, 525.f, 14.f, 2.f, Kind::WALLS);

    boundFactory->createBound(world, 995.f, 745.f, 35.f, 85.f, Kind::WALLS);
    boundFactory->createBound(world, 1080.f, 710.f, 73.f, 85.f, Kind::WALLS);
    boundFactory->createBound(world, 1464.f, 500.f, 72.f, 20.f, Kind::WALLS);
    boundFactory->createBound(world, 1583.f, 519.f, 32.f, 20.f, Kind::WALLS);
    boundFactory->createBound(world, 1609.f, 583.f, 40.f, 20.f, Kind::WALLS);
    boundFactory->createBound(world, 1745.f, 615.f, 145.f,20.f, Kind::WALLS);
    boundFactory->createBound(world, 1865.f, 598.f, 25.f,35.f, Kind::WALLS);
    boundFactory->createBound(world, 1930.f, 599.f, 25.f,20.f, Kind::WALLS);
    boundFactory->createBound(world, 1993.f, 637.f, 24.f,12.f, Kind::WALLS);
    boundFactory->createBound(world, 2159.f, 740.f, 208.f,50.f, Kind::WALLS);

    boundFactory->createBound(world, 2582.f, 523.f, 22.f,2.f, Kind::WALLS);
    boundFactory->createBound(world, 2593.f, 606.f, 145.f,2.f, Kind::WALLS);
    boundFactory->createBound(world, 2546.f, 492.f, 13.f,2.f, Kind::WALLS);
    boundFactory->createBound(world, 2662.f, 556.f, 22.f,2.f, Kind::WALLS);
    boundFactory->createBound(world, 2390.f, 637.f, 23.f,2.f, Kind::WALLS);

    boundFactory->createBound(world, 2505.f, 490.f, 40.f,20.f, Kind::WALLS);
    boundFactory->createBound(world, 2594.f, 629.f, 145.f,20.f, Kind::WALLS);
    boundFactory->createBound(world, 2585.f, 679.f, 40.f,25.f, Kind::WALLS);
    boundFactory->createBound(world, 2768.f, 529.f, 15.f,45.f, Kind::WALLS);
    boundFactory->createBound(world, 2768.f, 529.f, 48.f,13.f, Kind::WALLS);
    boundFactory->createBound(world, 2999.f, 729.f, 40.f,40.f, Kind::WALLS);

    boundFactory->createBound(world, 2090.f, 425.f, 282.f,25.f, Kind::WALLS);
    boundFactory->createBound(world, 2088.f, 455.f, 233.f,25.f, Kind::WALLS);
    boundFactory->createBound(world, 2193.f, 525.f, 33.f,50.f, Kind::WALLS);

    if(player != nullptr)
    {
        player->playerBody->SetTransform(b2Vec2(50.f / PPM, 500.f / PPM), 0.0f);
    }
    else
    {
        player = new Player;
        player->createPlayer(world, 50.f, 500.f);
    }
}

void Director::initDungeonScene()
{
    gameOver = false;

    initWorld(10.f);

    setScene(new DungeonScene);

    boundFactory = std::make_unique<LimitsFactory>();

    boundFactory->createBound(world, 106.f, 858.f, 150.f, 88.f, Kind::WALLS);
    boundFactory->createBound(world, 290.f, 986.f, 150.f, 88.f, Kind::WALLS);
    boundFactory->createBound(world, 488.f, 921.f, 57.f, 88.f, Kind::WALLS);
    boundFactory->createBound(world, 650.f, 920.f, 200.f, 87.f, Kind::WALLS);
    boundFactory->createBound(world, 1056.f, 923.f, 256.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 1391.f, 987.f, 80.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 1565.f, 1083.f, 100.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 1687.f, 1147.f, 23.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 1736.f, 1049.f, 25.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 2055.f, 1177.f, 121.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 2265.f, 1033.f, 25.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 2671.f, 1033.f, 321.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 3016.f, 1098.f, 25.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 3220.f, 1033.f, 180.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 3496.f, 1018.f, 88.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 4020.f, 1115.f, 440.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 4584.f, 1018.f, 136.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 4584.f, 1018.f, 136.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 4790.f, 917.f, 40.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 4880.f, 886.f, 34.f, 5.f, Kind::WALLS);

    boundFactory->createBound(world, 4880.f, 885.f, 34.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 4960.f, 852.f, 34.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 5055.f, 820.f, 50.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 5167.f, 998.f, 33.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 5240.f, 932.f, 25.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 5328.f, 885.f, 48.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 5407.f, 835.f, 32.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 5489.f, 868.f, 33.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 5489.f, 868.f, 33.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 5655.f, 820.f, 137.f, 5.f, Kind::WALLS);
    boundFactory->createBound(world, 6007.f, 1095.f, 137.f, 120.f, Kind::WALLS);

    boundFactory->createBound(world, 200.f, 582.f, 600.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 1100.f, 615.f, 300.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 200.f, 582.f, 600.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 1673.f, 663.f, 300.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 2500.f, 663.f, 400.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 1800.f, 663.f, 300.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 2672.f, 730.f, 323.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 2908.f, 700.f, 323.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 3800.f, 558.f, 800.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 4585.f, 710.f, 135.f, 120.f, Kind::WALLS);
    boundFactory->createBound(world, 6007.f, 780.f, 137.f, 120.f, Kind::WALLS);

    boundFactory = std::make_unique<SensorFactory>();
    boundFactory->createBound(world, 106.f, 770.f, 150.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 290.f, 898.f, 150.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 488.f, 832.f, 57.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 582.f, 832.f, 30.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 650.f, 832.f, 200.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1056.f, 803.f, 256.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1391.f, 867.f, 80.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1565.f, 962.f, 100.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1687.f, 1027.f, 23.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 1736.f, 929.f, 25.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2055.f, 1057.f, 121.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2265.f, 913.f, 25.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 2671.f, 913.f, 321.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 3016.f, 978.f, 25.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 3220.f, 913.f, 180.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 3496.f, 897.f, 88.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 4020.f, 994.f, 440.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 4584.f, 897.f, 136.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 4584.f, 897.f, 136.f, 2.f, Kind::FLOOR);
    boundFactory->createBound(world, 4790.f, 913.f, 40.f, 1.f, Kind::FLOOR);
    boundFactory->createBound(world, 4880.f, 879.f, 34.f, 1.f, Kind::FLOOR);
    boundFactory->createBound(world, 4880.f, 879.f, 34.f, 1.f, Kind::FLOOR);
    boundFactory->createBound(world, 4960.f, 8546.f, 34.f, 1.f, Kind::FLOOR);
    boundFactory->createBound(world, 5055.f, 814.f, 50.f, 1.f, Kind::FLOOR);
    boundFactory->createBound(world, 5167.f, 993.f, 33.f, 1.f, Kind::FLOOR);
    boundFactory->createBound(world, 5240.f, 927.f, 25.f, 1.f, Kind::FLOOR);
    boundFactory->createBound(world, 5328.f, 879.f, 48.f, 1.f, Kind::FLOOR);
    boundFactory->createBound(world, 5407.f, 830.f, 32.f, 1.f, Kind::FLOOR);
    boundFactory->createBound(world, 5489.f, 864.f, 33.f, 1.f, Kind::FLOOR);
    boundFactory->createBound(world, 5489.f, 862.f, 33.f, 1.f, Kind::FLOOR);
    boundFactory->createBound(world, 5655.f, 815.f, 137.f, 1.f, Kind::FLOOR);
    boundFactory->createBound(world, 6007.f, 1090.f, 137.f, 120.f, Kind::FLOOR);

    boundFactory = std::make_unique<SensorFactory>();

    sensor = boundFactory->createBound(world, 6070.f, 920.f, 10.f, 50.f, Kind::CASTLE);

    sensor->addObserver(currentScene);

    drawPlayer = true;

    if(player != nullptr)
    {
        player->playerBody->SetTransform(b2Vec2(50.f / PPM, 670.f / PPM), 0.0f);
    }
    else
    {
        player = new Player;
        player->createPlayer(world, 6000.f, 800.f);
    }
}

void Director::initCastleScene()
{
    gameOver = false;

    initWorld(10.f);

    setScene(new CastleScene);

    boundFactory = std::make_unique<LimitsFactory>();

    boundFactory->createBound(world, 0.f, 723.f, 3000.f, 0.f, Kind::FLOOR);

    drawPlayer = true;

    if(player != nullptr)
    {
        player->playerBody->SetTransform(b2Vec2(400.f / PPM, 680.f / PPM), 0.0f);
    }
    else
    {
        player = new Player();
        player->createPlayer(world, 400.f, 680.f);
    }
}

void Director::initTestScene()
{
    gameOver = false;

    initWorld(9.8f);

    setScene(new TestScene);

    boundFactory = std::make_unique<SensorFactory>();

    boundFactory = std::make_unique<LimitsFactory>();

    boundFactory->createBound(world, 700.f, 700.f, 700.f, 1.f, Kind::FLOOR);

    enemyFactory = std::make_unique<GhostFactory>();

    //enemies.push_back(enemyFactory->createEnemy(world, 800.f, 700.f));

    enemyFactory = std::make_unique<SkeletonFactory>();

    //enemies.push_back(enemyFactory->createEnemy(world, 600.f, 700.f));

    enemyFactory = std::make_unique<GoblinFactory>();

    enemies.push_back(enemyFactory->createEnemy(world, 800.f, 700.f));

    drawPlayer = true;

    drawEnemies = true;

    if(player != nullptr)
    {
        player->playerBody->SetTransform(b2Vec2(4.f / PPM, 676.f / PPM), 0.0f);
    }
    else
    {
        player = new Player;
        player->createPlayer(world, 4.f, 676.f);
    }

    currentScene->setEnemySprites(enemies);
}

void Director::initWorld(float gravity)
{
    enemies.clear();

    if(world != nullptr)
    {
        cleanScene(world);
    }
    else
    {
        world = new b2World(b2Vec2(0.f, gravity));
    }

    if(colisionCheck == nullptr)
    {
        colisionCheck = new Colision();
    }

    world->SetContactListener(colisionCheck);

    fooDrawInstance.SetFlags(b2Draw::e_shapeBit);

    world->SetDebugDraw(&fooDrawInstance);
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

void Director::cleanScene(b2World* world)
{
    if(world != nullptr)
    {
        for(b2Body* body = world->GetBodyList(); body != nullptr; body = body->GetNext())
        {
            UserdataTag* tag = reinterpret_cast<UserdataTag*>(body->GetUserData().pointer);

            if(body == player->getBody())
            {
                continue;
            }
            else
            {
                delete tag;
                world->DestroyBody(body);
            }
        }
    }
}

void Director::fadeOut(float deltaTime)
{
    if(alpha < 255)
    {
        alpha += 200 * deltaTime;
        if(alpha > 255)
        {
            alpha = 255;
        }
        fadeRectangle.setFillColor(sf::Color(0, 0, 0, alpha));
    }
    else
    {
        if(gameOver)
        {
            return;
        }
        transitionState = TransitionState::LOADING;
    }
}

void Director::fadeIn(float deltaTime)
{
    if(alpha > 0)
    {
        alpha -= 200 * deltaTime;
        if(alpha < 0)
        {
            alpha = 0;
        }
        fadeRectangle.setFillColor(sf::Color(0, 0, 0, alpha));
    }
    else
    {
        transitionState = TransitionState::NONE;
        transitioning = false;
    }
}

Director::~Director()
{

    if(world != nullptr)
    {
        if(world->GetBodyCount() != 0)
        {
            for(b2Body* body = world->GetBodyList(); body != nullptr;body = body->GetNext())
            {
                UserdataTag* tag = reinterpret_cast<UserdataTag*>(body->GetUserData().pointer);

                delete tag;

                world->DestroyBody(body);
            }
            delete world;
        }
        world = nullptr;
    }
    if(player != nullptr)
    {
        delete player;
        player = nullptr;
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
    if(colisionCheck != nullptr)
    {
        delete colisionCheck;
        colisionCheck = nullptr;
    }
}
