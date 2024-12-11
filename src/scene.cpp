#include "scene.hpp"
#include "entity.hpp"
#include "sceneState.hpp"

/**MENU**/
MenuScene::MenuScene()
{
    drawPlayer = false;

    if(!backgroundTexture.loadFromFile("Textures/menu/scroll.png"))
    {
        std::cerr << "Error cargando las texturas del menu" << std::endl;
        return;
    }

    if(!font.loadFromFile("Textures/font/menuFont.ttf"))
    {
        std::cerr << "Error al cargar la fuente del menú" << std::endl;
        return;
    }

    for(int i = 0; i < 3; i++)
    {
        menuOptions[i].setFont(font);
        menuOptions[i].setCharacterSize(50);
        menuOptions[i].setOutlineThickness(2.f);
    }

    float totalHeight = menuOptions[0].getGlobalBounds().height + 70.f;

    menuOptions[0].setString("Start");
    menuOptions[0].setOrigin(menuOptions[0].getGlobalBounds().width / 2, menuOptions[0].getGlobalBounds().height / 2);
    menuOptions[0].setPosition(WIDTH / 2, HEIGHT / 2 - totalHeight * 2);
    menuOptions[0].setFillColor(sf::Color::Yellow);

    menuOptions[1].setString("Settings");
    menuOptions[1].setOrigin(menuOptions[1].getGlobalBounds().width / 2, menuOptions[1].getGlobalBounds().height / 2);
    menuOptions[1].setPosition(WIDTH / 2, HEIGHT / 2 - 25);
    menuOptions[1].setFillColor(sf::Color::White);

    menuOptions[2].setString("Exit");
    menuOptions[2].setOrigin(menuOptions[2].getGlobalBounds().width / 2, menuOptions[1].getGlobalBounds().height / 2);
    menuOptions[2].setPosition(WIDTH / 2, HEIGHT / 2 + totalHeight);
    menuOptions[2].setFillColor(sf::Color::White);

    background.setTexture(backgroundTexture);
    background.setScale(13.f, 13.f);
    background.setOrigin(background.getLocalBounds().width / 2, background.getLocalBounds().height / 2);
    background.setPosition(WIDTH / 2, HEIGHT / 2);
}

void MenuScene::update(sf::RenderWindow& window, float deltaTime)
{
    const float cooldown = 0.4f;
    static float currentCooldown = 0.f;

    currentCooldown -= deltaTime;

    if((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))&& currentCooldown <= 0.f)
    {
        if (currentOption > 0)
            currentOption--;
        else
            currentOption = 2;

        currentCooldown = cooldown;
    }
    else if((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && currentCooldown <= 0.f)
    {
        if (currentOption < 2)
            currentOption++;
        else
            currentOption = 0;

        currentCooldown = cooldown;
    }

    for(int i = 0; i < 3; i++)
    {
        if(i == currentOption)
            menuOptions[i].setFillColor(sf::Color::Yellow);
        else
            menuOptions[i].setFillColor(sf::Color::White);
    }
}

void MenuScene::render(sf::RenderWindow& window)
{
    window.draw(background);

    window.draw(menuOptions[0]);
    window.draw(menuOptions[1]);
    window.draw(menuOptions[2]);
}

