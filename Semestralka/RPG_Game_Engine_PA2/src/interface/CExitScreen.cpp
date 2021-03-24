//
// Created by jorge on 6/21/20.
//

#include "CExitScreen.hpp"
#include <SDL2/SDL.h>
#include "../CTextureManager.hpp"
#include "../CGame.hpp"

CExitScreen::CExitScreen() {
    SDL_Surface * tempSurface = IMG_Load("src/assets/victory/victory_screen.png");
    m_VictoryScreen = SDL_CreateTextureFromSurface(CGame::m_Renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    m_Running = true;
}

CExitScreen::~CExitScreen() {
    SDL_DestroyTexture(m_VictoryScreen);
}

void CExitScreen::run() {
    while (m_Running){
        update();
        render();
    }
}

void CExitScreen::update() {
    SDL_PollEvent(&CGame::m_Event);
    if (CGame::m_Event.type == SDL_KEYDOWN && CGame::m_Event.key.keysym.sym == SDLK_ESCAPE){
        m_Running = false;
    }
}
void CExitScreen::render() {
    SDL_RenderClear(CGame::m_Renderer);
    CTextureManager::drawFullWindow(m_VictoryScreen);
    SDL_RenderPresent(CGame::m_Renderer);
}
