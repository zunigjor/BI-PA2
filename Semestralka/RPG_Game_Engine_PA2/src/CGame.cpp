//
// Created by jorge on 6/11/20.
//

#include "CGame.hpp"

#include <sstream>
#include <string>
#include <list>
#include <iostream>
#include <fstream>

#include "interface/CFpsCapper.hpp"
#include "interface/CLogger.hpp"
#include "interface/CMainMenu.hpp"
#include "interface/CExitScreen.hpp"

#include "EntityComponents/Components.hpp"

#include "CCollisions.hpp"
#include "CAssetFactory.hpp"

// Static variables init
CEntityManager CGame::entityManager;
SDL_Renderer * CGame::m_Renderer = nullptr;
SDL_Event CGame::m_Event;
bool CGame::m_Running = false;
SDL_Rect CGame::m_Camera;
CAssetFactory * CGame::m_AssetFactory = new CAssetFactory(&entityManager);
CEntity& CGame::player(entityManager.addEntity());

CGame::CGame():
    tiles(entityManager.getGroup(CGame::groupMap)),
    players(entityManager.getGroup(CGame::groupPlayer)),
    tileColliders(entityManager.getGroup(CGame::groupColliders)),
    monsters(entityManager.getGroup(CGame::groupMonsters)),
    items(entityManager.getGroup(CGame::groupItems)),
    NPCs(entityManager.getGroup(CGame::groupNPCs)),
    winning(entityManager.getGroup(CGame::groupWin)),
    UI(entityManager.addEntity()),
    map("mapTiles", TEXTURE_RESOLUTION, TEXTURE_SCALE)
{}

CGame::~CGame(){}

void CGame::run() {
    initWindow("Dungeon crawl!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT);
    CMainMenu mainMenu;
    mainMenu.run();
    if (mainMenu.runGame()){ // If the user proceeds with the game
        if (mainMenu.loadGame()){
            m_Running = loadGameObjects("examples/savefile/customSaveFile.save");
        } else {
            m_Running = loadGameObjects("examples/savefile/defaultSaveFile.save");
        }
        while (running() && !victory()){
            CFpsCapper::get().saveTicks();

            handleEvents();
            update();
            render();

            CFpsCapper::get().delay();
        }
        if (victory()){
            CExitScreen exitScreen;
            exitScreen.run();
        }
    }
    clean();
}
// This is pretty ugly code. It could get split into multiple methods... IDK... ¯\_( o.o)_/¯
void CGame::initWindow(const char *title, int xPos, int yPos, int Width, int Height) {
    // init and check SDL
    CLogger::printToLog("Initializing game. (0/3)");
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
        m_Running = true;
        CLogger::printToLog("OK: SDL initialized. (1/3)");
        // init and check window
        m_Window = SDL_CreateWindow(title,xPos,yPos,Width,Height, 0);
        if (m_Window){
            CLogger::printToLog("OK: Window created. (2/3)");
        } else {
            CLogger::printToLog("ERROR!");
            CLogger::printToLog(SDL_GetError());
            m_Running = false;
        }
        // init and check renderer
        m_Renderer = SDL_CreateRenderer(m_Window,-1,0);
        if (m_Renderer) {
            SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 0);
            CLogger::printToLog("OK: Renderer created. (3/3)");
        } else {
            CLogger::printToLog("ERROR!");
            CLogger::printToLog(SDL_GetError());
            m_Running = false;
        }
        if(TTF_Init() == -1){
            CLogger::printToLog("ERROR!");
            CLogger::printToLog("SDL_TTF failed to load");
            m_Running = false;
        }
    } else {
        CLogger::printToLog("ERROR!");
        CLogger::printToLog(SDL_GetError());
        m_Running = false;
    }
}

void CGame::handleEvents() {
    SDL_PollEvent(&CGame::m_Event);
    switch (CGame::m_Event.type) {
        case SDL_QUIT:
            m_Running = false;
            break;
        default:
            break;
    }
    if (CGame::m_Event.type == SDL_KEYDOWN) {
        switch (CGame::m_Event.key.keysym.sym) {
            case SDLK_n:
                if(player.getComponent<CHealthComponent>().alive()){
                    saveGame("examples/savefile/customSaveFile.save");
                    CLogger::printToLog("saved");
                } else
                CLogger::printToLog("Player dead, no saving..!");
                break;
            default:
                break;
        }
    }
}