bool MenuScene::shouldTransition() const
{
    if(currentOption == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
    {
        return true;
    }
    return false;
}

void MenuScene::notify(ObserverEvents event)
{
    if(event != ObserverEvents::DEFAULT) return;
}

SceneState MenuScene::nextSceneState() const
{
    return SceneState::HOUSE;
}

/**CASA**/

HouseScene::HouseScene()
{
    if(!candleText.loadFromFile("Textures/houseLevel/candle.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }
    if(!houseText.loadFromFile("Textures/houseLevel/house.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }
    if(!bedText.loadFromFile("Textures/houseLevel/bed.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }
    if(!tableText.loadFromFile("Textures/houseLevel/table.png"))
    {
        std::cerr << "Error al cargar las texturas de la casa" << std::endl;
    }
    if(!layerTexture5.loadFromFile("Textures/houseLevel/layer5.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }
    if(!layerTexture4.loadFromFile("Textures/houseLevel/layer4.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }
    if(!layerTexture3.loadFromFile("Textures/houseLevel/layer3.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }
    if(!layerTexture2.loadFromFile("Textures/houseLevel/layer2.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }
    if(!layerTexture1.loadFromFile("Textures/houseLevel/layer1.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }

    layerTexture5.setRepeated(true);
    layerTexture4.setRepeated(true);
    layerTexture3.setRepeated(true);
    layerTexture2.setRepeated(true);
    layerTexture1.setRepeated(true);

    layer5.setTexture(layerTexture5);
    layer4.setTexture(layerTexture4);
    layer3.setTexture(layerTexture3);
    layer2.setTexture(layerTexture2);
    layer1.setTexture(layerTexture1);

    layer5.setScale(2.f, 2.f);
    layer4.setScale(2.f, 2.f);
    layer3.setScale(2.f, 2.f);
    layer2.setScale(2.f, 2.f);
    layer1.setScale(2.f, 2.f);

    candle.setTexture(candleText);
    candle.setTextureRect(sf::IntRect(0, 0, 12, 20));
    candle.setPosition(457.f, 662.f);

    house.setTexture(houseText);
    house.setPosition(305.f, 529.f);

    bed.setTexture(bedText);
    bed.setPosition(368.f, 673.f);

    table.setTexture(tableText);
    table.setPosition(450.f, 682.f);
}

void HouseScene::update(sf::RenderWindow& window, float deltaTime)
{
    sf::Vector2f cameraPos = window.getView().getCenter();

    layer5.setPosition(-fmod(cameraPos.x * parallaxFactor5, WIDTH), 130.f);
    layer4.setPosition(-fmod(cameraPos.x * parallaxFactor4, WIDTH), 145.f);
    layer3.setPosition(-fmod(cameraPos.x * parallaxFactor3, WIDTH), 150.f);
    layer2.setPosition(-fmod(cameraPos.x * parallaxFactor2, WIDTH), 695.f);
    layer1.setPosition(-fmod(cameraPos.x * parallaxFactor1, WIDTH), 680.f);

    layer5.setTextureRect(sf::IntRect(fmod(cameraPos.x * parallaxFactor5, WIDTH), 0, 9000, HEIGHT));
    layer4.setTextureRect(sf::IntRect(fmod(cameraPos.x * parallaxFactor4, WIDTH), 0, 9000, HEIGHT));
    layer3.setTextureRect(sf::IntRect(fmod(cameraPos.x * parallaxFactor3, WIDTH), 0, 9000, HEIGHT));
    layer2.setTextureRect(sf::IntRect(fmod(cameraPos.x * parallaxFactor2, WIDTH), 0, 9000, 70));
    layer1.setTextureRect(sf::IntRect(fmod(cameraPos.x * parallaxFactor1, WIDTH), 0, 9000, 77));
}

void HouseScene::updatePlayer(float deltaTime, b2Vec2 pos, sf::Vector2f spriteScale, PlayerState state)
{
    playerAnimations.update(deltaTime, pos, spriteScale, state);
}

void HouseScene::render(sf::RenderWindow& window)
{
    window.draw(layer5);
    window.draw(layer4);
    window.draw(layer3);
    window.draw(layer2);
    window.draw(house);
    window.draw(table);
    window.draw(candle);
    window.draw(bed);
    playerAnimations.draw(window);
    window.draw(layer1);
}

bool HouseScene::shouldTransition() const
{
    return transition;
}

void HouseScene::notify(ObserverEvents event)
{
    if(transition) return;

    if(event == ObserverEvents::TRANSITION)
    {
        transition = true;
    }
}

SceneState HouseScene::nextSceneState() const
{
    return SceneState::CITY;
}

/**CIUDAD**/

CityScene::CityScene()
{
    drawPlayer = false;

    if(!backgroundTexture.loadFromFile("Textures/City/mapppppp.png"))
    {
        std::cerr << "Error al cargar las texturas de la ciudad" << std::endl;
    }

    if(!cartelTexture.loadFromFile("Textures/City/cartel.png"))
    {
        std::cerr << "Error al cargar las texturas de la ciudad" << std::endl;
    }

    if(!barrilTexture.loadFromFile("Textures/City/barrilete.png"))
    {
        std::cerr << "Error al cargar las texturas de la ciudad" << std::endl;
    }

    if(!farolasTexture.loadFromFile("Textures/City/farolas.png"))
    {
        std::cerr << "Error al cargar las texturas de la ciudad" << std::endl;
    }

    if(!fozaTexture.loadFromFile("Textures/City/zanja.png"))
    {
        std::cerr << "Error al cargar las texturas de la ciudad" << std::endl;
    }

    if(!carretaTexture.loadFromFile("Textures/City/carretilla.png"))
    {
        std::cerr << "Error al cargar las texturas de la ciudad" << std::endl;
    }

    if(!cajasTexture.loadFromFile("Textures/City/cajuelas.png"))
    {
        std::cerr << "Error al cargar las texturas de la ciudad" << std::endl;
    }



    background.setTexture(backgroundTexture);
    background.setPosition(0.f, 320.f);

    cartel.setTexture(cartelTexture);
    cartel.setPosition(480.f,630.f);

    barril.setTexture(barrilTexture);
    barril.setPosition(460.f,674.f);

    barril2.setTexture(barrilTexture);
    barril2.setPosition(500.f,674.f);

    farolas.setTexture(farolasTexture);
    farolas.setPosition(500.f,597.f);

    farolas2.setTexture(farolasTexture);
    farolas2.setPosition(580.f,597.f);

    cajas.setTexture(cajasTexture);
    cajas.setPosition(20.f,638.f);

    foza.setTexture(fozaTexture);
    foza.setPosition(1139.f,643.f);

    carreta.setTexture(carretaTexture);
    carreta.setPosition(700.f,630.f);

    auto dog = std::make_unique<Dog>(190.f, 690.f);
    npcs.push_back(std::move(dog));

    auto farmer = std::make_unique<Farmer>(300.f, 685.f);
    npcs.push_back(std::move(farmer));

    auto farmerGirl = std::make_unique<FarmerGirl>(450.f, 683.f);
    npcs.push_back(std::move(farmerGirl));
}

void CityScene::update(sf::RenderWindow& window, float deltaTime)
{
    if(sensorActive && sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        transition = true;
    }

    for(auto& npc : npcs){
        npc->update(deltaTime);
    }
}

void CityScene::updatePlayer(float deltaTime, b2Vec2 pos, sf::Vector2f spriteScale, PlayerState state)
{
    playerAnimations.update(deltaTime, pos, spriteScale, state);
}

void CityScene::render(sf::RenderWindow& window)
{
    window.draw(background);
    window.draw(cartel);
    window.draw(barril);

    for(auto& npc : npcs){
        npc->render(window);
    }
    playerAnimations.draw(window);
    window.draw(foza);
    window.draw(barril2);
    window.draw(farolas2);
    window.draw(farolas);
    window.draw(cajas);
    window.draw(carreta);
}
bool CityScene::shouldTransition() const
{
    return transition;
}

void CityScene::sensorNotify(bool v, SceneState nextState)
{
    sensorActive = v;
    nextScene = nextState;

}

void CityScene::notify(ObserverEvents event)
{
    if(event == ObserverEvents::DEFAULT) std::cout << "Default" << std::endl;


    if(event == ObserverEvents::TRANSITION)
    {
        transition = true;
    }
}

SceneState CityScene::nextSceneState() const
{
    return nextScene;
}

/**BAR**/

BarScene::BarScene()
{
    if(!barTexture.loadFromFile("Textures/Taverna/bar.png"))
    {
        std::cerr << "Error al cargar la textura" << std::endl;
    }

        if(!arcadeTexture.loadFromFile("Textures/Taverna/arcade.png"))
    {
        std::cerr << "Error al cargar la textura" << std::endl;
    }


        if(!pisoTexture.loadFromFile("Textures/Taverna/piso.png"))
    {
        std::cerr << "Error al cargar la textura" << std::endl;
    }


        if(!nubesTexture.loadFromFile("Textures/Taverna/nubes.png"))
    {
        std::cerr << "Error al cargar la textura" << std::endl;
    }


        if(!castillosTexture.loadFromFile("Textures/Taverna/castillos.png"))
    {
        std::cerr << "Error al cargar la textura" << std::endl;
    }

    if(!mesaTexture.loadFromFile("Textures/Taverna/barTable.png"))
    {
        std::cerr << "Error al cargar la textura" << std::endl;
    }

        if(!copaTexture.loadFromFile("Textures/Taverna/copita.png"))
    {
        std::cerr << "Error al cargar la textura" << std::endl;
    }

    if(!music.openFromFile("Textures/Taverna/musiquita.mp3")){
        std::cerr <<"Error al cargar la musica"<<std::endl;
    }

    music.setLoop(true);
    music.setVolume(10.f);

    music.play();


    piso.setTexture(pisoTexture);
    piso.setPosition(-380.f ,320.f);
    castillos.setTexture(castillosTexture);
    castillos.setPosition(-380.f,305.f);
    nubes.setTexture(nubesTexture);
    nubes.setPosition(-380.f,300.f);
    arcade.setTexture(arcadeTexture);
    arcade.setPosition(215.f , 630.f);
    arcade.setScale(0.6f, 0.6f);
    mesa.setTexture(mesaTexture);
    mesa.setPosition(100.f,663.f);

    copa.setTexture(copaTexture);
    copa.setPosition(150.f,658.f);
    copa.setScale(0.6f,0.6f);

    bar.setTexture(barTexture);
    bar.setPosition(0.f, 390.f);

    auto marcianito = std::make_unique<Marcianito>(332.f,645.f);
    npcs.push_back(std::move(marcianito));

    auto barman = std::make_unique<Barman>(230.f, 680.f);
    npcs.push_back(std::move(barman));
}

void BarScene::update(sf::RenderWindow& window, float deltaTime)
{
if(sensorActive && sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        transition = true;
    }

    for(auto& npc : npcs){
        npc->update(deltaTime);
    }
}

void BarScene::updatePlayer(float deltaTime, b2Vec2 pos, sf::Vector2f spriteScale, PlayerState state)
{
    playerAnimations.update(deltaTime, pos, spriteScale, state);
}

void BarScene::render(sf::RenderWindow& window)
{

    window.draw(piso);
    window.draw(nubes);
    window.draw(castillos);
    window.draw(bar);
    window.draw(arcade);
    for (auto& npc : npcs){
    npc->render(window);
    }
    window.draw(mesa);
    window.draw(copa);
    playerAnimations.draw(window);

}

SceneState BarScene::nextSceneState() const
{
    return SceneState::CITY;
}

void BarScene::notify(ObserverEvents event)
{
    if(event == ObserverEvents::TRANSITION) transition = true;
      if(event == ObserverEvents::TRANSITION)
    {
        transition = true;
    }
}

bool BarScene::shouldTransition() const
{
    return transition;
}

void BarScene::sensorNotify(bool v,SceneState nextState)
{
    sensorActive = v;
    if(nextState == SceneState::BAR){
    nextScene = SceneState::CITY;
    }
}

/*BOSQUE*/

ForestScene::ForestScene()
{
    if(!backgroundTexture.loadFromFile("Textures/Forest/elbosquedemo2.png"))
    {
        std::cerr << "Error al cargar la textura" << std::endl;
    }
    if(!fondoTexture.loadFromFile("Textures/Forest/fondo.png"))
    {
        std::cerr << "Error al cargar la textura" << std::endl;
    }
     if(!plantaTexture.loadFromFile("Textures/Forest/plantitas.png"))
    {
        std::cerr << "Error al cargar la textura" << std::endl;
    }

    background.setTexture(backgroundTexture);
    background.setPosition(0.f, 400.f);

    planta.setTexture(plantaTexture);
    planta.setPosition(0.f, 400.f);

    fondo.setTexture(fondoTexture);
    fondo.setPosition(-500.f, 150.f);
    fondo.setScale(1.6f,1.6f);
}

void ForestScene::update(sf::RenderWindow& window, float deltaTime)
{
if(sensorActive && sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        transition = true;
    }

    for(auto& npc : npcs){
        npc->update(deltaTime);
    }
}

void ForestScene::updatePlayer(float deltaTime, b2Vec2 pos, sf::Vector2f spriteScale, PlayerState state)
{
    playerAnimations.update(deltaTime, pos, spriteScale, state);
}

void ForestScene::render(sf::RenderWindow& window)
{
    window.draw(fondo);
    window.draw(background);


    for (auto& npc : npcs){
    npc->render(window);
    }

    playerAnimations.draw(window);
    window.draw(planta);

}

SceneState ForestScene::nextSceneState() const
{
    return SceneState::DUNGEON;
}

void ForestScene::notify(ObserverEvents event)
{
    if(event == ObserverEvents::TRANSITION) transition = true;
}

bool ForestScene::shouldTransition() const
{
    return transition;
}

void ForestScene::sensorNotify(bool v,SceneState nextState)
{
    sensorActive = v;
}

/*DUNGEON*/

DungeonScene::DungeonScene()
{
    if(!backgroundTexture.loadFromFile("Textures/Dungeon/map.png"))
    {
        std::cerr << "Error al cargar la textura" << std::endl;
    }

    background.setTexture(backgroundTexture);
    background.setPosition(0.f, 400.f);
}

void DungeonScene::update(sf::RenderWindow& window, float deltaTime)
{


    for(auto& npc : npcs){
        npc->update(deltaTime);
    }
}

void DungeonScene::updatePlayer(float deltaTime, b2Vec2 pos, sf::Vector2f spriteScale, PlayerState state)
{
    playerAnimations.update(deltaTime, pos, spriteScale, state);
}

void DungeonScene::render(sf::RenderWindow& window)
{

    window.draw(background);

    playerAnimations.draw(window);
}

SceneState DungeonScene::nextSceneState() const
{
    return SceneState::CASTLE;
}

void DungeonScene::notify(ObserverEvents event)
{
    if(event == ObserverEvents::TRANSITION) transition = true;
}

bool DungeonScene::shouldTransition() const
{
    return transition;
}

void DungeonScene::sensorNotify(bool v,SceneState nextState)
{
    if(nextState == SceneState::DUNGEON){
    nextScene = SceneState::CASTLE;
    }
}

/*CASTILLO*/
CastleScene::CastleScene()
{
    if(!backgroundTexture.loadFromFile("Textures/Castle/castle.png"))
    {
        std::cerr << "Error al cargar la textura" << std::endl;
    }

    background.setTexture(backgroundTexture);
    background.setPosition(0.f, 16.f);
}

void CastleScene::update(sf::RenderWindow& window, float deltaTime)
{

    for(auto& npc : npcs){
        npc->update(deltaTime);
    }
}

void CastleScene::updatePlayer(float deltaTime, b2Vec2 pos, sf::Vector2f spriteScale, PlayerState state)
{
    playerAnimations.update(deltaTime, pos, spriteScale, state);
}

void CastleScene::render(sf::RenderWindow& window)
{

    window.draw(background);

    playerAnimations.draw(window);
}

SceneState CastleScene::nextSceneState() const
{
    return SceneState::DEFAULT;
}

void CastleScene::notify(ObserverEvents event)
{
    if(event == ObserverEvents::TRANSITION) transition = true;
}

bool CastleScene::shouldTransition() const
{
    return transition;
}

void CastleScene::sensorNotify(bool v,SceneState nextState)
{
    if(nextState == SceneState::DUNGEON){
    nextScene = SceneState::DEFAULT;
    }
}

/**TEST**/

TestScene::TestScene()
{
}

void TestScene::notify(ObserverEvents event)
{
    if(event != ObserverEvents::DEFAULT) return;
}

void TestScene::render(sf::RenderWindow& window)
{
    for(auto& enemy : enemyReferences)
    {
        window.draw(enemy->sprite);
    }

    playerAnimations.draw(window);

    theWitch.render(window);
}

void TestScene::update(sf::RenderWindow& window, float deltaTime)
{
    for(auto it = enemyReferences.begin(); it != enemyReferences.end();)
    {
        if(!(*it)->getIsAlive())
        {
            it = enemyReferences.erase(it);
        }
        else
        {
            ++it;
        }
    }

    theWitch.update(deltaTime);
}

void TestScene::updatePlayer(float deltaTime, b2Vec2 pos, sf::Vector2f spriteScale, PlayerState state)
{
    playerAnimations.update(deltaTime, pos, spriteScale, state);
}

bool TestScene::gameOverBool()
{
    return playerAnimations.getGameOver();
}

bool TestScene::shouldTransition() const
{
    return false;
}

SceneState TestScene::nextSceneState() const
{
    return SceneState::DEFAULT;
}

void TestScene::setEnemySprites(const std::vector<Enemy*>& enemyReferences)
{
    this->enemyReferences = enemyReferences;
}
