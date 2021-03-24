//
// Created by jorge on 6/21/20.
//

#ifndef RPG_GAME_ENGINE_PA2_CEXITSCREEN_HPP
#define RPG_GAME_ENGINE_PA2_CEXITSCREEN_HPP

#include <SDL2/SDL.h>

/**
 * Exit screen class to present the victory screen.
 */
class CExitScreen {
public:
    CExitScreen();
    ~CExitScreen();
    void run();
    void update();
    void render();
private:
    SDL_Texture * m_VictoryScreen;
    bool m_Running;
};


#endif //RPG_GAME_ENGINE_PA2_CEXITSCREEN_HPP