void CGame::update() {
    // Get player position
    SDL_Rect playerCol = player.getComponent<CCollisionComponent>().m_Collider;
    CVector2D playerPos = player.getComponent<CTransformationComponent>().m_Position;
    // Save player info to variables for easier output and code readability.
    int playerHealth = player.getComponent<CHealthComponent>().getCurrentHealth();
    int playerMaxHealth = player.getComponent<CHealthComponent>().getMaxHealth();
    int playerDamage = player.getComponent<CHealthComponent>().getDamage(false);
    // String stream to output information to the UI
    std::stringstream UIoutput;
    UIoutput << playerPos << " HP:" << playerHealth << " / " << playerMaxHealth << " DMG: " << playerDamage;
    UI.getComponent<CUIComponent>().setLabelText(UIoutput.str(), "arial16");
    // Refresh all entities and update them.
    entityManager.refresh();
    entityManager.update();
    // Player collision with the tiles
    for (auto& c : tileColliders){
        SDL_Rect cCol = c->getComponent<CCollisionComponent>().m_Collider;
        if(CCollisions::AABB(cCol, playerCol)){
            player.getComponent<CTransformationComponent>().m_Position = playerPos;
        }
    }
    // Player collision with monsters.
    for (auto& m : monsters){
        SDL_Rect mCol = m->getComponent<CCollisionComponent>().m_Collider;
        if(CCollisions::AABB(mCol, playerCol)){
            if (m->getComponent<CHealthComponent>().alive()){ // player takes damage, monsters attack goes on cooldown
                player.getComponent<CHealthComponent>().takeDamage(m->getComponent<CHealthComponent>().getDamage(true));
            }
            if ( player.getComponent<CKeyboardComponent>().isAttacking() ){ // monster takes damage, attack goes on cooldown
                m->getComponent<CHealthComponent>().takeDamage(player.getComponent<CHealthComponent>().getDamage(true));
            }
        }
    }
    // Player collisions with items.
    for (auto& i : items){
        SDL_Rect iCol = i->getComponent<CCollisionComponent>().m_Collider;
        if(CCollisions::AABB(iCol, playerCol)){
            i->getComponent<CItemComponent>().pickup(&player);
        }
    }
    // Player collisions with victory tiles.
    for (auto& win : winning){
        SDL_Rect wCol = win->getComponent<CCollisionComponent>().m_Collider;
        if(CCollisions::AABB(wCol, playerCol)){
            m_Victory = true;
        }
    }
    // Update camera position
    updateCamera();
}

void CGame::render() {
    SDL_RenderClear(m_Renderer);
    // Back
    for (auto& t: tiles) {t->draw();}
    for (auto& c: tileColliders) {c->draw();} // comment out to not draw collision indicators
    for (auto& i: items) {i->draw();}
    for (auto& NPC: NPCs) {NPC->draw();}
    // Player colisions with NPCs - this must be here so the NPC text renders.
    for (auto& NPC : NPCs){
        SDL_Rect NPCCol = NPC->getComponent<CCollisionComponent>().m_Collider;
        if(CCollisions::AABB(NPCCol, player.getComponent<CCollisionComponent>().m_Collider)){
            NPC->getComponent<CUIComponent>().display();
        }
    }
    for (auto& w: winning) {w->draw();}
    for (auto& m: monsters){m->draw();}
    for (auto& p: players) {p->draw();}
    UI.getComponent<CUIComponent>().display();
    // Front
    SDL_RenderPresent(m_Renderer);
}

void CGame::clean() {
    // Clean all entity groups
    for (auto& t: tiles) {t->destroy();}
    for (auto& c: tileColliders) {c->destroy();}
    for (auto& i: items) {i->destroy();}
    for (auto& m: monsters){m->destroy();}
    for (auto& p: players) {p->destroy();}
    for (auto& NPC: NPCs){NPC->destroy();}
    for (auto& w: winning){w->destroy();}
    delete m_AssetFactory;
    SDL_DestroyWindow(m_Window);
    SDL_DestroyRenderer(m_Renderer);
    SDL_Quit();
    CLogger::printToLog("Game cleaned!");
}

bool CGame::running() {
    return m_Running;
}

bool CGame::victory(){
    return m_Victory;
}

void CGame::updateCamera() {
    m_Camera.x = static_cast<int>(player.getComponent<CTransformationComponent>().m_Position.m_x - static_cast<int>(WINDOW_WIDTH/2));
    m_Camera.y = static_cast<int>(player.getComponent<CTransformationComponent>().m_Position.m_y - static_cast<int>(WINDOW_HEIGHT/2));
    if (m_Camera.x < 0) m_Camera.x = 0;
    if (m_Camera.y < 0) m_Camera.y = 0;
    if (m_Camera.x > m_Camera.w) m_Camera.x = m_Camera.w;
    if (m_Camera.y > m_Camera.h) m_Camera.y = m_Camera.h;
}

