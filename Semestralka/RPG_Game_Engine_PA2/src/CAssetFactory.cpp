//
// Created by jorge on 6/20/20.
//

#include "CAssetFactory.hpp"
#include "EntityComponents/Components.hpp"

CAssetFactory::CAssetFactory(CEntityManager *man):m_Manager(man) {

}
CAssetFactory::~CAssetFactory() {
    for (auto& t : m_Textures) {
        SDL_DestroyTexture(t.second);
    }
}
void CAssetFactory::addTexture(std::string texName, const char *pathToTexture) {
    m_Textures.emplace(texName, CTextureManager::loadTexture(pathToTexture));
}
SDL_Texture *CAssetFactory::getTexture(std::string texName) {
    return m_Textures[texName];
}


void CAssetFactory::createMonster(CEntity * entityToFollow,int x, int y, std::string texName, int health, int damage, int atkSpeed, int movementSpeed) {
    auto& monster(m_Manager->addEntity());
    // Be careful with the order of the components!
    monster.addComponent<CTransformationComponent>(x, y, TEXTURE_SCALE, TEXTURE_RESOLUTION, TEXTURE_RESOLUTION);
    monster.addComponent<CCollisionComponent>("monster");
    monster.addComponent<CHealthComponent>(health,damage, atkSpeed);
    monster.addComponent<CSpriteComponent>(texName,false);
    monster.addComponent<CAIComponent>(entityToFollow);
    monster.getComponent<CTransformationComponent>().m_Speed = movementSpeed;
    monster.addGroup(CGame::groupMonsters);
}

void CAssetFactory::createItem(int x, int y, int addHealth, int addDamage, std::string texName){
    // Be careful with the order of the components!
    auto& item(m_Manager->addEntity());
    item.addComponent<CTransformationComponent>(x, y, TEXTURE_SCALE, TEXTURE_RESOLUTION, TEXTURE_RESOLUTION);
    item.addComponent<CCollisionComponent>("item");
    item.addComponent<CSpriteComponent>(texName);
    item.addComponent<CItemComponent>(texName, addHealth, addDamage);
    item.addGroup(CGame::groupItems);
}

void CAssetFactory::addFont(std::string fontName, std::string path, int fontSize) {
    m_Fonts.emplace(fontName, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font *CAssetFactory::getFont(std::string fontName) {
    return m_Fonts[fontName];
}

void CAssetFactory::createNPC(int x, int y, std::string texName, std::string textSay) {
    auto& NPC(m_Manager->addEntity());
    NPC.addComponent<CTransformationComponent>(x, y, TEXTURE_SCALE, TEXTURE_RESOLUTION, TEXTURE_RESOLUTION);
    NPC.addComponent<CCollisionComponent>("NPC");
    NPC.addComponent<CSpriteComponent>(texName);
    SDL_Color white = {255,255,255,255};
    NPC.addComponent<CUIComponent>(10, 40, textSay, "arial16", white);
    NPC.addGroup(CGame::groupNPCs);
}

void CAssetFactory::createWin(int x, int y, std::string texName) {
    auto& win(m_Manager->addEntity());
    win.addComponent<CTransformationComponent>(x, y, TEXTURE_SCALE, TEXTURE_RESOLUTION, TEXTURE_RESOLUTION);
    win.addComponent<CCollisionComponent>("victory");
    win.addComponent<CSpriteComponent>(texName);
    win.addGroup(CGame::groupWin);
}

