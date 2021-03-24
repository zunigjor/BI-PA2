//
// Created by jorge on 6/19/20.
//

#ifndef RPG_GAME_ENGINE_PA2_CTILECOMPONENT_HPP
#define RPG_GAME_ENGINE_PA2_CTILECOMPONENT_HPP

#include <SDL2/SDL.h>

#include "EntityComponent.hpp"
#include "CTransformationComponent.hpp"
#include "CSpriteComponent.hpp"

/**
 * Tile component represents all map tiles that are drawn on the screen.
 */
class CTileComponent : public CComponent {
public:
    CTileComponent() = default;
    ~CTileComponent(){
    }
    CTileComponent(int x, int y, int id, std::string texName, int tileResolution, int tileScale){
        m_Position.m_x = x;
        m_Position.m_y = y;

        m_Texture = CGame::m_AssetFactory->getTexture(texName);

        m_SourceRect.x = id * tileResolution;
        m_SourceRect.y = 0; // All tile textures are in the same row
        m_SourceRect.w = m_SourceRect.h = tileResolution;

        m_DestRect.x = x;
        m_DestRect.y = y;
        m_DestRect.w = m_DestRect.h = tileResolution * tileScale;
    }
    /// Updates the position of the tiles accordingly to the camera position.
    void update() override {
        m_DestRect.x = static_cast<int>(m_Position.m_x - CGame::m_Camera.x);
        m_DestRect.y = static_cast<int>(m_Position.m_y - CGame::m_Camera.y);
    }
    void draw() override {
        CTextureManager::draw(m_Texture, m_SourceRect, m_DestRect);
    }
public:
    SDL_Texture * m_Texture;
    SDL_Rect m_SourceRect;
    SDL_Rect m_DestRect;
    CVector2D m_Position;
};


#endif //RPG_GAME_ENGINE_PA2_CTILECOMPONENT_HPP