void CGame::addTextures() {
    m_AssetFactory->addTexture("mapTiles", "src/assets/maps/map_assets/map_ss.png");
    m_AssetFactory->addTexture("player","src/assets/player/player_ss.png");
    m_AssetFactory->addTexture("solid", "src/assets/maps/map_assets/solid.png");
    m_AssetFactory->addTexture("monster", "src/assets/monster/monster.png");
    m_AssetFactory->addTexture("sword", "src/assets/items/sword.png");
    m_AssetFactory->addTexture("potion", "src/assets/items/potion.png");
    m_AssetFactory->addTexture("signpost", "src/assets/signpost/signpost.png");
    m_AssetFactory->addTexture("victory", "src/assets/victory/victory.png");

    m_AssetFactory->addFont("arial16", "src/assets/font/arial.ttf", 16);
    SDL_Color white = {255,255,255,255};
    UI.addComponent<CUIComponent>(10, 10, "","arial16", white);
}

void CGame::initPlayer(int playerPosX, int playerPosY, int playerMS, int playerMaxHP, int playerDMG, int playerATTSPD) {
    // Be careful with the order of the components!
    player.addComponent<CTransformationComponent>(playerPosX, playerPosY, TEXTURE_SCALE, TEXTURE_RESOLUTION, TEXTURE_RESOLUTION, playerMS);
    player.addComponent<CSpriteComponent>("player", true);
    player.addComponent<CHealthComponent>(playerMaxHP, playerDMG, playerATTSPD);
    player.addComponent<CKeyboardComponent>();
    player.addComponent<CCollisionComponent>("player");
    player.addGroup(groupPlayer);
}

bool CGame::loadGameObjects(const char * file){
    addTextures();
    map.loadMap("examples/map/map_one");
    // The following lines of code represent the pseudo-format of the input file.
    // playerstart
    int playerPosX, playerPosY, playerMS, playerMaxHP, playerDMG, playerATTSPD;
    // monsterstart
    int numOfMonsters;
    int monsterPosX, monsterPosY, monsterMaxHP, monsterDMG, monsterATTSPD, monsterMS;
    // itermstart
    int numOfItems;
    int itemPosX, itemPosY, itemHeal, itemDamage;
    std::string itemName;
    // npcstart
    int numOfNpc;
    int NPCposX, NPCposY;
    std::string NPCtext;
    // winstart
    int winPosX, winPosY;
    // END OF DATA PSEUDO-FORMAT
    // file with data
    std::ifstream gameData(file, std::fstream::in);
    std::string line;
    std::stringstream ss;
    if (gameData.is_open()){
        // read player info
        std::getline(gameData, line);
        if (strcmp(line.c_str(), "playerstart") == 0){
            getline(gameData, line); ss << line;
            ss >> playerPosX; ss >> playerPosY;ss >> playerMS;
            ss >> playerMaxHP;ss >> playerDMG; ss >> playerATTSPD;
            initPlayer(playerPosX, playerPosY, playerMS, playerMaxHP, playerDMG, playerATTSPD);
            ss.clear();
        } else return false;
        // read monster info
        std::getline(gameData, line);
        if(strcmp(line.c_str(), "monsterstart") == 0){
            std::getline(gameData, line); ss << line;
            ss >> numOfMonsters; ss.clear();
            for (int j = 0; j < numOfMonsters; j++) {
                std::getline(gameData, line); ss << line;
                ss >> monsterPosX; ss >> monsterPosY;   ss >> monsterMaxHP;
                ss >> monsterDMG;  ss >> monsterATTSPD; ss >> monsterMS;
                m_AssetFactory->createMonster(&player,monsterPosX, monsterPosY, "monster", monsterMaxHP, monsterDMG, monsterATTSPD, monsterMS);
                ss.clear();
            }
        } else return false;
        // read item info
        std::getline(gameData, line);
        if(strcmp(line.c_str(), "itemstart") == 0){
            std::getline(gameData, line); ss << line;
            ss >> numOfItems; ss.clear();
            for (int j = 0; j < numOfItems; j++) {
                std::getline(gameData, line); ss << line;
                ss >> itemPosX; ss >> itemPosY; ss >> itemHeal; ss >> itemDamage;
                ss.clear();
                std::getline(gameData, line); ss << line;
                ss >> itemName;
                m_AssetFactory->createItem(itemPosX, itemPosY, itemHeal, itemDamage, itemName);
                ss.clear();
            }
        } else return false;
        // read npc info
        std::getline(gameData, line);
        if(strcmp(line.c_str(), "npcstart") == 0){
            std::getline(gameData, line); ss << line;
            ss >> numOfNpc; ss.clear();
            for (int j = 0; j < numOfNpc; j++) {
                std::getline(gameData, line); ss << line;
                ss >> NPCposX; ss >> NPCposY; ss.clear();
                std::getline(gameData, line);
                m_AssetFactory->createNPC(NPCposX, NPCposY, "signpost", line.c_str());
                ss.clear();
            }
        } else return false;
        // read victory tile info
        std::getline(gameData, line);
        if(strcmp(line.c_str(), "winstart") == 0) {
            std::getline(gameData, line); ss << line;
            ss >> winPosX; ss >> winPosY;
            m_AssetFactory->createWin(winPosX, winPosY, "victory");
            ss.clear();
        } else return false;
        gameData.close();
        m_Camera = {0,0,map.width() * TEXTURE_RESOLUTION, map.height() * TEXTURE_RESOLUTION};
        return true;
    }
    return false;
}



