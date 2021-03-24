//
// Created by jorge on 6/21/20.
//

#include "CMainMenu.hpp"
#include "CFpsCapper.hpp"

CMainMenu::CMainMenu(){
    m_Status = IN_MENU;
    m_LoadGame = false;
    m_RunGame = true;
    SDL_Surface * tempSurface = IMG_Load("src/assets/main_menu/main_menu.png");
    m_MainMenuTexture = SDL_CreateTextureFromSurface(CGame::m_Renderer, tempSurface);
    tempSurface = IMG_Load("src/assets/main_menu/credits.png");
    m_CreditsTexture = SDL_CreateTextureFromSurface(CGame::m_Renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
}

CMainMenu::~CMainMenu(){
}

void CMainMenu::run(){
    while (m_Status == IN_MENU || m_Status == IN_CREDITS){
        CFpsCapper::get().saveTicks();
        update();
        render();
        CFpsCapper::get().delay();
    }
    clear();
}

void CMainMenu::update(){
    SDL_PollEvent(&CGame::m_Event);
    if (CGame::m_Event.type == SDL_KEYDOWN){
        switch (m_Status) {
            case IN_MENU:
                switch (CGame::m_Event.key.keysym.sym) {
                    case SDLK_c:
                        m_Status = IN_CREDITS;
                        break;
                    case SDLK_f:
                        m_Status = START_NEW;
                        break;
                    case SDLK_l:
                        m_Status = START_LOAD;
                        m_LoadGame = true;
                        break;
                    case SDLK_ESCAPE:
                        m_Status = EXIT;
                        m_RunGame = false;
                        break;
                    default:
                        break;
                }
                break;
            case IN_CREDITS:
                switch (CGame::m_Event.key.keysym.sym) {
                    case SDLK_c:
                        m_Status = IN_MENU;
                        break;
                    case SDLK_ESCAPE:
                        m_Status = EXIT;
                        m_RunGame = false;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void CMainMenu::render(){
    SDL_RenderClear(CGame::m_Renderer);
    switch (m_Status) {
        case IN_MENU:
            CTextureManager::drawFullWindow(m_MainMenuTexture);
            break;
        case IN_CREDITS:
            CTextureManager::drawFullWindow(m_CreditsTexture);
            break;
        default:
            break;
    }
    SDL_RenderPresent(CGame::m_Renderer);
}

void CMainMenu::clear(){
    SDL_DestroyTexture(m_MainMenuTexture);
    SDL_DestroyTexture(m_CreditsTexture);
}

bool CMainMenu::loadGame() {
    return m_LoadGame;
}

bool CMainMenu::runGame() {
    return m_RunGame;
}
