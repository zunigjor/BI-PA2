//
// Created by jorge on 6/20/20.
//

#ifndef RPG_GAME_ENGINE_PA2_CASSETFACTORY_HPP
#define RPG_GAME_ENGINE_PA2_CASSETFACTORY_HPP

#include <map>
#include <string>

#include <SDL2/SDL_ttf.h>

#include "CTextureManager.hpp"
#include "CVector2D.hpp"
#include "EntityComponents/EntityComponent.hpp"

/**
 * Class functions as a factory for textures and other game objects.
 */
class CAssetFactory {
public:
    CAssetFactory(CEntityManager * man);
    ~CAssetFactory();
    /// Creates a monster
    void createMonster(CEntity * entityToFollow, int x, int y, std::string texName, int health, int damage, int atkSpeed, int movementSpeed);
    /// Creates an item
    void createItem(int x, int y, int addHealth, int addDamage, std::string texName);
    /// Creates an NPC
    void createNPC(int x, int y, std::string texName, std::string textSay);
    /// Creates a victory enitiy
    void createWin(int x, int y, std::string texName);
    /// Adds a texture to the manager.
    void addTexture(std::string texName, const char * pathToTexture);
    /// Returns a texture.
    SDL_Texture * getTexture(std::string texName);
    /// Adds a font
    void addFont(std::string fontName, std::string path, int fontSize);
    /// Returns a font
    TTF_Font * getFont(std::string fontName);
private:
    CEntityManager * m_Manager;
    std::map<std::string, SDL_Texture*> m_Textures;
    std::map<std::string, TTF_Font*> m_Fonts;
};


#endif //RPG_GAME_ENGINE_PA2_CASSETFACTORY_HPP
