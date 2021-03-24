//
// Created by jorge on 6/11/20.
//

#ifndef RPG_GAME_ENGINE_CGAME_HPP
#define RPG_GAME_ENGINE_CGAME_HPP



#include "RPGconfig.hpp"
#include "EntityComponents/EntityComponent.hpp"
#include "CMap.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


class CCollisionComponent;
class CAssetFactory;

/**
 * Main game class.
 */
class CGame {
public:
    /**
     * Default constructor.
     * Initializes entity groups.
     * Map and UI
     */
    CGame();
    /// CGame destructor.
    ~CGame();
    /// Main command which runs the game.
    void run();
public:
    /// Allows access to SDL renderer
    static SDL_Renderer * m_Renderer;
    /// Allows access to SDL event
    static SDL_Event m_Event;
    /// States whether the fame is running.
    static bool m_Running;
    /// Game camera.
    static SDL_Rect m_Camera;
    /// Asset factory
    static CAssetFactory * m_AssetFactory;
    /// Player entity
    static CEntity & player;
    /// Entity manager
    static CEntityManager entityManager;
    /// Entity group labels
    enum groupLabel : std::size_t {
        groupMap,
        groupPlayer,
        groupColliders,
        groupItems,
        groupMonsters,
        groupNPCs,
        groupWin
    };
    /// Tiles entity group
    std::vector<CEntity*> & tiles;
    /// Players entity group
    std::vector<CEntity*> & players;
    /// Colliding objects entity group
    std::vector<CEntity*> & tileColliders;
    /// Monsters entity group
    std::vector<CEntity*> & monsters;
    /// Items entity group
    std::vector<CEntity*> & items;
    /// NPC entities group
    std::vector<CEntity*> & NPCs;
    /// Winning entity group
    std::vector<CEntity*> & winning;
private:
    /**
     * Initializes the fundamental SDL elements of the game, the window and the renderer.
     * @param title Game title
     * @param xPos Window X position.
     * @param yPos Window Y position.
     * @param Width Window width.
     * @param Height Window height.
     */
    void initWindow(const char * title, int xPos, int yPos, int Width, int Height);
    /**
     * Loads a game object savefile
     * Savefile must be in the right format!!!
     * Check the default savefile.
     *
     * @param file Path to savefile
     * @return truw
     */
    bool loadGameObjects(const char * file);
    /// Handles SDL_Event events. Closing the window. Key presses etc.
    void handleEvents();
    /// Updates game objects.
    void update();
    /// Renders game objects. Back to front.
    void render();
    /// Cleans everything including SDL objects.
    void clean();
    ///States whether the game is running or not.
    bool running();
    /// Returns whether the player has won or not.
    bool victory();
    /// Saves the game to a custom file.
    void saveGame(const char * file);
private:
    /// Initializes the player.
    void initPlayer(int playerPosX, int playerPosY, int playerMS, int playerMaxHP, int playerDMG, int playerATTSPD);
    /// Adds textures to asset manager
    void addTextures();
    /// Updates camera position
    static void updateCamera();
private:
    /// SDL_Window
    SDL_Window * m_Window;
    /// Bool indicating whether the game is won or not.
    bool m_Victory = false;
    /// UI entity
    CEntity & UI;
    /// Map
    CMap map;
};


#endif //RPG_GAME_ENGINE_CGAME_HPP
