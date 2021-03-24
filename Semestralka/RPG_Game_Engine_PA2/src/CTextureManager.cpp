//
// Created by jorge on 6/11/20.
//

#include "CTextureManager.hpp"
#include "CGame.hpp"


SDL_Texture* CTextureManager::loadTexture(const char* texture)
{
    SDL_Surface* tempSurface = IMG_Load(texture);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(CGame::m_Renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
}

void CTextureManager::draw(SDL_Texture * tex, SDL_Rect src, SDL_Rect dest)
{
    SDL_RenderCopy(CGame::m_Renderer, tex, &src, &dest);
}

void CTextureManager::draw(SDL_Texture * tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
    SDL_RenderCopyEx(CGame::m_Renderer, tex, &src, &dest, 0, NULL, flip);
}

void CTextureManager::drawFullWindow(SDL_Texture * tex){
    SDL_Rect srcRect = {0,0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_Rect destRect = {0,0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderCopy(CGame::m_Renderer, tex, &srcRect, &destRect);
}