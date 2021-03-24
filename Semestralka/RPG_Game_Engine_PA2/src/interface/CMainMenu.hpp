//
// Created by jorge on 6/21/20.
//

#ifndef RPG_GAME_ENGINE_PA2_CMAINMENU_HPP
#define RPG_GAME_ENGINE_PA2_CMAINMENU_HPP

#include "../CGame.hpp"

/**
 * Used to display the main menu of the game.
 */
class CMainMenu {
public:
    CMainMenu();
    ~CMainMenu();
    void run();
    void update();
    void render();
    void clear();
public:
    /// Returns whether to load the game from a file or not.
    bool loadGame();
    /// Returns whether to run the game or not.
    bool runGame();
private:
    enum {IN_MENU, IN_CREDITS, START_NEW, START_LOAD, EXIT} m_Status;
    SDL_Texture * m_MainMenuTexture;
    SDL_Texture * m_CreditsTexture;
    bool m_LoadGame;
    bool m_RunGame;
};


#endif //RPG_GAME_ENGINE_PA2_CMAINMENU_HPP