void CGame::saveGame(const char * file) {
    // format of variables to save
    int playerPosX = static_cast<int>(player.getComponent<CTransformationComponent>().m_Position.m_x);
    int playerPosY = static_cast<int>(player.getComponent<CTransformationComponent>().m_Position.m_x);
    int playerMS = player.getComponent<CTransformationComponent>().m_Speed;
    int playerMaxHP = player.getComponent<CHealthComponent>().getMaxHealth();
    int playerDMG = player.getComponent<CHealthComponent>().getDamage(false);
    int playerATTSPD = player.getComponent<CHealthComponent>().getAttackSpeed();
    std::ofstream gameData(file, std::fstream::out);
    if (gameData.is_open()){
        gameData << "playerstart" << std::endl;
        gameData << playerPosX << " " << playerPosY << " " << playerMS << " " << playerMaxHP << " " << playerDMG << " " << playerATTSPD << std::endl;
        gameData << "monsterstart" << std::endl;
        int monstersAlive = 0;
        for (auto& m: monsters) {
            if ( m->getComponent<CHealthComponent>().alive() )
                monstersAlive ++;
        }
        gameData << monstersAlive << std::endl;
        for (auto& m : monsters) {
            if (m->getComponent<CHealthComponent>().alive()){
                int monsterPosX = m->getComponent<CTransformationComponent>().m_Position.m_x;
                int monsterPosY = m->getComponent<CTransformationComponent>().m_Position.m_y;
                int monsterMaxHP = m->getComponent<CHealthComponent>().getMaxHealth();
                int monsterDMG = m->getComponent<CHealthComponent>().getDamage(false);
                int monsterATTSPD = m->getComponent<CHealthComponent>().getAttackSpeed();
                int monsterMS = m->getComponent<CTransformationComponent>().m_Speed;
                gameData << monsterPosX << " " << monsterPosY << " " << monsterMaxHP << " " << monsterDMG << " " << monsterATTSPD << " " << monsterMS << std::endl;
            }
        }
        gameData << "itemstart" << std::endl;
        gameData << items.size() << std::endl;
        for (auto& i : items) {
            int itemPosX = i->getComponent<CTransformationComponent>().m_Position.m_x;
            int itemPosY = i->getComponent<CTransformationComponent>().m_Position.m_y;
            int itemHeal = i->getComponent<CItemComponent>().getHeal();
            int itemDamage = i->getComponent<CItemComponent>().getDamage();
            std::string itemName = i->getComponent<CSpriteComponent>().getName();
            gameData << itemPosX << " " << itemPosY << " " << itemHeal << " " << itemDamage << std::endl;
            gameData << itemName << std::endl;
        }
        gameData << "npcstart" << std::endl;
        gameData << NPCs.size() << std::endl;
        for (auto& npc : NPCs) {
            int npcPosX = npc->getComponent<CTransformationComponent>().m_Position.m_x;
            int npcPosY = npc->getComponent<CTransformationComponent>().m_Position.m_y;
            std::string npcText = npc->getComponent<CUIComponent>().getText();
            gameData << npcPosX << " " << npcPosY << std::endl;
            gameData << npcText << std::endl;
        }
        gameData << "winstart" << std::endl;
        for (auto& win : winning) {
            int winPosX = win->getComponent<CTransformationComponent>().m_Position.m_x;
            int winPosY = win->getComponent<CTransformationComponent>().m_Position.m_y;
            gameData << winPosX << " " << winPosY << std::endl;
        }
        gameData.close();
    }
}
